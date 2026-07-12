/**
 * @file    BatteryMonitoringSystem.ino
 * @brief   Automatic Battery Monitoring System using CAN Bus and Node-RED.
 *
 * @details Top-level Arduino sketch. Wires together sensor drivers,
 *          the battery data model, CAN protocol packing, and the
 *          non-blocking task scheduler.
 *
 * Board:   ESP32 DevKit V1
 * IDE:     Arduino IDE (>= 2.x recommended)
 *
 * Required Libraries (Arduino Library Manager):
 *   - Adafruit INA219
 *   - OneWire
 *   - DallasTemperature
 *   - HX711 (by bogde)
 *   - mcp_can (by coryjfowler)
 *
 * See docs/BUILD_INSTRUCTIONS.md for full setup instructions.
 *
 * @author  Battery Monitoring System Firmware Team
 * @date    2026
 */

#include "config.h"
#include "uart_debug.h"
#include "scheduler.h"
#include "battery.h"
#include "can_protocol.h"

#if ENABLE_WATCHDOG
#include "esp_task_wdt.h"
#endif

/* -------------------------------------------------------------------------
 *  Forward declarations of scheduled tasks
 * ------------------------------------------------------------------------- */
static void task_sensor_read(void);
static void task_can_transmit(void);
static void task_uart_debug_print(void);
static void task_led_heartbeat(void);

static bool s_led_state = false;

/* =========================================================================
 *  SETUP
 * ========================================================================= */
void setup(void)
{
    uart_debug_init();

#if ENABLE_STATUS_LED
    pinMode(PIN_STATUS_LED, OUTPUT);
    digitalWrite(PIN_STATUS_LED, LOW);
#endif

    uart_debug_println("[MAIN] Initializing sensors...");
    bool sensors_ok = battery_module_init();
    if (!sensors_ok) {
        uart_debug_println("[MAIN] WARNING: Continuing with degraded sensor set.");
    }

    uart_debug_println("[MAIN] Initializing CAN bus controller...");
    bool can_ok = mcp2515_driver_init();
    if (!can_ok) {
        uart_debug_println("[MAIN] ERROR: CAN bus initialization failed. Halting is avoided; "
                            "system will keep retrying via periodic re-checks.");
    }

#if ENABLE_WATCHDOG
    esp_task_wdt_init(WATCHDOG_TIMEOUT_S, true);
    esp_task_wdt_add(NULL);
    uart_debug_println("[MAIN] Watchdog enabled.");
#endif

    scheduler_init();
    scheduler_register_task(task_sensor_read,      TASK_PERIOD_SENSOR_READ_MS);
    scheduler_register_task(task_can_transmit,      TASK_PERIOD_CAN_TX_MS);
    scheduler_register_task(task_uart_debug_print,  TASK_PERIOD_UART_DEBUG_MS);
    scheduler_register_task(task_led_heartbeat,     TASK_PERIOD_LED_HEARTBEAT_MS);

    uart_debug_println("[MAIN] Setup complete. Entering main loop.");
}

/* =========================================================================
 *  MAIN LOOP
 * ========================================================================= */
void loop(void)
{
    scheduler_run();

#if ENABLE_WATCHDOG
    esp_task_wdt_reset();
#endif
}

/* =========================================================================
 *  TASK IMPLEMENTATIONS
 * ========================================================================= */

/**
 * @brief Periodic task: sample all sensors and update the filtered
 *        battery state and status.
 */
static void task_sensor_read(void)
{
    battery_module_update();
}

/**
 * @brief Periodic task: pack and transmit the latest battery state over
 *        the CAN bus (frames 0x100-0x105).
 */
static void task_can_transmit(void)
{
    const battery_state_t *state = battery_module_get_state();
    can_protocol_transmit_battery_state(state);
}

/**
 * @brief Periodic task: print a human-readable telemetry snapshot to the
 *        debug UART.
 */
static void task_uart_debug_print(void)
{
    const battery_state_t *state = battery_module_get_state();

    uart_debug_printf(
        "[TELEMETRY] V=%.2fV  I=%.2fA  T=%.2fC  W=%.3fkg  P=%.2fW  Status=%s  SensorsOK=%d\n",
        state->voltage_v,
        state->current_a,
        state->temperature_c,
        state->weight_kg,
        state->power_w,
        battery_status_to_string(state->status),
        state->sensors_ok ? 1 : 0
    );
}

/**
 * @brief Periodic task: toggle the onboard status LED as a heartbeat
 *        indicator that the main loop is running and not stuck.
 */
static void task_led_heartbeat(void)
{
#if ENABLE_STATUS_LED
    s_led_state = !s_led_state;
    digitalWrite(PIN_STATUS_LED, s_led_state ? HIGH : LOW);
#endif
}
