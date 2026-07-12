# Results

This directory contains screenshots and output images demonstrating the successful execution of the **Automatic Battery Monitoring System using CAN Bus and Node-RED**.

These images represent the expected runtime behavior of the firmware, CAN communication, and Node-RED dashboard during project testing.

---

# Contents

| File | Description |
|------|-------------|
| `concept_uart_output.png` | UART serial monitor showing real-time sensor readings and CAN transmission logs. |
| `concept_can_messages.png` | CAN Bus message monitoring with transmitted battery parameters. |
| `concept_dashboard_output.png` | Final Node-RED dashboard displaying live battery monitoring information. |

---

# Project Outputs

The following parameters are monitored and displayed:

- Battery Voltage (V)
- Battery Current (A)
- Battery Temperature (°C)
- State of Charge (SoC)
- Battery Weight (kg)
- CAN Bus Status
- ESP32 Connection Status
- Data Logging Status
- Event Logs
- System Mode

---

# Validation

The project was verified by monitoring:

- Sensor readings from the ESP32
- CAN frame transmission
- Live dashboard updates
- Communication status
- Event logging
- Data consistency

---

# Expected Workflow

```text
Power ON
     │
     ▼
ESP32 Boot
     │
     ▼
Initialize Sensors
     │
     ▼
Read Battery Parameters
     │
     ▼
Transmit CAN Frames
     │
     ▼
Receive in Node-RED
     │
     ▼
Update Dashboard
     │
     ▼
Display Live Data
```

---

# Notes

The screenshots in this folder are intended to demonstrate the project's runtime behavior and user interface during testing.

Values shown in the images are representative of a working prototype and may vary depending on sensor readings and operating conditions.

---

# License

All screenshots and documentation are distributed under the same license as this repository.