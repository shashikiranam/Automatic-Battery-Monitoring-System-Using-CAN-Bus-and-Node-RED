/**
 * @file    ina219.cpp
 * @brief   Implementation of the INA219 driver wrapper.
 */

#include "ina219.h"
#include "config.h"
#include "uart_debug.h"
#include <Wire.h>
#include <Adafruit_INA219.h>

static Adafruit_INA219 s_ina219(INA219_I2C_ADDRESS);
static bool             s_initialized = false;

bool ina219_driver_init(void)
{
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);

    if (!s_ina219.begin(&Wire)) {
        uart_debug_println("[INA219] ERROR: Sensor not detected on I2C bus.");
        s_initialized = false;
        return false;
    }

    /* Calibrate for a 32V, 2A range - suitable for typical small battery
     * packs. Adjust via setCalibration_* if a different range is needed. */
    s_ina219.setCalibration_32V_2A();

    s_initialized = true;
    uart_debug_println("[INA219] Sensor initialized successfully.");
    return true;
}

bool ina219_driver_read(ina219_reading_t *out_reading)
{
    if (out_reading == NULL) {
        return false;
    }

    if (!s_initialized) {
        out_reading->valid = false;
        return false;
    }

    float shunt_voltage_mv = s_ina219.getShuntVoltage_mV();
    float bus_voltage_v    = s_ina219.getBusVoltage_V();
    float current_ma       = s_ina219.getCurrent_mA();
    float power_mw         = s_ina219.getPower_mW();

    out_reading->shunt_voltage_mv = shunt_voltage_mv;
    /* Total battery terminal voltage = bus voltage + shunt drop */
    out_reading->bus_voltage_v    = bus_voltage_v + (shunt_voltage_mv / 1000.0f);
    out_reading->current_a        = current_ma / 1000.0f;
    out_reading->power_w          = power_mw / 1000.0f;
    out_reading->valid            = true;

    return true;
}
