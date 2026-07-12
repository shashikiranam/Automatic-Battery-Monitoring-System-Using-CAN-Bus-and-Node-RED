/**
 * @file    battery.h
 * @brief   Battery data model, sensor aggregation and status evaluation.
 */

#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>

typedef enum {
    BATTERY_STATUS_GOOD     = 0,
    BATTERY_STATUS_WARNING  = 1,
    BATTERY_STATUS_CRITICAL = 2,
    BATTERY_STATUS_FAULT    = 3   /* Sensor read failure */
} battery_status_t;

typedef struct {
    float             voltage_v;
    float             current_a;
    float             temperature_c;
    float             weight_kg;
    float             power_w;
    battery_status_t  status;
    bool              sensors_ok;
} battery_state_t;

/**
 * @brief Initialize the battery module (sensors + filters).
 * @return true if all sensors initialized successfully.
 */
bool battery_module_init(void);

/**
 * @brief Sample all sensors, apply filtering, compute power and status.
 *        Call this periodically from the sensor-read scheduler task.
 */
void battery_module_update(void);

/**
 * @brief Get the latest computed battery state (filtered values).
 */
const battery_state_t *battery_module_get_state(void);

/**
 * @brief Convert a battery_status_t to a human-readable string.
 */
const char *battery_status_to_string(battery_status_t status);

#endif /* BATTERY_H */
