/**
 * @file    scheduler.h
 * @brief   Lightweight, non-blocking cooperative task scheduler.
 *
 * @details Implements a simple millis()-based periodic task scheduler so
 *          the main loop() never blocks with delay(), keeping the firmware
 *          watchdog-friendly and responsive.
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <Arduino.h>

#define SCHEDULER_MAX_TASKS 8U

typedef void (*scheduler_task_fn_t)(void);

typedef struct {
    scheduler_task_fn_t callback;
    uint32_t            period_ms;
    uint32_t            last_run_ms;
    bool                enabled;
} scheduler_task_t;

/**
 * @brief Initialize the scheduler (clears the task table).
 */
void scheduler_init(void);

/**
 * @brief Register a periodic task.
 *
 * @param callback   Function to call when the period elapses.
 * @param period_ms  Period in milliseconds.
 * @return true if the task was registered successfully, false if the task
 *         table is full.
 */
bool scheduler_register_task(scheduler_task_fn_t callback, uint32_t period_ms);

/**
 * @brief Run the scheduler. Must be called continuously from loop().
 *        Executes any due tasks (non-blocking).
 */
void scheduler_run(void);

#endif /* SCHEDULER_H */
