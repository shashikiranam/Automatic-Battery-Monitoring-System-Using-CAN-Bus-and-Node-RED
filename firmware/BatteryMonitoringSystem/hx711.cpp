/**
 * @file    hx711.cpp
 * @brief   Implementation of the HX711 load-cell driver wrapper.
 */

#include "hx711.h"
#include "config.h"
#include "uart_debug.h"
#include <HX711.h>

static HX711 s_scale;
static bool  s_initialized = false;

bool hx711_driver_init(void)
{
    s_scale.begin(PIN_HX711_DOUT, PIN_HX711_SCK);

    if (!s_scale.wait_ready_timeout(1000)) {
        uart_debug_println("[HX711] ERROR: Amplifier not responding.");
        s_initialized = false;
        return false;
    }

    s_scale.set_scale(HX711_CALIBRATION_FACTOR);
    s_scale.tare(HX711_TARE_SAMPLES);

    s_initialized = true;
    uart_debug_println("[HX711] Load cell initialized and tared.");
    return true;
}

bool hx711_driver_read(float *out_weight_kg)
{
    if (out_weight_kg == NULL || !s_initialized) {
        return false;
    }

    if (!s_scale.wait_ready_timeout(200)) {
        uart_debug_println("[HX711] WARNING: Amplifier not ready.");
        return false;
    }

    /* get_units() returns the value in the same unit as the calibration
     * factor was derived (kilograms, in this configuration). */
    float weight_kg = s_scale.get_units(3);

    /* Load cell noise can produce small negative values near zero load. */
    if (weight_kg < 0.0f && weight_kg > -0.05f) {
        weight_kg = 0.0f;
    }

    *out_weight_kg = weight_kg;
    return true;
}

void hx711_driver_tare(void)
{
    if (!s_initialized) {
        return;
    }
    s_scale.tare(HX711_TARE_SAMPLES);
    uart_debug_println("[HX711] Re-tared.");
}
