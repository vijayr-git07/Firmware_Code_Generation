/**
 * @file    flap_control.h
 * @brief   Flap control logic for HVAC system.
 *
 * Compares the target position against the current feedback position,
 * determines motor direction, detects the precise stop window, and
 * coordinates motor and LED updates.
 */

#ifndef FLAP_CONTROL_H
#define FLAP_CONTROL_H

#include <stdint.h>

/**
 * @brief  Initialize flap control logic.
 */
void flap_control_init(void);

/**
 * @brief  Execute one cycle of flap control processing.
 *
 * Reads the current feedback, compares with the target, drives the
 * motor in the appropriate direction, checks the precise stop window,
 * and updates LEDs.
 */
void flap_control_process(void);

/**
 * @brief  Set a new target flap position.
 *
 * @param  position  Target position (0 .. 5).
 */
void flap_control_set_target(uint8_t position);

#endif /* FLAP_CONTROL_H */
