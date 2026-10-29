/**
 * @file    command_parser.h
 * @brief   HVAC Flap Control - Command Parser interface
 *
 * Receives and validates UART commands representing HVAC flap
 * target positions ('0' to '5').  Invalid characters are silently
 * discarded.
 *
 * SWE-REQ: 012, 013, 014, 015, 051
 */

#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <stdint.h>

/* ---------------- Public Constants -------------------------------- */

/** Number of supported flap positions (0 .. 5). */
#define CMD_POSITION_MIN  ((uint8_t)0U)
#define CMD_POSITION_MAX  ((uint8_t)5U)

/** Sentinel returned when no valid command has been received yet. */
#define CMD_INVALID       ((uint8_t)0xFFU)

/* ---------------- Public API -------------------------------------- */

/**
 * @brief  Initialise the command parser module.
 *
 * Clears internal state so that command_parser_get_latest() returns
 * CMD_INVALID until the first valid command arrives.
 */
void command_parser_init(void);

/**
 * @brief  Poll UART for one byte and validate it.
 *
 * If a byte is available and represents a valid position character
 * ('0' .. '5'), the internal latest-command is updated.
 * Any other byte is silently ignored.
 *
 * Must be called once per superloop iteration.
 */
void command_parser_process(void);

/**
 * @brief  Return the latest validated target position.
 * @return 0 .. 5 if a valid command has been received,
 *         CMD_INVALID (0xFF) otherwise.
 */
uint8_t command_parser_get_latest(void);

#endif /* COMMAND_PARSER_H */
