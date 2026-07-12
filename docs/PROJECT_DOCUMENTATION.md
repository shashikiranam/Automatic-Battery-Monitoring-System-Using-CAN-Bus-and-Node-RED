# Project Documentation

## Automatic Battery Monitoring System using CAN Bus and Node-RED

---

## 1. Project Overview

The Automatic Battery Monitoring System is an embedded, real-time telemetry
platform for lead-acid or lithium battery packs. An ESP32 microcontroller
samples voltage, current, temperature, and weight from a dedicated sensor
suite, evaluates the pack's health status, and broadcasts the results as CAN
bus frames at 500 kbps. A Node-RED application running on a gateway/host
device subscribes to the CAN bus and renders the data on a live dashboard
with gauges, trend charts, and alerting.

## 2. Problem Statement

Battery packs used in EVs, solar storage, robotics, and backup power systems
degrade or fail silently if their electrical and thermal parameters are not
continuously monitored. Manual, periodic inspection cannot catch transient
over-current events, thermal runaway precursors, or gradual capacity loss
(reflected as weight/electrolyte changes in some chemistries). A low-cost,
CAN-based monitoring node that any vehicle/industrial CAN network can
consume — paired with an accessible dashboard — closes this gap.

## 3. Objectives

- Continuously measure battery **voltage**, **current**, **temperature**,
  and **weight**.
- Compute derived **power** and an overall **battery status** classification
  (GOOD / WARNING / CRITICAL / FAULT).
- Broadcast all telemetry over a standard, industry-relevant bus (**CAN**)
  using a well-documented, compact frame format.
- Visualize telemetry in real time via a **Node-RED** dashboard with
  gauges, historical trend charts, and threshold-based alerts.
- Provide firmware that is modular, watchdog-friendly, and suitable as a
  foundation for a production Battery Management System (BMS) front end.

## 4. Features

- Modular embedded C/C++ firmware (Arduino framework, ESP32).
- Cooperative, non-blocking task scheduler (no `delay()` in the hot path).
- Moving-average filtering on all analog sensor channels.
- Configurable warning/critical thresholds per parameter.
- Six well-documented CAN frames (`0x100`–`0x105`).
- UART debug console with periodic human-readable telemetry snapshots.
- Node-RED flow: CAN decode → dashboard (gauges, charts, table, alerts).
- Hardware watchdog integration (`esp_task_wdt`) for field robustness.

## 5. Applications

- Electric vehicle / e-bike battery pack monitoring.
- Solar energy storage system (ESS) health monitoring.
- Mobile robotics power subsystems.
- UPS / backup battery banks in telecom or industrial cabinets.
- Educational / research platform for CAN bus and BMS development.

## 6. Working Principle

1. The ESP32 samples the **INA219** (I²C) for bus voltage and current, the
   **DS18B20** (1-Wire) for temperature, and the **HX711** + load cell for
   weight, every 100 ms.
2. Each raw sample is passed through a per-channel **moving-average filter**
   to suppress sensor noise before it influences status decisions or is
   transmitted.
3. Power is computed as `voltage × current`, and an overall **battery
   status** is derived by comparing filtered values against configurable
   warning/critical thresholds.
4. The filtered telemetry and status are packed into six CAN frames and
   transmitted via the **MCP2515** controller (SPI) at 500 kbps, on a
   100 ms cycle.
5. A Node-RED flow running on a CAN-capable gateway (SocketCAN) decodes each
   frame per `docs/CAN_FRAME_DOCUMENTATION.md`, updates dashboard gauges and
   trend charts, and raises toast alerts when thresholds are breached.

## 7. Hardware Used

| Component              | Role                                   |
|-------------------------|------------------------------------------|
| ESP32 DevKit V1          | Main microcontroller                     |
| MCP2515 + TJA1050 module | CAN bus controller/transceiver (SPI)     |
| INA219                   | Bus voltage & current sensor (I²C)       |
| DS18B20                  | Digital temperature sensor (1-Wire)      |
| HX711 + Load Cell        | Weight sensing amplifier                 |
| USB-CAN Adapter          | Bridges physical CAN bus to Node-RED host|

See `hardware/PIN_CONFIGURATION.md` for the full wiring table.

## 8. Software Used

| Tool / Library                | Purpose                                |
|---------------------------------|-------------------------------------------|
| Arduino IDE (≥ 2.x)             | Firmware build environment                |
| Adafruit INA219 library         | INA219 driver                             |
| OneWire + DallasTemperature     | DS18B20 driver                            |
| HX711 (bogde)                   | Load cell amplifier driver                |
| mcp_can (coryjfowler)           | MCP2515 CAN controller driver             |
| Node-RED + node-red-dashboard   | Telemetry visualization                   |
| node-red-contrib-socketcan      | CAN bus ingestion into Node-RED           |

## 9. Programming Language

Embedded C / C++ (Arduino framework), targeting the ESP32 (Xtensa LX6).

## 10. Communication Protocols

- **SPI** — ESP32 ↔ MCP2515 CAN controller.
- **I²C** — ESP32 ↔ INA219.
- **1-Wire** — ESP32 ↔ DS18B20.
- **CAN 2.0B (500 kbps, standard 11-bit ID)** — MCP2515 ↔ CAN bus ↔ Node-RED
  gateway.
- **UART (115200 baud)** — ESP32 ↔ USB debug console.

## 11. Firmware Architecture

