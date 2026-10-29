/**
 * @file    system_init.h
 * @brief   System Initialization Manager interface for HVAC flap control.
 *
 * Initializes all application-layer modules in the correct order,
 * reads initial feedback, and sets initial LED state.
 */

#ifndef SYSTEM_INIT_H
#define SYSTEM_INIT_H

#include <stdint.h>

/**
 * @brief  Initialize all application-layer modules.
 *         Ensures motor is in safe stopped state, reads initial
 *         feedback position, and updates initial LED state.
 *         Call once after platform (HAL/CubeMX) initialization.
 */
void system_init(void);

#endif /* SYSTEM_INIT_H */
