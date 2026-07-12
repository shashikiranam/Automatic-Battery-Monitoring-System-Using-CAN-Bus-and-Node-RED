/**
 * @file    mcp2515.h
 * @brief   Driver wrapper for the MCP2515 CAN Bus controller (SPI).
 *
 * Dependency: "mcp_can" library by coryjfowler
 *             (Arduino Library Manager -> "mcp_can")
 */

#ifndef MCP2515_DRIVER_H
#define MCP2515_DRIVER_H

#include <Arduino.h>

/**
 * @brief Initialize the MCP2515 controller over SPI at the configured
 *        bit rate (see config.h: CAN_BUS_SPEED, CAN_CLOCK_MHZ).
 * @return true on success.
 */
bool mcp2515_driver_init(void);

/**
 * @brief Transmit a standard CAN data frame.
 *
 * @param can_id  11-bit standard CAN identifier.
 * @param data    Pointer to up to 8 bytes of payload.
 * @param length  Number of valid bytes in data (0-8).
 * @return true if the frame was accepted for transmission.
 */
bool mcp2515_driver_send(uint32_t can_id, const uint8_t *data, uint8_t length);

/**
 * @brief Non-blocking check for a received CAN frame.
 *
 * @param out_can_id  Pointer to receive the identifier of the received frame.
 * @param out_data    Buffer (>= 8 bytes) to receive payload.
 * @param out_length  Pointer to receive payload length.
 * @return true if a frame was received and copied out.
 */
bool mcp2515_driver_receive(uint32_t *out_can_id, uint8_t *out_data, uint8_t *out_length);

/**
 * @brief Returns true if the controller reported a bus error / error-passive
 *        state on the last status check.
 */
bool mcp2515_driver_has_error(void);

#endif /* MCP2515_DRIVER_H */
