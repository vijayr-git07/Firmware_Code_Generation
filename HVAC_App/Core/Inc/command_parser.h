/**
 * @file    command_parser.h
 * @brief   UART command parser for HVAC flap position commands.
 *
 * Provides interfaces to initialize, process, and retrieve the latest
 * validated HVAC flap target position received over USART2.
 */

#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <stdint.h>

/**
 * @brief  Initialize the command parser module.
 *
 * Resets internal state so that no valid command is pending.
 */
void command_parser_init(void);

/**
 * @brief  Poll USART2 for a new command byte and validate it.
 *
 * If a byte in the range '0' .. '5' is received it is stored as the
 * latest valid command.  Any other value is silently discarded.
 * A short polling timeout is used so the call never blocks the
 * super-loop indefinitely.
 */
void command_parser_process(void);

/**
 * @brief  Retrieve the latest validated target position.
 *
 * @param[out] position  Pointer that receives the position (0 .. 5).
 * @return     1 if a valid command is available, 0 otherwise.
 */
uint8_t command_parser_get_latest(uint8_t *position);

#endif /* COMMAND_PARSER_H */
