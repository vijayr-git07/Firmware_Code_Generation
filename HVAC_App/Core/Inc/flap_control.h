/**
 * @file    flap_control.h
 * @brief   HVAC Flap Control - Flap Control Logic interface
 *
 * Compares the target position against the current feedback position,
 * determines movement direction, monitors the precise stop window,
 * and coordinates motor and LED updates.
 *
 * SWE-REQ: 017, 018, 019, 020, 021, 022, 023
 */

#ifndef FLAP_CONTROL_H
#define FLAP_CONTROL_H

#include <stdint.h>

/* ---------------- Public API -------------------------------------- */

/**
 * @brief  Initialise flap control logic.
 */
void flap_control_init(void);

/**
 * @brief  Execute one cycle of flap control logic.
 *
 * Reads the latest command, compares against feedback, drives the
 * motor, and updates LEDs.  Must be called once per superloop
 * iteration after command_parser_process() and
 * feedback_processor_update().
 */
void flap_control_process(void);

/**
 * @brief  Set a new target flap position.
 * @param  target  0 .. 5.
 */
void flap_control_set_target(uint8_t target);

#endif /* FLAP_CONTROL_H */
