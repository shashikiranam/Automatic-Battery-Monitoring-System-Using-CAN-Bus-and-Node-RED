/**
 * @file    config.h
 * @brief   Global configuration for the Automatic Battery Monitoring System.
 *
 * @details Contains pin mappings, timing constants, CAN bus configuration,
 *          threshold values and feature flags used across the firmware.
 *
 * @author  Battery Monitoring System Firmware Team
 * @date    2026
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

/* =========================================================================
 *  BOARD: ESP32 DevKit V1
 * ========================================================================= */

/* -------------------------------------------------------------------------
 *  SPI PIN MAPPING (Shared bus - MCP2515)
 * ------------------------------------------------------------------------- */
#define PIN_SPI_SCK             18
#define PIN_SPI_MISO            19
#define PIN_SPI_MOSI            23
#define PIN_MCP2515_CS          5
#define PIN_MCP2515_INT         4

/* -------------------------------------------------------------------------
 *  I2C PIN MAPPING (INA219)
 * ------------------------------------------------------------------------- */
#define PIN_I2C_SDA              21
#define PIN_I2C_SCL              22
#define INA219_I2C_ADDRESS       0x40

/* -------------------------------------------------------------------------
 *  DS18B20 (OneWire) PIN MAPPING
 * ------------------------------------------------------------------------- */
#define PIN_ONEWIRE_BUS          15

/* -------------------------------------------------------------------------
 *  HX711 LOAD CELL PIN MAPPING
 * ------------------------------------------------------------------------- */
#define PIN_HX711_DOUT           16
#define PIN_HX711_SCK            17

/* -------------------------------------------------------------------------
 *  STATUS LED / DEBUG
 * ------------------------------------------------------------------------- */
#define PIN_STATUS_LED           2
#define UART_DEBUG_BAUDRATE      115200

/* =========================================================================
 *  CAN BUS CONFIGURATION
 * ========================================================================= */
#define CAN_BUS_SPEED            CAN_500KBPS
#define CAN_CLOCK_MHZ            MCP_8MHZ

/* CAN Message identifiers - see docs/CAN_FRAME_DOCUMENTATION.md */
#define CAN_ID_VOLTAGE           0x100
#define CAN_ID_CURRENT           0x101
#define CAN_ID_TEMPERATURE       0x102
#define CAN_ID_WEIGHT            0x103
#define CAN_ID_POWER             0x104
#define CAN_ID_SYSTEM_STATUS     0x105

#define CAN_FRAME_DLC            8U

/* =========================================================================
 *  SCHEDULER TIMING (milliseconds)
 * ========================================================================= */
#define TASK_PERIOD_SENSOR_READ_MS     100U   /* Sensor sampling period      */
#define TASK_PERIOD_CAN_TX_MS          100U   /* CAN frame transmit period   */
#define TASK_PERIOD_UART_DEBUG_MS      500U   /* Debug print period          */
#define TASK_PERIOD_LED_HEARTBEAT_MS   1000U  /* Heartbeat LED toggle period */

/* =========================================================================
 *  MOVING AVERAGE FILTER CONFIGURATION
 * ========================================================================= */
#define FILTER_WINDOW_VOLTAGE     8U
#define FILTER_WINDOW_CURRENT     8U
#define FILTER_WINDOW_TEMPERATURE 4U
#define FILTER_WINDOW_WEIGHT      6U

/* =========================================================================
 *  BATTERY THRESHOLDS
 * ========================================================================= */
/* Voltage thresholds (Volts) - example: 3S Li-ion pack (nominal 11.1V) */
#define BATTERY_VOLTAGE_MIN_WARN       10.5f
#define BATTERY_VOLTAGE_MIN_CRITICAL   9.6f
#define BATTERY_VOLTAGE_MAX_WARN       12.6f
#define BATTERY_VOLTAGE_MAX_CRITICAL   13.0f

/* Current thresholds (Amps) */
#define BATTERY_CURRENT_WARN            5.0f
#define BATTERY_CURRENT_CRITICAL        8.0f

/* Temperature thresholds (Degrees Celsius) */
#define BATTERY_TEMP_WARN               45.0f
#define BATTERY_TEMP_CRITICAL           60.0f

/* =========================================================================
 *  HX711 CALIBRATION
 * ========================================================================= */
/* Calibration factor obtained via calibration routine (see docs). Adjust
 * per physical load cell. Value below is a placeholder and MUST be
 * re-calibrated for the specific load cell used. */
#define HX711_CALIBRATION_FACTOR   -7050.0f
#define HX711_TARE_SAMPLES         10U

/* =========================================================================
 *  FEATURE FLAGS
 * ========================================================================= */
#define ENABLE_UART_DEBUG          1
#define ENABLE_STATUS_LED          1
#define ENABLE_WATCHDOG            1
#define WATCHDOG_TIMEOUT_S         8U

#endif /* CONFIG_H */
