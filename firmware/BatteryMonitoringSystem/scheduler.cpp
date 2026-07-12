/**
 * @file    scheduler.cpp
 * @brief   Implementation of the cooperative task scheduler.
 */

#include "scheduler.h"
#include "utilities.h"

static scheduler_task_t s_tasks[SCHEDULER_MAX_TASKS];
static uint8_t          s_task_count = 0U;

void scheduler_init(void)
{
    s_task_count = 0U;
    for (uint8_t i = 0U; i < SCHEDULER_MAX_TASKS; i++) {
        s_tasks[i].callback    = NULL;
        s_tasks[i].period_ms   = 0U;
        s_tasks[i].last_run_ms = 0U;
        s_tasks[i].enabled     = false;
    }
}

bool scheduler_register_task(scheduler_task_fn_t callback, uint32_t period_ms)
{
    if (callback == NULL || s_task_count >= SCHEDULER_MAX_TASKS) {
        return false;
    }

    s_tasks[s_task_count].callback    = callback;
    s_tasks[s_task_count].period_ms   = period_ms;
    s_tasks[s_task_count].last_run_ms = millis();
    s_tasks[s_task_count].enabled     = true;
    s_task_count++;

    return true;
}

void scheduler_run(void)
{
    for (uint8_t i = 0U; i < s_task_count; i++) {
        if (!s_tasks[i].enabled || s_tasks[i].callback == NULL) {
            continue;
        }

        if (utilities_elapsed_ms(s_tasks[i].last_run_ms) >= s_tasks[i].period_ms) {
            s_tasks[i].last_run_ms = millis();
            s_tasks[i].callback();
        }
    }
}
