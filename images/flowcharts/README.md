# Flowcharts

This directory contains the flowcharts used to explain the execution flow of the **Automatic Battery Monitoring System using CAN Bus and Node-RED**.

The flowcharts describe the firmware logic, CAN communication, Node-RED processing, and overall system operation.

---

# Contents

| File | Description |
|------|-------------|
| `system_flowchart.png` | Overall execution flow of the complete battery monitoring system. |
| `firmware_flowchart.png` | Embedded firmware execution sequence running on the ESP32. |
| `can_transmission_flowchart.png` | CAN Bus message creation, transmission, and reception workflow. |
| `node_red_flowchart.png` | Node-RED data processing and dashboard update flow. |
| `state_machine.png` | High-level firmware state machine showing system operation. |

---

# Purpose

These flowcharts help explain:

- System startup sequence
- Sensor data acquisition
- Data validation
- CAN Bus communication
- Node-RED processing
- Dashboard updates
- Error handling
- Continuous monitoring loop

---

# Flowchart Overview

The overall workflow is:

```text
System Power ON
        │
        ▼
Initialize ESP32
        │
        ▼
Initialize Sensors
        │
        ▼
Read Battery Parameters
        │
        ▼
Process & Validate Data
        │
        ▼
Transmit via CAN Bus
        │
        ▼
Receive in Node-RED
        │
        ▼
Update Dashboard
        │
        ▼
Store Logs
        │
        ▼
Repeat Monitoring Loop
```

---

# Design Guidelines

All flowcharts follow these principles:

- Professional engineering style
- Standard flowchart symbols
- White background
- Easy-to-read layout
- GitHub-friendly resolution
- Consistent terminology

---

# Notes

These flowcharts represent the intended firmware and software workflow based on the implemented project architecture.

Actual runtime behavior may vary depending on hardware configuration and sensor inputs.

---

# License

All flowchart diagrams are distributed under the same license as this repository.