/**
 * @file    hx711.h
 * @brief   Driver wrapper for the HX711 load-cell amplifier.
 *
 * Dependency: HX711 library by bogde
 *             (Arduino Library Manager -> "HX711")
 */

#ifndef HX711_DRIVER_H
#define HX711_DRIVER_H

#include <Arduino.h>

/**
 * @brief Initialize the HX711 amplifier and perform a tare (zero) operation.
 * @return true if the amplifier responded and tare completed.
 */
bool hx711_driver_init(void);

/**
 * @brief Read the current weight in kilograms, using the configured
 *        calibration factor.
 *
 * @param out_weight_kg Pointer to receive the weight in kilograms.
 * @return true if a valid reading was obtained.
 */
bool hx711_driver_read(float *out_weight_kg);

/**
 * @brief Re-tare (zero) the scale. Call with no load on the load cell.
 */
void hx711_driver_tare(void);

#endif /* HX711_DRIVER_H */
