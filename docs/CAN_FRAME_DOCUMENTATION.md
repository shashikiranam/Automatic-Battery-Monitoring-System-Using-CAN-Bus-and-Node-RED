# CAN Frame Documentation

This document defines the CAN bus message set used by the Automatic Battery
Monitoring System. All frames are **standard (11-bit) identifiers**, sent at
**500 kbps**, with a fixed **DLC of 8 bytes**. Unused trailing bytes are
zero-padded.

All multi-byte numeric fields are **big-endian** (most significant byte
first) signed 16-bit integers, scaled by the factor listed below to preserve
fractional precision without using floating point on the wire.

| Signal        | CAN ID  | Bytes | Type       | Scale | Unit | Range (approx.)     |
|---------------|---------|-------|------------|-------|------|----------------------|
| Voltage       | `0x100` | 0-1   | int16 BE   | 100   | V    | -327.68 to 327.67 V  |
| Current       | `0x101` | 0-1   | int16 BE   | 100   | A    | -327.68 to 327.67 A  |
| Temperature   | `0x102` | 0-1   | int16 BE   | 100   | °C   | -327.68 to 327.67 °C |
| Weight        | `0x103` | 0-1   | int16 BE   | 1000  | kg   | -32.768 to 32.767 kg |
| Power         | `0x104` | 0-1   | int16 BE   | 100   | W    | -327.68 to 327.67 W  |
| System Status | `0x105` | 0-3   | see below  | n/a   | n/a  | n/a                  |

Decode formula for signals `0x100`–`0x104`:

```
raw_int16 = (byte[0] << 8) | byte[1]
value     = raw_int16 / scale_factor
```

---

## 0x100 — Battery Voltage

| Byte | Field         | Description                          |
|------|---------------|---------------------------------------|
| 0    | Voltage MSB   | High byte of `voltage_v * 100`        |
| 1    | Voltage LSB   | Low byte of `voltage_v * 100`         |
| 2-7  | Reserved      | Always `0x00`                         |

Example: `voltage_v = 11.85 V` → raw = 1185 = `0x04A1` → bytes `[0x04, 0xA1]`

---

## 0x101 — Battery Current

| Byte | Field         | Description                                          |
|------|---------------|--------------------------------------------------------|
| 0    | Current MSB   | High byte of `current_a * 100` (signed)                |
| 1    | Current LSB   | Low byte of `current_a * 100`                          |
| 2-7  | Reserved      | Always `0x00`                                          |

Positive values indicate discharge current; the sign convention is defined
by how the INA219 shunt is wired (see `hardware/PIN_CONFIGURATION.md`).

---

## 0x102 — Battery Temperature

| Byte | Field            | Description                                |
|------|------------------|----------------------------------------------|
| 0    | Temperature MSB  | High byte of `temperature_c * 100` (signed) |
| 1    | Temperature LSB  | Low byte of `temperature_c * 100`           |
| 2-7  | Reserved         | Always `0x00`                               |

---

## 0x103 — Battery Weight

| Byte | Field         | Description                              |
|------|---------------|--------------------------------------------|
| 0    | Weight MSB    | High byte of `weight_kg * 1000` (signed)  |
| 1    | Weight LSB    | Low byte of `weight_kg * 1000`            |
| 2-7  | Reserved      | Always `0x00`                             |

Weight is measured by the HX711 + load cell assembly the battery pack rests
on, allowing correlation between mass changes and coolant/electrolyte or
physical pack integrity over time.

---

## 0x104 — Battery Power

| Byte | Field       | Description                                     |
|------|-------------|---------------------------------------------------|
| 0    | Power MSB   | High byte of `power_w * 100` (signed)             |
| 1    | Power LSB   | Low byte of `power_w * 100`                       |
| 2-7  | Reserved    | Always `0x00`                                     |

Computed on the ESP32 as `power_w = voltage_v * current_a` from the
*filtered* voltage/current values, then re-transmitted so downstream
consumers (Node-RED) don't need to recompute it.

---

## 0x105 — System Status

| Byte | Field          | Description                                                        |
|------|----------------|----------------------------------------------------------------------|
| 0    | Battery Status | `0`=GOOD, `1`=WARNING, `2`=CRITICAL, `3`=FAULT                       |
| 1    | CAN Error Flag | `0`=no error, `1`=last transmit failed / bus error                   |
| 2    | Sensors OK     | `0`=one or more sensors failed to read, `1`=all sensors read OK      |
| 3    | Alert Bitmask  | bit0=Overvoltage, bit1=Undervoltage, bit2=Overcurrent, bit3=Overtemp |
| 4-7  | Reserved       | Always `0x00`                                                        |

### Battery Status Enum

| Value | Meaning  | Trigger Condition (see `config.h`)                                   |
|-------|----------|-------------------------------------------------------------------------|
| 0     | GOOD     | All parameters within normal operating range                            |
| 1     | WARNING  | Voltage/current/temperature crossed a warning threshold                 |
| 2     | CRITICAL | Voltage/current/temperature crossed a critical threshold                |
| 3     | FAULT    | One or more sensors failed to produce a reading this cycle              |

### Alert Bitmask

| Bit | Flag           | Set When                                            |
|-----|----------------|-------------------------------------------------------|
| 0   | Overvoltage    | `voltage_v >= BATTERY_VOLTAGE_MAX_WARN`               |
| 1   | Undervoltage   | `voltage_v <= BATTERY_VOLTAGE_MIN_WARN`               |
| 2   | Overcurrent    | `current_a >= BATTERY_CURRENT_WARN`                   |
| 3   | Overtemperature| `temperature_c >= BATTERY_TEMP_WARN`                  |

---

## Transmission Timing

All six frames are transmitted back-to-back every **100 ms** by the
`task_can_transmit` scheduler task (see `firmware/BatteryMonitoringSystem/scheduler.cpp`
and `BatteryMonitoringSystem.ino`). At 500 kbps, six 8-byte standard frames
take well under 3 ms of bus time, leaving ample headroom for future
expansion (e.g. cell-level balancing telemetry, BMS command frames).

## Default Thresholds (from `config.h`)

| Parameter          | Warning Low | Warning High | Critical Low | Critical High |
|---------------------|-------------|---------------|----------------|-----------------|
| Voltage (V)         | 10.5        | 12.6          | 9.6            | 13.0            |
| Current (A)         | —           | 5.0           | —              | 8.0             |
| Temperature (°C)    | —           | 45.0          | —              | 60.0            |

These are firmware-configurable constants intended as a starting point for a
typical 3S Li-ion pack and **must** be adjusted to match the actual battery
chemistry and pack configuration used in a real deployment.
