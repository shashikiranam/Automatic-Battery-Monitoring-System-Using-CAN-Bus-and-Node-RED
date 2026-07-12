/**
 * @file    filter.h
 * @brief   Generic moving-average filter used to smooth noisy sensor data.
 */

#ifndef FILTER_H
#define FILTER_H

#include <Arduino.h>

#define FILTER_MAX_WINDOW_SIZE 16U

/**
 * @brief Moving average filter instance.
 */
typedef struct {
    float    buffer[FILTER_MAX_WINDOW_SIZE];
    uint8_t  window_size;
    uint8_t  write_index;
    uint8_t  sample_count;
    float    running_sum;
} filter_moving_average_t;

/**
 * @brief Initialize a moving average filter instance.
 *
 * @param filter      Pointer to filter instance.
 * @param window_size Number of samples to average over
 *                     (clamped to FILTER_MAX_WINDOW_SIZE).
 */
void filter_init(filter_moving_average_t *filter, uint8_t window_size);

/**
 * @brief Feed a new raw sample into the filter and return the filtered
 *        (averaged) output.
 */
float filter_apply(filter_moving_average_t *filter, float new_sample);

/**
 * @brief Reset filter state (clears history, does not change window size).
 */
void filter_reset(filter_moving_average_t *filter);

#endif /* FILTER_H */
