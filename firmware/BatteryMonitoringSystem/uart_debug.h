/**
 * @file    uart_debug.h
 * @brief   Centralized UART debug logging (compile-time toggle via
 *          ENABLE_UART_DEBUG in config.h).
 */

#ifndef UART_DEBUG_H
#define UART_DEBUG_H

#include <Arduino.h>

/**
 * @brief Initialize the debug UART at the configured baud rate.
 */
void uart_debug_init(void);

/**
 * @brief Print a line to the debug UART (no-op if disabled).
 */
void uart_debug_println(const char *message);

/**
 * @brief Print without a trailing newline (no-op if disabled).
 */
void uart_debug_print(const char *message);

/**
 * @brief Printf-style formatted debug output (no-op if disabled).
 */
void uart_debug_printf(const char *format, ...);

#endif /* UART_DEBUG_H */
