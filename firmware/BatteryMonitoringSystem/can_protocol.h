/**
 * @file    can_protocol.h
 * @brief   CAN message packing/unpacking for the Battery Monitoring System.
 *
 * @details See docs/CAN_FRAME_DOCUMENTATION.md for the full frame
 *          specification (IDs, byte layout, scale factors).
 */

#ifndef CAN_PROTOCOL_H
#define CAN_PROTOCOL_H

#include <Arduino.h>
#include "battery.h"

/**
 * @brief Transmit all battery telemetry as a sequence of CAN frames
 *        (0x100-0x105). Call periodically from the CAN-TX scheduler task.
 *
 * @param state Pointer to the current battery state to transmit.
 * @return true if all frames were transmitted successfully.
 */
bool can_protocol_transmit_battery_state(const battery_state_t *state);

#endif /* CAN_PROTOCOL_H */
