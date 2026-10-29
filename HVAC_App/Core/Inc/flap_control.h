/**
 * @file    flap_control.h
 * @brief   Flap Control Logic module interface for HVAC flap control.
 *
 * Compares the target position against current feedback, determines
 * motor direction, and stops the motor when the precise stop window
 * is reached.
 */

#ifndef FLAP_CONTROL_H
#define FLAP_CONTROL_H

#include "stm32f4xx_hal.h"
#include <stdint.h>

/**
 * @brief  Initialize the flap control module.
 *         Clears internal state; motor remains stopped.
 */
void flap_control_init(void);

/**
 * @brief  Execute one cycle of flap control logic.
 *         Reads feedback, compares against target, drives motor,
 *         and updates LEDs as needed.
 */
void flap_control_process(void);

/**
 * @brief  Set a new target flap position.
 * @param  position  Target position (0-5).
 */
void flap_control_set_target(uint8_t position);

#endif /* FLAP_CONTROL_H */
