/**
 * @file    system_init.h
 * @brief   HVAC Flap Control - System Initialization Manager interface
 *
 * Initialises all application-layer modules in the correct order,
 * reads the initial flap position, and sets the initial LED state.
 *
 * SWE-REQ: 046, 047, 048, 049
 */

#ifndef SYSTEM_INIT_H
#define SYSTEM_INIT_H

/* ---------------- Public API -------------------------------------- */

/**
 * @brief  Initialise all application-layer modules.
 *
 * Call once after CubeMX peripheral initialisation completes.
 */
void system_init(void);

#endif /* SYSTEM_INIT_H */
