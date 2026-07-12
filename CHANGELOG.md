# Changelog

All notable changes to this project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2026-07-11

### Added
- Initial release of the Automatic Battery Monitoring System firmware
  (ESP32, Arduino framework).
- Modular sensor drivers: INA219 (voltage/current), DS18B20 (temperature),
  HX711 (weight).
- MCP2515 CAN bus driver wrapper and 500 kbps configuration.
- Moving-average filtering for all sensor channels.
- Battery status evaluation (GOOD / WARNING / CRITICAL / FAULT) with
  configurable thresholds.
- CAN protocol definition and packing for frames `0x100`-`0x105`.
- Cooperative, non-blocking task scheduler with watchdog integration.
- UART debug console with periodic telemetry snapshots.
- Node-RED flow (`node-red/flow.json`) with gauges, trend charts, status
  indicators, and threshold-based toast alerts.
- Full project documentation set (`docs/`), hardware pin configuration
  (`hardware/`), and repository community files (`.github/`).

[1.0.0]: https://github.com/your-org/Automatic-Battery-Monitoring-System-Using-CAN-Bus-and-Node-RED/releases/tag/v1.0.0
