/**
 * @file    command_parser.c
 * @brief   Command Parser module implementation for HVAC flap control.
 *
 * Polls USART2 for incoming bytes and validates them as flap position
 * commands ('0' through '5'). Invalid characters are silently discarded.
 */

#include "command_parser.h"
#include "main.h"

/* ---------------------------------------------------------------------------
 * External HAL handle (defined in main.c)
 * -----------------------------------------------------------------------*/
extern UART_HandleTypeDef huart2;

/* ---------------------------------------------------------------------------
 * Module constants
 * -----------------------------------------------------------------------*/
#define CMD_POSITION_MIN  ((uint8_t)'0')
#define CMD_POSITION_MAX  ((uint8_t)'5')
#define UART_RX_TIMEOUT   ((uint32_t)1U)  /* 1 ms polling timeout */

/* ---------------------------------------------------------------------------
 * Module-static variables
 * -----------------------------------------------------------------------*/
static uint8_t latest_command      = 0U;
static uint8_t command_valid_flag  = 0U;

/* ---------------------------------------------------------------------------
 * Public functions
 * -----------------------------------------------------------------------*/

void command_parser_init(void)
{
    latest_command     = 0U;
    command_valid_flag = 0U;
}

void command_parser_process(void)
{
    uint8_t rx_byte = 0U;
    HAL_StatusTypeDef status;

    status = HAL_UART_Receive(&huart2, &rx_byte, 1U, UART_RX_TIMEOUT);

    if (status == HAL_OK)
    {
        /* Validate: accept only '0' .. '5' */
        if ((rx_byte >= CMD_POSITION_MIN) && (rx_byte <= CMD_POSITION_MAX))
        {
            latest_command     = rx_byte - CMD_POSITION_MIN;  /* 0..5 */
            command_valid_flag = 1U;
        }
        /* else: invalid command — silently ignored */
    }
    /* else: no data or error — do nothing */
}

uint8_t command_parser_get_latest(uint8_t *p_position)
{
    uint8_t valid = 0U;

    if ((p_position != (void *)0) && (command_valid_flag != 0U))
    {
        *p_position = latest_command;
        valid = 1U;
    }

    return valid;
}