```
firmware/BatteryMonitoringSystem/
├── BatteryMonitoringSystem.ino   Entry point: setup(), loop(), tasks
├── config.h                      Pins, timing, thresholds, feature flags
├── scheduler.h / .cpp             Cooperative periodic task scheduler
├── filter.h / .cpp                Moving-average filter
├── ina219.h / .cpp                INA219 driver wrapper
├── ds18b20.h / .cpp               DS18B20 driver wrapper
├── hx711.h / .cpp                 HX711 driver wrapper
├── mcp2515.h / .cpp               MCP2515 CAN driver wrapper
├── battery.h / .cpp               Sensor aggregation & status evaluation
├── can_protocol.h / .cpp          CAN frame packing (0x100-0x105)
├── uart_debug.h / .cpp            Centralized debug logging
└── utilities.h / .cpp             Byte packing, clamping, timing helpers
```

Each module has a single responsibility and communicates through explicit
function calls and small, well-defined structs (`battery_state_t`,
`ina219_reading_t`, etc.) — no global mutable state is shared outside of
each module's own translation unit.

## 12. Software Architecture

The firmware follows a **layered** architecture:

1. **Hardware Abstraction Layer** — `ina219`, `ds18b20`, `hx711`, `mcp2515`
   modules wrap third-party libraries behind project-specific interfaces.
2. **Application Layer** — `battery` module aggregates sensor readings,
   applies filtering, and evaluates status.
3. **Communication Layer** — `can_protocol` module packs application data
   into the wire format and hands frames to the `mcp2515` driver.
4. **Orchestration Layer** — `scheduler` + `BatteryMonitoringSystem.ino`
   tie everything together via periodic, non-blocking tasks.

## 13. System Architecture

```
 ┌─────────────┐   I2C    ┌──────────┐
 │   INA219    │◄────────►│          │
 └─────────────┘          │          │      SPI      ┌───────────┐   CAN Bus (500kbps)   ┌──────────────┐
 ┌─────────────┐  1-Wire  │  ESP32   │◄─────────────►│  MCP2515  │◄─────────────────────►│  Node-RED    │
 │  DS18B20    │◄────────►│ DevKit V1│                └───────────┘   USB-CAN Adapter     │  Dashboard   │
 └─────────────┘          │          │                                                     └──────────────┘
 ┌─────────────┐  Digital │          │
 │ HX711+Cell  │◄────────►│          │
 └─────────────┘          └────┬─────┘
                                │ UART
                                ▼
                          Debug Console
```

## 14. Testing Procedure

1. **Bench power-up**: Flash firmware, open Serial Monitor at 115200 baud,
   confirm each sensor reports "initialized successfully".
2. **Sensor sanity check**: Apply a known voltage/load; compare UART debug
   telemetry against a reference multimeter/scale.
3. **CAN bus verification**: Use `candump can0` (or a CAN analyzer) to
   confirm frames `0x100`–`0x105` appear every ~100 ms with correct DLC (8).
4. **Node-RED decode check**: Import `node-red/flow.json`, confirm gauges
   and charts update and match the values seen in `candump`/Serial Monitor.
5. **Threshold/alert test**: Temporarily lower a threshold in `config.h`
   (e.g. `BATTERY_TEMP_WARN`) to safely trigger a WARNING/CRITICAL state and
   confirm the dashboard alert toast and status LED respond correctly.
6. **Fault handling test**: Disconnect one sensor; confirm `battery_status`
   reports `FAULT` and `sensorsOk` becomes `0` on the dashboard.

> Note: This repository does not include fabricated oscilloscope captures,
> serial logs, or CAN dumps from a physical test run. Diagrams and any
> screenshots included are clearly labeled as **concept/reference
> illustrations** — see `images/README.md`.

## 15. Expected Results

- Six CAN frames transmitted every 100 ms with correctly scaled telemetry.
- Node-RED dashboard gauges tracking sensor values within the filter's
  settling time (a few sample periods after a step change).
- Battery status transitioning GOOD → WARNING → CRITICAL as configured
  thresholds are crossed, reflected on both UART debug output and the
  Node-RED status widgets.

## 16. Advantages

- Standard CAN bus output integrates naturally with vehicle/industrial
  networks and existing CAN tooling.
- Modular firmware is straightforward to extend (e.g. additional cells,
  BMS balancing commands).
- Node-RED dashboard requires no custom front-end development and is easy
  to re-theme or extend with additional flows (MQTT, cloud logging, etc.).

## 17. Limitations

- Single load-cell weight sensing is a proxy signal, not a substitute for
  proper per-cell voltage monitoring in a full BMS.
- The reference thresholds in `config.h` are for a generic 3S Li-ion pack
  and must be re-tuned for the actual battery chemistry in use.
- MCP2515 restricts throughput versus a native ESP32 TWAI/CAN peripheral;
  suitable for this telemetry rate, but not ideal for very high frame-rate
  BMS balancing traffic.
- No persistent (flash/SD) data logging is implemented in this revision;
  historical data lives only in the Node-RED chart buffers unless a
  logging flow (e.g. to InfluxDB) is added.

## 18. Future Improvements

- Add per-cell voltage monitoring for multi-cell packs.
- Add CAN bus data logging to a time-series database from Node-RED.
- Add OTA firmware updates.
- Add State-of-Charge (SoC) and State-of-Health (SoH) estimation.
- Migrate to the ESP32's native TWAI controller for lower latency at scale.
- Add authentication and TLS to the Node-RED dashboard for field deployments.

## 19. Author

Firmware, CAN protocol design, and Node-RED dashboard authored as part of
this repository's embedded systems reference implementation.

## 20. License

Released under the MIT License — see [`../LICENSE`](../LICENSE).
