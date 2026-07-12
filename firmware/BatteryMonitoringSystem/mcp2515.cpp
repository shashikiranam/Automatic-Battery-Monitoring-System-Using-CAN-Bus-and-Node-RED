/**
 * @file    mcp2515.cpp
 * @brief   Implementation of the MCP2515 CAN driver wrapper.
 */

#include "mcp2515.h"
#include "config.h"
#include "uart_debug.h"
#include <SPI.h>
#include <mcp_can.h>
#include <string.h>  /* memcpy */

static MCP_CAN s_can(PIN_MCP2515_CS);
static bool     s_initialized = false;
static bool     s_last_error  = false;

bool mcp2515_driver_init(void)
{
    SPI.begin(PIN_SPI_SCK, PIN_SPI_MISO, PIN_SPI_MOSI, PIN_MCP2515_CS);

    pinMode(PIN_MCP2515_INT, INPUT);

    uint8_t result = s_can.begin(MCP_ANY, CAN_BUS_SPEED, CAN_CLOCK_MHZ);

    if (result != CAN_OK) {
        uart_debug_println("[MCP2515] ERROR: CAN controller initialization failed.");
        s_initialized = false;
        return false;
    }

    /* Switch to normal mode so the controller can transmit/receive on the
     * physical bus (default after begin() on this library is normal, but
     * we set explicitly for clarity/robustness). */
    s_can.setMode(MCP_NORMAL);

    s_initialized = true;
    uart_debug_println("[MCP2515] CAN controller initialized at 500 kbps.");
    return true;
}

bool mcp2515_driver_send(uint32_t can_id, const uint8_t *data, uint8_t length)
{
    if (!s_initialized || data == NULL) {
        return false;
    }

    if (length > 8U) {
        length = 8U;
    }

    uint8_t result = s_can.sendMsgBuf(can_id, 0 /* standard frame */, length, data);

    if (result != CAN_OK) {
        s_last_error = true;
        uart_debug_println("[MCP2515] WARNING: CAN transmit failed.");
        return false;
    }

    s_last_error = false;
    return true;
}

bool mcp2515_driver_receive(uint32_t *out_can_id, uint8_t *out_data, uint8_t *out_length)
{
    if (!s_initialized || out_can_id == NULL || out_data == NULL || out_length == NULL) {
        return false;
    }

    if (s_can.checkReceive() != CAN_MSGAVAIL) {
        return false;
    }

    unsigned long can_id  = 0;
    uint8_t       length  = 0;
    uint8_t       buffer[8];

    if (s_can.readMsgBuf(&can_id, &length, buffer) != CAN_OK) {
        return false;
    }

    *out_can_id  = (uint32_t)can_id;
    *out_length  = length;
    memcpy(out_data, buffer, length);

    return true;
}

bool mcp2515_driver_has_error(void)
{
    return s_last_error;
}
