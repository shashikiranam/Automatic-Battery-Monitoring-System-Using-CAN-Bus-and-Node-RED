# Build Instructions

## 1. Prerequisites

- [Arduino IDE](https://www.arduino.cc/en/software) 2.x (or 1.8.19+)
- ESP32 board support package installed via Boards Manager
  (`https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`)
- USB driver for your ESP32 board's UART bridge (CP2102 / CH340, depending
  on the specific DevKit V1 variant)

## 2. Install Required Libraries

Open **Sketch → Include Library → Manage Libraries…** and install:

| Library                | Author         |
|--------------------------|------------------|
| Adafruit INA219           | Adafruit         |
| OneWire                   | Paul Stoffregen  |
| DallasTemperature         | Miles Burton et al. |
| HX711                     | bogde            |
| mcp_can                   | coryjfowler      |

## 3. Open the Sketch

Open `firmware/BatteryMonitoringSystem/BatteryMonitoringSystem.ino` in the
Arduino IDE. All companion `.h`/`.cpp` files in the same folder are compiled
automatically as part of the sketch.

## 4. Board Configuration

**Tools** menu settings:

| Setting            | Value                          |
|---------------------|----------------------------------|
| Board                | ESP32 Dev Module                 |
| Upload Speed         | 921600 (or 115200 if unstable)   |
| Flash Frequency      | 80MHz                            |
| Flash Mode           | QIO                              |
| Partition Scheme     | Default 4MB with spiffs          |
| Core Debug Level     | None (or Verbose while debugging)|
| Port                 | Select the enumerated COM/tty port |

## 5. Wiring

Wire the sensors and MCP2515 module per
[`../hardware/PIN_CONFIGURATION.md`](../hardware/PIN_CONFIGURATION.md)
before powering on. Double-check the MCP2515 module's logic level (most
common breakout boards are 5V-tolerant on SPI but confirm against your
specific module's datasheet).

## 6. Calibrate the Load Cell

The default `HX711_CALIBRATION_FACTOR` in `config.h` is a **placeholder**.
To calibrate:

1. Flash the firmware with an approximate factor (e.g. the default).
2. With no load on the cell, call `hx711_driver_tare()` (already invoked
   automatically at startup).
3. Place a known reference mass on the load cell and observe the raw
   reading via UART debug.
4. Compute `new_factor = current_factor * (raw_reading / known_mass_kg)`
   and update `HX711_CALIBRATION_FACTOR` in `config.h`.
5. Re-flash and verify readings match the reference mass within tolerance.

## 7. Compile & Upload

Click **Verify** to compile, then **Upload**. Hold the ESP32's **BOOT**
button if your board requires manual entry into download mode.

## 8. Monitor

Open **Tools → Serial Monitor** at **115200 baud**. You should see sensor
initialization messages followed by periodic `[TELEMETRY]` lines every
500 ms.

## 9. Verify CAN Output

Connect the MCP2515 module's CAN-H/CAN-L to your CAN bus (with proper
120Ω termination at both bus ends) and, on a Linux host with a USB-CAN
adapter:

```bash
sudo ip link set can0 type can bitrate 500000
sudo ip link set up can0
candump can0
```

You should see frames with IDs `100`, `101`, `102`, `103`, `104`, `105`
(hex) repeating roughly every 100 ms.

## 10. Bring up the Node-RED Dashboard

See [`../node-red/README.md`](../node-red/README.md) for importing
`flow.json` and installing the required Node-RED palette nodes.

## 11. Troubleshooting

| Symptom                                   | Likely Cause                                             |
|---------------------------------------------|--------------------------------------------------------------|
| `[MCP2515] ERROR: CAN controller initialization failed` | Wrong CS pin, SPI wiring, or crystal frequency mismatch (`CAN_CLOCK_MHZ` in `config.h`) |
| `[INA219] ERROR: Sensor not detected`     | I²C wiring/address mismatch, missing pull-ups                |
| `[DS18B20] ERROR: No 1-Wire sensors found`| Missing 4.7kΩ pull-up resistor on the data line               |
| `[HX711] ERROR: Amplifier not responding` | DOUT/SCK pins swapped, or amplifier not powered               |
| Node-RED shows no data                    | SocketCAN interface not up, or USB-CAN adapter not detected  |
