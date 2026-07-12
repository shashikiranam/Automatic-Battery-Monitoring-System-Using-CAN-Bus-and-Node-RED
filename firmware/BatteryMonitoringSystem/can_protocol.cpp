/**
 * @file    can_protocol.cpp
 * @brief   Implementation of CAN message packing for battery telemetry.
 *
 * @details Byte layout summary (all multi-byte fields big-endian, signed
 *          16-bit, scale factor 100 unless noted). Full documentation in
 *          docs/CAN_FRAME_DOCUMENTATION.md.
 *
 *   0x100 VOLTAGE      : [0-1] voltage_v    x100  (int16)
 *   0x101 CURRENT      : [0-1] current_a    x100  (int16, signed)
 *   0x102 TEMPERATURE  : [0-1] temperature_c x100 (int16, signed)
 *   0x103 WEIGHT       : [0-1] weight_kg    x1000 (int16)
 *   0x104 POWER        : [0-1] power_w      x100  (int16)
 *   0x105 SYSTEM_STATUS: [0] battery_status enum
 *                        [1] can_bus_error (0/1)
 *                        [2] sensors_ok    (0/1)
 *                        [3] alert_flags bitmask
 */

#include "can_protocol.h"
#include "config.h"
#include "mcp2515.h"
#include "utilities.h"
#include "uart_debug.h"
#include <string.h>  /* memset */

/* Alert bitmask flags for byte [3] of the SYSTEM_STATUS frame */
#define ALERT_FLAG_OVERVOLTAGE   (1U << 0)
#define ALERT_FLAG_UNDERVOLTAGE  (1U << 1)
#define ALERT_FLAG_OVERCURRENT   (1U << 2)
#define ALERT_FLAG_OVERTEMP      (1U << 3)

#define SCALE_VOLTAGE       100.0f
#define SCALE_CURRENT       100.0f
#define SCALE_TEMPERATURE   100.0f
#define SCALE_WEIGHT        1000.0f
#define SCALE_POWER         100.0f

static uint8_t compute_alert_flags(const battery_state_t *state)
{
    uint8_t flags = 0U;

    if (state->voltage_v >= BATTERY_VOLTAGE_MAX_WARN) {
        flags |= ALERT_FLAG_OVERVOLTAGE;
    }
    if (state->voltage_v <= BATTERY_VOLTAGE_MIN_WARN) {
        flags |= ALERT_FLAG_UNDERVOLTAGE;
    }
    if (state->current_a >= BATTERY_CURRENT_WARN) {
        flags |= ALERT_FLAG_OVERCURRENT;
    }
    if (state->temperature_c >= BATTERY_TEMP_WARN) {
        flags |= ALERT_FLAG_OVERTEMP;
    }

    return flags;
}

bool can_protocol_transmit_battery_state(const battery_state_t *state)
{
    if (state == NULL) {
        return false;
    }

    bool all_ok = true;
    uint8_t payload[CAN_FRAME_DLC];

    /* --- 0x100 VOLTAGE --- */
    memset(payload, 0, sizeof(payload));
    utilities_pack_float_to_i16(state->voltage_v, SCALE_VOLTAGE, &payload[0]);
    all_ok &= mcp2515_driver_send(CAN_ID_VOLTAGE, payload, CAN_FRAME_DLC);

    /* --- 0x101 CURRENT --- */
    memset(payload, 0, sizeof(payload));
    utilities_pack_float_to_i16(state->current_a, SCALE_CURRENT, &payload[0]);
    all_ok &= mcp2515_driver_send(CAN_ID_CURRENT, payload, CAN_FRAME_DLC);

    /* --- 0x102 TEMPERATURE --- */
    memset(payload, 0, sizeof(payload));
    utilities_pack_float_to_i16(state->temperature_c, SCALE_TEMPERATURE, &payload[0]);
    all_ok &= mcp2515_driver_send(CAN_ID_TEMPERATURE, payload, CAN_FRAME_DLC);

    /* --- 0x103 WEIGHT --- */
    memset(payload, 0, sizeof(payload));
    utilities_pack_float_to_i16(state->weight_kg, SCALE_WEIGHT, &payload[0]);
    all_ok &= mcp2515_driver_send(CAN_ID_WEIGHT, payload, CAN_FRAME_DLC);

    /* --- 0x104 POWER --- */
    memset(payload, 0, sizeof(payload));
    utilities_pack_float_to_i16(state->power_w, SCALE_POWER, &payload[0]);
    all_ok &= mcp2515_driver_send(CAN_ID_POWER, payload, CAN_FRAME_DLC);

    /* --- 0x105 SYSTEM STATUS --- */
    memset(payload, 0, sizeof(payload));
    payload[0] = (uint8_t)state->status;
    payload[1] = mcp2515_driver_has_error() ? 1U : 0U;
    payload[2] = state->sensors_ok ? 1U : 0U;
    payload[3] = compute_alert_flags(state);
    all_ok &= mcp2515_driver_send(CAN_ID_SYSTEM_STATUS, payload, CAN_FRAME_DLC);

    if (!all_ok) {
        uart_debug_println("[CAN_PROTOCOL] WARNING: One or more frames failed to send.");
    }

    return all_ok;
}
