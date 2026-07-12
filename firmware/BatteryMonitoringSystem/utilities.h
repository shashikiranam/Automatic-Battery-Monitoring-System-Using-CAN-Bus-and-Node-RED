/**
 * @file    utilities.h
 * @brief   Miscellaneous helper utilities (byte packing, clamping, etc).
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include <Arduino.h>

/**
 * @brief Clamp a float value between a minimum and maximum bound.
 */
float utilities_clamp_float(float value, float min_value, float max_value);

/**
 * @brief Pack a float (scaled to an int16_t) into two bytes, big-endian.
 *
 * @param value        Floating point value to pack.
 * @param scale_factor Multiplier applied before truncation to int16_t
 *                      (e.g. 100.0 to preserve 2 decimal digits).
 * @param out_bytes     Pointer to a 2-byte buffer to receive the result.
 */
void utilities_pack_float_to_i16(float value, float scale_factor, uint8_t *out_bytes);

/**
 * @brief Unpack two big-endian bytes into a scaled float value.
 */
float utilities_unpack_i16_to_float(const uint8_t *in_bytes, float scale_factor);

/**
 * @brief Return milliseconds elapsed since a previous timestamp, handling
 *        millis() overflow safely.
 */
uint32_t utilities_elapsed_ms(uint32_t previous_millis);

#endif /* UTILITIES_H */
