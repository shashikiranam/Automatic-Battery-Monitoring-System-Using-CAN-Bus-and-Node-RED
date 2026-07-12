# Simulations

This folder is reserved for circuit simulation files (e.g. Proteus, KiCad,
or Wokwi projects) modeling the sensor front-end and CAN interface prior to
physical assembly.

## Suggested Contents

- `wokwi/` — Wokwi ESP32 simulation project (`diagram.json`, `wokwi.toml`)
  for validating firmware logic without physical hardware. Note: Wokwi's
  simulated peripherals do not currently include MCP2515/CAN, INA219,
  DS18B20, or HX711 out of the box; a simulation would need to mock these
  via custom chips or stub the drivers for logic-level testing.
- `proteus/` — Proteus ISIS schematic and PCB layout files, if used for
  hardware-in-the-loop style simulation.

No simulation project files are included in this initial revision — this
folder is a placeholder for the team's simulation artifacts as they are
developed. Populate it as simulation work is completed; do not commit
fabricated simulation results or screenshots.
