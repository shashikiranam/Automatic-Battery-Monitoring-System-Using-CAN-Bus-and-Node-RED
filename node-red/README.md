# Node-RED Dashboard — Battery Monitoring System

This folder contains the Node-RED flow that ingests CAN bus telemetry from the
ESP32 firmware and renders a live dashboard.

## Contents

| File          | Description                                              |
|---------------|-----------------------------------------------------------|
| `flow.json`   | Importable Node-RED flow (CAN decode + dashboard widgets) |
| `README.md`   | This file                                                  |

## Architecture

```
[ MCP2515 / ESP32 ]--CAN Bus (500 kbps)--> [ USB-CAN Adapter ]
                                                    |
                                             SocketCAN (can0)
                                                    |
                                              [ Node-RED ]
                                          decode -> dashboard
```

The flow assumes a Linux host (e.g. Raspberry Pi, or a PC running
Ubuntu with a USB-CAN adapter such as a CANable or PCAN-USB) exposing the
physical CAN bus as a SocketCAN interface named `can0`.

If you don't have SocketCAN hardware available, the `socketcan-in` node can
be swapped for `node-red-contrib-serialport` reading frames from an
Arduino/ESP32-based USB-CAN gateway sketch, or for an MQTT bridge — the
downstream decode/dashboard logic is unchanged since it only depends on
`msg.id` and `msg.payload` (byte array).

## Required Node-RED Nodes

Install via the Node-RED "Manage palette" menu, or with npm inside your
`~/.node-red` directory:

```bash
npm install node-red-dashboard
npm install node-red-contrib-socketcan
```

## Bringing up the SocketCAN interface (Linux)

```bash
sudo ip link set can0 type can bitrate 500000
sudo ip link set up can0
candump can0   # optional: sanity-check that frames are arriving
```

## Importing the Flow

1. Open the Node-RED editor (default: `http://<host>:1880`).
2. Menu (☰) → **Import** → select `flow.json`.
3. Deploy.
4. Open the dashboard at `http://<host>:1880/ui`.

## Dashboard Layout

- **Overview** — Voltage / Current / Temperature / Weight / Power gauges,
  plus a live telemetry snapshot table.
- **Trends** — Rolling line charts (last 10 minutes) for Voltage, Current,
  Temperature, and Power.
- **System Status & Alerts** — Battery status text (GOOD / WARNING /
  CRITICAL / FAULT), CAN bus health LED, and toast notifications when an
  alert condition is active.

## CAN Frame Reference

See [`../docs/CAN_FRAME_DOCUMENTATION.md`](../docs/CAN_FRAME_DOCUMENTATION.md)
for the full byte-level specification that this flow decodes against.
