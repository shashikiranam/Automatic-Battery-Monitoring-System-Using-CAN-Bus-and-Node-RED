/**
 * @file    ina219.h
 * @brief   Driver wrapper for the INA219 current/voltage sensor (I2C).
 *
 * @details Thin wrapper around the Adafruit_INA219 library, exposing a
 *          simplified interface that matches this project's naming
 *          conventions and adds basic fault handling.
 *
 * Dependency: Adafruit INA219 library
 *             (Arduino Library Manager -> "Adafruit INA219")
 */

#ifndef INA219_H
#define INA219_H

#include <Arduino.h>

typedef struct {
    float bus_voltage_v;   /* Battery terminal voltage, Volts   */
    float shunt_voltage_mv;/* Shunt voltage, millivolts         */
    float current_a;       /* Battery current, Amps (signed)    */
    float power_w;         /* Instantaneous power, Watts        */
    bool  valid;           /* true if the last read succeeded   */
} ina219_reading_t;

/**
 * @brief Initialize the INA219 sensor over I2C.
 * @return true on success, false if the sensor was not detected.
 */
bool ina219_driver_init(void);

/**
 * @brief Perform a single reading of voltage/current/power.
 * @param out_reading Pointer to structure receiving the reading.
 * @return true if the read succeeded.
 */
bool ina219_driver_read(ina219_reading_t *out_reading);

#endif /* INA219_H */
