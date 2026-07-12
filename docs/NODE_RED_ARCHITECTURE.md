# Node-RED Architecture

## Data Flow

```
ESP32 (CAN TX @ 100ms) → CAN Bus (500 kbps) → USB-CAN Adapter → SocketCAN (can0)
        → socketcan-in node → switch (route by CAN ID) → function (decode) → dashboard widgets
```

## Node Graph Summary

| Stage                | Node Type       | Purpose                                             |
|------------------------|-------------------|--------------------------------------------------------|
| Ingestion               | `socketcan-in`     | Reads raw CAN frames from the `can0` interface          |
| Routing                 | `switch`           | Routes each frame to its dedicated decoder by `msg.id`  |
| Decoding                | `function` (×6)    | Converts raw bytes into engineering-unit values          |
| State Merge             | `function`         | Maintains a merged snapshot of the latest value per signal |
| Alerting                | `function`         | Builds a toast notification when thresholds are breached |
| Visualization           | `ui_gauge`, `ui_chart`, `ui_text`, `ui_led`, `ui_toast`, `ui_template` | Renders the dashboard |

## Why a `switch` + per-ID `function` design?

Each CAN frame has a distinct byte layout (see
`docs/CAN_FRAME_DOCUMENTATION.md`), so a single generic decoder would need
branching logic anyway. Splitting decoding into one `function` node per CAN
ID keeps each decoder small, testable in isolation, and easy to extend
(e.g. adding a new signal only requires one new `switch` output and one new
`function` node, with no changes to existing decoders).

## State Snapshot Pattern

Because dashboard widgets like the telemetry table need to display multiple
signals at once, but CAN frames arrive as separate messages, the
`fn_merge_state` node maintains a `flow` context object keyed by topic
(`battery/voltage`, `battery/current`, etc.), updating one field at a time
and re-emitting the full snapshot on every update. This avoids the need for
a `join` node with strict timing assumptions, and stays correct even if a
frame is occasionally dropped.

## Extending the Flow

- **Persistent logging**: Tap off `fn_merge_state`'s output into an
  `influxdb out` or `mongodb out` node for historical storage.
- **Remote alerting**: Tap off `fn_alert_notification` into an `e-mail` or
  `mqtt out` node in addition to (or instead of) `ui_toast`.
- **Multi-pack support**: If multiple battery packs share a bus, extend the
  CAN ID scheme (e.g. `0x100 + pack_index * 0x10`) and parameterize the
  `switch`/decoder nodes accordingly.

## Security Note

`node-red-contrib-socketcan` requires the Node-RED process to have access
to the host's CAN network namespace. In a field deployment, run Node-RED
under a dedicated, least-privileged user and restrict dashboard access
(reverse proxy + authentication) before exposing port 1880 beyond a trusted
LAN.
