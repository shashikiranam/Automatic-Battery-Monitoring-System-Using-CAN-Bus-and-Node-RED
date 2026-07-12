/**
 * @file    filter.cpp
 * @brief   Implementation of the moving-average filter.
 */

#include "filter.h"

void filter_init(filter_moving_average_t *filter, uint8_t window_size)
{
    if (filter == NULL) {
        return;
    }

    if (window_size == 0U) {
        window_size = 1U;
    }
    if (window_size > FILTER_MAX_WINDOW_SIZE) {
        window_size = FILTER_MAX_WINDOW_SIZE;
    }

    filter->window_size  = window_size;
    filter_reset(filter);
}

float filter_apply(filter_moving_average_t *filter, float new_sample)
{
    if (filter == NULL) {
        return new_sample;
    }

    /* Remove the value being overwritten from the running sum, unless the
     * buffer is not yet full. */
    if (filter->sample_count >= filter->window_size) {
        filter->running_sum -= filter->buffer[filter->write_index];
    } else {
        filter->sample_count++;
    }

    filter->buffer[filter->write_index] = new_sample;
    filter->running_sum += new_sample;

    filter->write_index++;
    if (filter->write_index >= filter->window_size) {
        filter->write_index = 0U;
    }

    return filter->running_sum / (float)filter->sample_count;
}

void filter_reset(filter_moving_average_t *filter)
{
    if (filter == NULL) {
        return;
    }

    for (uint8_t i = 0U; i < FILTER_MAX_WINDOW_SIZE; i++) {
        filter->buffer[i] = 0.0f;
    }

    filter->write_index  = 0U;
    filter->sample_count = 0U;
    filter->running_sum  = 0.0f;
}
