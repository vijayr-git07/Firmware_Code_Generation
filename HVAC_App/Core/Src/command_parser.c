/**
 * @file    command_parser.c
 * @brief   HVAC Flap Control - Command Parser implementation
 *
 * SWE-REQ: 012, 013, 014, 015, 040, 044, 051, 054
 */

#include "command_parser.h"
#include "main.h"          /* HAL types, Error_Handler */

/* ---- Extern HAL handle (defined in main.c) ----------------------- */
extern UART_HandleTypeDef huart2;

/* ---- Module-static data ------------------------------------------ */
static uint8_t latest_command;
static uint8_t command_valid_flag;

/* ---- Public API -------------------------------------------------- */

void command_parser_init(void)
{
    latest_command    = 0xFFU;   /* CMD_INVALID */
    command_valid_flag = 0U;
}

void command_parser_process(void)
{
    uint8_t rx_byte = 0U;
    HAL_StatusTypeDef status;

    /* Non-blocking single-byte receive (1 ms timeout). */
    status = HAL_UART_Receive(&huart2, &rx_byte, 1U, 1U);

    if (status == HAL_OK)
    {
        /* Validate: ASCII '0' .. '5' maps to position 0 .. 5. */
        if ((rx_byte >= (uint8_t)'0') && (rx_byte <= (uint8_t)'5'))
        {
            latest_command     = rx_byte - (uint8_t)'0';
            command_valid_flag = 1U;
        }
        /* else: invalid character — silently ignored (SWE-REQ-013). */
    }
    /* HAL_TIMEOUT / HAL_ERROR — no data available, nothing to do. */
}

uint8_t command_parser_get_latest(void)
{
    if (command_valid_flag != 0U)
    {
        return latest_command;
    }
    return 0xFFU;   /* CMD_INVALID */
}
