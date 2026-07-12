# Architecture

This directory contains the architecture diagrams for the **Automatic Battery Monitoring System using CAN Bus and Node-RED**.

These diagrams document the system from different engineering perspectives, including hardware architecture, firmware architecture, software architecture, CAN communication, Node-RED integration, and overall data flow.

---

# Contents

| File | Description |
|------|-------------|
| `system_architecture.png` | Overall system architecture showing ESP32, sensors, CAN Bus, and Node-RED. |
| `software_architecture.png` | Software layer organization and module interactions. |
| `firmware_architecture.png` | Embedded firmware module hierarchy. |
| `can_network_architecture.png` | CAN Bus communication architecture. |
| `node_red_architecture.png` | Node-RED data flow and dashboard architecture. |
| `data_flow.png` | End-to-end data movement from sensors to visualization. |

---

# Purpose

The architecture diagrams are intended to help readers understand:

- Overall system organization
- Embedded firmware structure
- Software module interactions
- Hardware connectivity
- CAN Bus communication
- Node-RED dashboard integration
- Data acquisition and processing flow

---

# Technologies

- ESP32 DevKit V1
- MCP2515 CAN Controller
- INA219 Voltage & Current Sensor
- DS18B20 Temperature Sensor
- HX711 Load Cell Amplifier
- CAN Bus (SPI Interface)
- UART Debug
- Node-RED Dashboard

---

# Diagram Guidelines

All diagrams follow these principles:

- Professional engineering style
- White background
- High-resolution PNG format
- GitHub-friendly layout
- Consistent naming convention
- Clear signal and data flow

---

# Notes

These diagrams describe the intended design and architecture of the system.

Any dashboard views or runtime illustrations are **concept illustrations** unless explicitly identified as captured from a real hardware implementation.

---

# License

All architecture diagrams are distributed under the same license as this repository.