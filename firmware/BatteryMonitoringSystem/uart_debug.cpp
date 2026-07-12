/**
 * @file    uart_debug.cpp
 * @brief   Implementation of centralized UART debug logging.
 */

#include "uart_debug.h"
#include "config.h"
#include <stdarg.h>

void uart_debug_init(void)
{
#if ENABLE_UART_DEBUG
    Serial.begin(UART_DEBUG_BAUDRATE);
    uint32_t start = millis();
    while (!Serial && (millis() - start) < 2000U) {
        /* Wait briefly for USB-CDC serial to be ready on some boards. */
    }
    Serial.println();
    Serial.println("==============================================");
    Serial.println(" Automatic Battery Monitoring System (ESP32)   ");
    Serial.println(" CAN Bus + Node-RED Dashboard                  ");
    Serial.println("==============================================");
#endif
}

void uart_debug_println(const char *message)
{
#if ENABLE_UART_DEBUG
    Serial.println(message);
#else
    (void)message;
#endif
}

void uart_debug_print(const char *message)
{
#if ENABLE_UART_DEBUG
    Serial.print(message);
#else
    (void)message;
#endif
}

void uart_debug_printf(const char *format, ...)
{
#if ENABLE_UART_DEBUG
    char buffer[160];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    Serial.print(buffer);
#else
    (void)format;
#endif
}
