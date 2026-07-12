/**
 * @file    battery.cpp
 * @brief   Implementation of battery data aggregation and status logic.
 */

#include "battery.h"
#include "config.h"
#include "filter.h"
#include "ina219.h"
#include "ds18b20.h"
#include "hx711.h"
#include "uart_debug.h"
#include <string.h>  /* memset */

static battery_state_t          s_state;
static filter_moving_average_t  s_filter_voltage;
static filter_moving_average_t  s_filter_current;
static filter_moving_average_t  s_filter_temperature;
static filter_moving_average_t  s_filter_weight;

static bool s_ina219_ok  = false;
static bool s_ds18b20_ok = false;
static bool s_hx711_ok   = false;

static battery_status_t evaluate_status(float voltage_v, float current_a, float temperature_c);

bool battery_module_init(void)
{
    memset(&s_state, 0, sizeof(s_state));
    s_state.status = BATTERY_STATUS_FAULT;

    filter_init(&s_filter_voltage,     FILTER_WINDOW_VOLTAGE);
    filter_init(&s_filter_current,     FILTER_WINDOW_CURRENT);
    filter_init(&s_filter_temperature, FILTER_WINDOW_TEMPERATURE);
    filter_init(&s_filter_weight,      FILTER_WINDOW_WEIGHT);

    s_ina219_ok  = ina219_driver_init();
    s_ds18b20_ok = ds18b20_driver_init();
    s_hx711_ok   = hx711_driver_init();

    bool all_ok = s_ina219_ok && s_ds18b20_ok && s_hx711_ok;

    if (!all_ok) {
        uart_debug_println("[BATTERY] WARNING: One or more sensors failed to initialize.");
    }

    return all_ok;
}

void battery_module_update(void)
{
    bool read_ok = true;

    ina219_reading_t ina_reading;
    if (s_ina219_ok && ina219_driver_read(&ina_reading)) {
        s_state.voltage_v = filter_apply(&s_filter_voltage, ina_reading.bus_voltage_v);
        s_state.current_a = filter_apply(&s_filter_current, ina_reading.current_a);
    } else {
        read_ok = false;
    }

    float temperature_c;
    if (s_ds18b20_ok && ds18b20_driver_read(&temperature_c)) {
        s_state.temperature_c = filter_apply(&s_filter_temperature, temperature_c);
    } else {
        read_ok = false;
    }

    float weight_kg;
    if (s_hx711_ok && hx711_driver_read(&weight_kg)) {
        s_state.weight_kg = filter_apply(&s_filter_weight, weight_kg);
    } else {
        read_ok = false;
    }

    s_state.power_w    = s_state.voltage_v * s_state.current_a;
    s_state.sensors_ok = read_ok;

    if (read_ok) {
        s_state.status = evaluate_status(s_state.voltage_v, s_state.current_a, s_state.temperature_c);
    } else {
        s_state.status = BATTERY_STATUS_FAULT;
    }
}

const battery_state_t *battery_module_get_state(void)
{
    return &s_state;
}

const char *battery_status_to_string(battery_status_t status)
{
    switch (status) {
        case BATTERY_STATUS_GOOD:     return "GOOD";
        case BATTERY_STATUS_WARNING:  return "WARNING";
        case BATTERY_STATUS_CRITICAL: return "CRITICAL";
        case BATTERY_STATUS_FAULT:    return "FAULT";
        default:                      return "UNKNOWN";
    }
}

static battery_status_t evaluate_status(float voltage_v, float current_a, float temperature_c)
{
    /* Critical conditions take priority over warnings. */
    if (voltage_v <= BATTERY_VOLTAGE_MIN_CRITICAL ||
        voltage_v >= BATTERY_VOLTAGE_MAX_CRITICAL ||
        current_a >= BATTERY_CURRENT_CRITICAL ||
        temperature_c >= BATTERY_TEMP_CRITICAL) {
        return BATTERY_STATUS_CRITICAL;
    }

    if (voltage_v <= BATTERY_VOLTAGE_MIN_WARN ||
        voltage_v >= BATTERY_VOLTAGE_MAX_WARN ||
        current_a >= BATTERY_CURRENT_WARN ||
        temperature_c >= BATTERY_TEMP_WARN) {
        return BATTERY_STATUS_WARNING;
    }

    return BATTERY_STATUS_GOOD;
}
