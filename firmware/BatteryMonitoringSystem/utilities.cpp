/**
 * @file    utilities.cpp
 * @brief   Implementation of miscellaneous helper utilities.
 */

#include "utilities.h"

float utilities_clamp_float(float value, float min_value, float max_value)
{
    if (value < min_value) {
        return min_value;
    }
    if (value > max_value) {
        return max_value;
    }
    return value;
}

void utilities_pack_float_to_i16(float value, float scale_factor, uint8_t *out_bytes)
{
    if (out_bytes == NULL) {
        return;
    }

    int32_t scaled = (int32_t)(value * scale_factor);

    /* Clamp into int16_t range to avoid overflow when packing. */
    if (scaled > INT16_MAX) {
        scaled = INT16_MAX;
    } else if (scaled < INT16_MIN) {
        scaled = INT16_MIN;
    }

    int16_t value_i16 = (int16_t)scaled;

    out_bytes[0] = (uint8_t)((value_i16 >> 8) & 0xFF); /* MSB */
    out_bytes[1] = (uint8_t)(value_i16 & 0xFF);         /* LSB */
}

float utilities_unpack_i16_to_float(const uint8_t *in_bytes, float scale_factor)
{
    if (in_bytes == NULL || scale_factor == 0.0f) {
        return 0.0f;
    }

    int16_t value_i16 = (int16_t)((in_bytes[0] << 8) | in_bytes[1]);
    return ((float)value_i16) / scale_factor;
}

uint32_t utilities_elapsed_ms(uint32_t previous_millis)
{
    /* Subtraction on unsigned integers naturally handles millis() overflow
     * (wraps around after ~49.7 days). */
    return (uint32_t)(millis() - previous_millis);
}
