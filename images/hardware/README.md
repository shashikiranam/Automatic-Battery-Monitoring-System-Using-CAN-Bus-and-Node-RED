# Hardware

This directory contains the hardware documentation for the **Automatic Battery Monitoring System using CAN Bus and Node-RED**.

The diagrams in this folder illustrate the physical hardware design, wiring connections, sensor placement, and ESP32 pin mapping used in the project.

---

# Contents

| File | Description |
|------|-------------|
| `hardware_block_diagram.png` | High-level hardware architecture showing all connected modules. |
| `circuit_diagram.png` | Complete hardware circuit and wiring diagram. |
| `sensor_layout.png` | Physical arrangement of sensors and communication modules. |
| `pin_connection_diagram.png` | ESP32 GPIO pin assignments and peripheral connections. |

---

# Hardware Components

The project is built using the following hardware:

- ESP32 DevKit V1
- INA219 Voltage & Current Sensor
- DS18B20 Temperature Sensor
- HX711 Load Cell Amplifier
- Load Cell
- MCP2515 CAN Controller
- TJA1050 CAN Transceiver
- USB Power Supply
- Node-RED Dashboard (Host PC)

---

# Communication Interfaces

| Interface | Purpose |
|-----------|---------|
| I²C | INA219 Communication |
| 1-Wire | DS18B20 Temperature Sensor |
| SPI | MCP2515 CAN Controller |
| CAN Bus | Data Transmission |
| Wi-Fi | Node-RED Dashboard |
| UART | Debugging & Logging |

---

# Design Objectives

The hardware architecture has been designed to provide:

- Modular embedded hardware design
- Reliable sensor data acquisition
- CAN Bus communication
- Easy hardware expansion
- Simple debugging
- Real-time monitoring support

---

# Hardware Workflow

```text
Battery Pack
      │
      ▼
Voltage / Current Sensor (INA219)
Temperature Sensor (DS18B20)
Load Cell (HX711)
      │
      ▼
ESP32 DevKit V1
      │
      ▼
MCP2515 CAN Controller
      │
      ▼
TJA1050 CAN Transceiver
      │
      ▼
CAN Bus
      │
      ▼
Node-RED Dashboard
```

---

# Notes

The hardware diagrams included in this folder represent the reference implementation used for this project.

Always verify wiring, supply voltage, and GPIO assignments before deploying the firmware on physical hardware.

---

# License

All hardware documentation and diagrams are distributed under the same license as this repository.