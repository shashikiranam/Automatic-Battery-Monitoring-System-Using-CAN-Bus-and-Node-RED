# Dashboard

This directory contains the dashboard screenshots and user interface concepts for the **Automatic Battery Monitoring System using CAN Bus and Node-RED**.

The dashboard provides real-time visualization of battery parameters received from the ESP32 through the CAN Bus network.

---

# Contents

| File | Description |
|------|-------------|
| `node_red_dashboard.png` | Main Node-RED dashboard showing real-time battery monitoring. |
| `battery_dashboard.png` | Detailed battery parameter dashboard with gauges and charts. |
| `mobile_dashboard.png` | Mobile-friendly dashboard layout for remote monitoring. |

---

# Dashboard Features

The dashboard displays:

- Battery Voltage
- Battery Current
- Battery Temperature
- State of Charge (SoC)
- Battery Weight / Load Cell Data
- CAN Communication Status
- System Health
- Connection Status
- Timestamp
- Alert Notifications

---

# Dashboard Workflow

```text
ESP32
   │
   ▼
Read Sensors
   │
   ▼
Create CAN Message
   │
   ▼
Transmit via CAN Bus
   │
   ▼
Node-RED
   │
   ▼
Process Data
   │
   ▼
Update Dashboard
   │
   ▼
Display Live Values
```

---

# Dashboard Components

- Analog Gauges
- Digital Value Cards
- Line Charts
- Status Indicators
- Alert Panel
- Communication Status
- Historical Trends
- Timestamp Display

---

# Technologies

- Node-RED Dashboard
- MQTT
- CAN Bus
- ESP32 DevKit V1
- JavaScript Function Nodes
- SQLite (Optional)
- CSV Logging

---

# Design Guidelines

The dashboard has been designed to provide:

- Clean industrial interface
- White theme
- Real-time updates
- Easy readability
- Responsive layout
- Automotive-inspired monitoring

---

# Notes

Dashboard images represent the intended user interface of the project.

Concept images are created for documentation purposes unless explicitly identified as screenshots captured from the running Node-RED application.

---

# License

Dashboard images and documentation are distributed under the same license as this repository.