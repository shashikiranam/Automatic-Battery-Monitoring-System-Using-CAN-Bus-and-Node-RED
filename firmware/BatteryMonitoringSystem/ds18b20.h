/**
 * @file    ds18b20.h
 * @brief   Driver wrapper for the DS18B20 1-Wire temperature sensor.
 *
 * Dependencies: OneWire library, DallasTemperature library
 *               (Arduino Library Manager)
 */

#ifndef DS18B20_H
#define DS18B20_H

#include <Arduino.h>

/**
 * @brief Initialize the 1-Wire bus and locate the DS18B20 sensor.
 * @return true if at least one DS18B20 device was found.
 */
bool ds18b20_driver_init(void);

/**
 * @brief Request a temperature conversion and read the result.
 *
 * @param out_temperature_c Pointer to receive temperature in Celsius.
 * @return true if a valid reading was obtained.
 */
bool ds18b20_driver_read(float *out_temperature_c);

#endif /* DS18B20_H */
