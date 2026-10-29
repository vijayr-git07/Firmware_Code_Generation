/**
 * @file    command_parser.c
 * @brief   UART command parser for HVAC flap control
 *
 * Receives single-character UART commands ('0'-'5') via polling on USART2.
 * Validates and stores the latest command. Invalid characters are ignored.
 *
 * Traces to: SWE-REQ-012, SWE-REQ-013, SWE-REQ-014, SWE-REQ-015, SWE-REQ-051
 */

#include "command_parser.h"
#include "main.h"

/* ---------------------------------------------------------------------------
 * External HAL handle (defined in main.c)
 * --------------------------------------------------------------------------- */
extern UART_HandleTypeDef huart2;

/* ---------------------------------------------------------------------------
 * Module constants
 * --------------------------------------------------------------------------- */
#define CMD_MIN_CHAR  '0'
#define CMD_MAX_CHAR  '5'
#define UART_RX_TIMEOUT_MS  ((uint32_t)1U)

/* ---------------------------------------------------------------------------
 * Module-static data
 * --------------------------------------------------------------------------- */
static uint8_t latest_command;
static uint8_t command_valid_flag;

/* ---------------------------------------------------------------------------
 * Public API
 * --------------------------------------------------------------------------- */

void command_parser_init(void)
{
    latest_command    = 0U;
    command_valid_flag = 0U;
}

void command_parser_process(void)
{
    uint8_t rx_byte = 0U;
    HAL_StatusTypeDef status;

    /* Non-blocking poll: 1 ms timeout keeps superloop responsive */
    status = HAL_UART_Receive(&huart2, &rx_byte, 1U, UART_RX_TIMEOUT_MS);

    if (status == HAL_OK)
    {
        if ((rx_byte >= CMD_MIN_CHAR) && (rx_byte <= CMD_MAX_CHAR))
        {
            latest_command     = rx_byte - CMD_MIN_CHAR;  /* 0..5 */
            command_valid_flag = 1U;
        }
        /* else: invalid character — silently ignored (SWE-REQ-013) */
    }
    /* HAL_TIMEOUT / HAL_ERROR: no data available — do nothing */
}

uint8_t command_parser_get_latest(uint8_t *p_position)
{
    if ((command_valid_flag != 0U) && (p_position != (void *)0))
    {
        *p_position = latest_command;
        return 1U;
    }
    return 0U;
}
