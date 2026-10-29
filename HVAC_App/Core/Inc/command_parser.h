/**
 * @file    command_parser.h
 * @brief   UART command parser interface for HVAC flap control
 */

#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief  Initialize the command parser module.
 */
void command_parser_init(void);

/**
 * @brief  Process UART input. Call periodically from the runtime loop.
 */
void command_parser_process(void);

/**
 * @brief  Get the latest validated target position command.
 * @param  p_position  Pointer to store the target position (0-5).
 * @return 1 if a valid command is available, 0 otherwise.
 */
uint8_t command_parser_get_latest(uint8_t *p_position);

#ifdef __cplusplus
}
#endif

#endif /* COMMAND_PARSER_H */
