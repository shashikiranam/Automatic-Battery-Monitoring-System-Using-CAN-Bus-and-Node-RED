# Hardware — Pin Configuration

Board: **ESP32 DevKit V1** (30-pin variant)

## MCP2515 CAN Module (SPI)

| MCP2515 Pin | ESP32 Pin | Notes                        |
|-------------|-----------|-------------------------------|
| VCC         | 5V        | Most breakout boards need 5V for the TJA1050 transceiver |
| GND         | GND       |                                |
| CS          | GPIO 5    | `PIN_MCP2515_CS`               |
| SO (MISO)   | GPIO 19   | `PIN_SPI_MISO`                 |
| SI (MOSI)   | GPIO 23   | `PIN_SPI_MOSI`                 |
| SCK         | GPIO 18   | `PIN_SPI_SCK`                  |
| INT         | GPIO 4    | `PIN_MCP2515_INT`               |
| CAN-H/CAN-L | —         | To physical CAN bus, 120Ω termination at both bus ends |

## INA219 Current/Voltage Sensor (I²C)

| INA219 Pin | ESP32 Pin | Notes                    |
|------------|-----------|----------------------------|
| VCC        | 3.3V      |                             |
| GND        | GND       |                             |
| SDA        | GPIO 21   | `PIN_I2C_SDA`                |
| SCL        | GPIO 22   | `PIN_I2C_SCL`                |
| VIN+       | Battery + terminal (via shunt) | High side of current shunt |
| VIN-       | Load + terminal                | Low side of current shunt |

Default I²C address: `0x40` (`INA219_I2C_ADDRESS`). Adjust A0/A1 solder
jumpers on the module and update `config.h` if multiple INA219 units share
the bus.

## DS18B20 Temperature Sensor (1-Wire)

| DS18B20 Pin | ESP32 Pin | Notes                              |
|-------------|-----------|--------------------------------------|
| VDD         | 3.3V      |                                       |
| GND         | GND       |                                       |
| DQ          | GPIO 15   | `PIN_ONEWIRE_BUS`, requires 4.7kΩ pull-up to 3.3V |

Mount the sensor in thermal contact with the battery pack casing (not
floating in air) for representative readings.

## HX711 + Load Cell (Digital)

| HX711 Pin | ESP32 Pin | Notes                    |
|-----------|-----------|----------------------------|
| VCC       | 3.3V (or 5V, module-dependent) | Check module silkscreen |
| GND       | GND       |                             |
| DOUT      | GPIO 16   | `PIN_HX711_DOUT`            |
| SCK       | GPIO 17   | `PIN_HX711_SCK`             |
| E+/E-/A+/A-| Load cell wiring per load cell datasheet |

The battery pack physically rests on the load cell platform; weight
tracking is a proxy for detecting mechanical/electrolyte changes over the
pack's lifetime.

## Status LED

| Function     | ESP32 Pin | Notes                          |
|---------------|-----------|-----------------------------------|
| Heartbeat LED | GPIO 2    | Onboard LED on most DevKit V1 boards, toggles every 1s (`PIN_STATUS_LED`) |

## Power Notes

- The ESP32 DevKit V1 can be powered via USB (5V) during development.
- In a field installation, power the ESP32 from a regulated 5V supply
  derived from the monitored battery pack (with appropriate
  over-voltage/reverse-polarity protection), independent of the CAN bus
  transceiver's own supply rail if isolation is required.
- Ensure a **common ground** between the ESP32, all sensors, and the
  MCP2515/CAN transceiver.

## Bill of Materials (Reference)

| Qty | Component                          |
|-----|--------------------------------------|
| 1   | ESP32 DevKit V1                        |
| 1   | MCP2515 + TJA1050 CAN Bus Module       |
| 1   | INA219 Breakout Board                  |
| 1   | DS18B20 Waterproof Temperature Probe   |
| 1   | HX711 Load Cell Amplifier Module       |
| 1   | Load Cell (capacity sized to pack weight) |
| 1   | 4.7kΩ resistor (DS18B20 pull-up, if not already on module) |
| 2   | 120Ω CAN bus termination resistors     |
| 1   | USB-CAN adapter (host side)            |

See `datasheets/README.md` for where to source the official component
datasheets.
