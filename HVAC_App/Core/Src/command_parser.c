/**
 * @file    command_parser.c
 * @brief   UART command parser implementation for HVAC flap control.
 */

#include "command_parser.h"
#include "main.h"
#include <string.h>

/* --------------- external HAL handle (defined in main.c) --------------- */
extern UART_HandleTypeDef huart2;

/* --------------- module constants -------------------------------------- */
#define CMD_MIN_CHAR  '0'
#define CMD_MAX_CHAR  '5'
#define UART_RX_TIMEOUT_MS  5U

/* --------------- module-static state ----------------------------------- */
static uint8_t latest_command;
static uint8_t command_valid_flag;

/* ======================================================================= */
void command_parser_init(void)
{
    latest_command    = 0U;
    command_valid_flag = 0U;
}

/* ======================================================================= */
void command_parser_process(void)
{
    uint8_t rx_byte = 0U;
    HAL_StatusTypeDef status;

    status = HAL_UART_Receive(&huart2, &rx_byte, 1U, UART_RX_TIMEOUT_MS);

    if (status == HAL_OK)
    {
        if ((rx_byte >= CMD_MIN_CHAR) && (rx_byte <= CMD_MAX_CHAR))
        {
            latest_command     = rx_byte - CMD_MIN_CHAR;  /* 0 .. 5 */
            command_valid_flag = 1U;
        }
        /* else: invalid character – silently ignored */
    }
    /* else: timeout or error – no new command */
}

/* ======================================================================= */
uint8_t command_parser_get_latest(uint8_t *position)
{
    if ((position != NULL) && (command_valid_flag != 0U))
    {
        *position = latest_command;
        return 1U;
    }
    return 0U;
}
