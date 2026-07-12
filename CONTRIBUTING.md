# Contributing

Thanks for your interest in improving the Automatic Battery Monitoring
System! Contributions of all sizes are welcome — bug fixes, documentation
improvements, new sensor drivers, Node-RED flow enhancements, and more.

## Getting Started

1. Fork the repository and create a feature branch from `main`:
   ```bash
   git checkout -b feature/short-description
   ```
2. Follow [`docs/BUILD_INSTRUCTIONS.md`](docs/BUILD_INSTRUCTIONS.md) to set
   up the firmware build environment.
3. Follow [`node-red/README.md`](node-red/README.md) to set up the
   Node-RED dashboard for testing flow changes.

## Code Style — Firmware (C/C++)

- One module = one responsibility. New hardware drivers go in their own
  `<name>.h` / `<name>.cpp` pair under `firmware/BatteryMonitoringSystem/`.
- Use the existing naming convention: `snake_case` for functions and
  variables, `SCREAMING_SNAKE_CASE` for macros/constants, `_t` suffix for
  typedef'd structs.
- Document every public function with a Doxygen-style `/** ... */` comment
  in the header file.
- Avoid `delay()` in any code reachable from `loop()` — use the
  `scheduler` module for periodic behavior instead.
- Keep hardware register/pin literals in `config.h`, not scattered through
  driver source files.

## Code Style — Node-RED

- Keep decode logic for each CAN ID in its own `function` node (see
  `docs/NODE_RED_ARCHITECTURE.md` for rationale).
- Prefer `flow` context over `global` context for state that's specific to
  this dashboard.
- Test flow changes by re-importing `flow.json` into a clean Node-RED
  instance before submitting a PR.

## Commit Messages

Use short, descriptive, imperative-mood messages, e.g.:

```
Add SoC estimation to battery module
Fix HX711 tare timing on cold boot
Document CAN bus termination requirements
```

## Submitting a Pull Request

1. Ensure the firmware compiles cleanly in the Arduino IDE for the
   `ESP32 Dev Module` board target.
2. Update relevant documentation (`docs/`, `README.md`,
   `CHANGELOG.md`) alongside code changes.
3. Describe what you tested (bench setup, sensors used, CAN
   verification method) in the PR description.
4. Link any related issues.

## Reporting Issues

Please use the issue templates in `.github/ISSUE_TEMPLATE/` for bug reports
and feature requests, and include firmware version, board revision, and
relevant Serial Monitor / `candump` output where applicable.

## Code of Conduct

By participating in this project you agree to abide by our
[Code of Conduct](CODE_OF_CONDUCT.md).
