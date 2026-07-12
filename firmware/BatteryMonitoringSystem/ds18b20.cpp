/**
 * @file    ds18b20.cpp
 * @brief   Implementation of the DS18B20 driver wrapper.
 */

#include "ds18b20.h"
#include "config.h"
#include "uart_debug.h"
#include <OneWire.h>
#include <DallasTemperature.h>

static OneWire           s_one_wire(PIN_ONEWIRE_BUS);
static DallasTemperature s_sensors(&s_one_wire);
static DeviceAddress     s_device_address;
static bool              s_initialized = false;

/* Out-of-range value returned by DallasTemperature on a failed read. */
#define DS18B20_DISCONNECTED_C -127.0f

bool ds18b20_driver_init(void)
{
    s_sensors.begin();

    uint8_t device_count = s_sensors.getDeviceCount();
    if (device_count == 0U) {
        uart_debug_println("[DS18B20] ERROR: No 1-Wire temperature sensors found.");
        s_initialized = false;
        return false;
    }

    if (!s_sensors.getAddress(s_device_address, 0)) {
        uart_debug_println("[DS18B20] ERROR: Unable to read device address.");
        s_initialized = false;
        return false;
    }

    /* 12-bit resolution: 0.0625 C, ~750ms conversion time. */
    s_sensors.setResolution(s_device_address, 12);
    s_sensors.setWaitForConversion(true);

    s_initialized = true;
    uart_debug_println("[DS18B20] Sensor initialized successfully.");
    return true;
}

bool ds18b20_driver_read(float *out_temperature_c)
{
    if (out_temperature_c == NULL || !s_initialized) {
        return false;
    }

    s_sensors.requestTemperatures();
    float temperature_c = s_sensors.getTempC(s_device_address);

    if (temperature_c == DS18B20_DISCONNECTED_C) {
        uart_debug_println("[DS18B20] WARNING: Sensor disconnected or read error.");
        return false;
    }

    *out_temperature_c = temperature_c;
    return true;
}
