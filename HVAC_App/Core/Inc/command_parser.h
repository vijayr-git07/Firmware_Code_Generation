/**
 * @file    command_parser.h
 * @brief   Command Parser module interface for HVAC flap control.
 *
 * Receives and validates UART commands representing target flap positions.
 */

#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include "stm32f4xx_hal.h"
#include <stdint.h>

/**
 * @brief  Initialize the command parser module.
 *         Clears internal state and prepares for UART command reception.
 */
void command_parser_init(void);

/**
 * @brief  Process incoming UART data.
 *         Polls USART2 for a single byte and validates it as a position command.
 *         Valid commands are ASCII '0' through '5'.
 */
void command_parser_process(void);

/**
 * @brief  Get the latest validated target position command.
 * @param  p_position  Pointer to store the latest valid position (0-5).
 * @return 1 if a valid command is available, 0 otherwise.
 */
uint8_t command_parser_get_latest(uint8_t *p_position);

#endif /* COMMAND_PARSER_H */
