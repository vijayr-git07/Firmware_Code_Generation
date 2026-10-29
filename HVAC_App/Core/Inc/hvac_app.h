/**
 * @file    hvac_app.h
 * @brief   HVAC application integration interface.
 *
 * Provides the fixed application integration contract:
 *   hvac_app_init()  – called once after platform startup.
 *   hvac_app_task()  – called repeatedly from the super-loop.
 */

#ifndef HVAC_APP_H
#define HVAC_APP_H

/**
 * @brief  Initialize all HVAC application modules.
 *
 * Must be called once after all CubeMX peripheral initialization
 * has completed.  Does not repeat hardware initialization.
 */
void hvac_app_init(void);

/**
 * @brief  Execute one HVAC application processing cycle.
 *
 * Coordinates command parsing, feedback acquisition, flap control,
 * motor output, and LED status in the documented sequence.
 * Returns to its caller; contains no infinite loop.
 */
void hvac_app_task(void);

#endif /* HVAC_APP_H */
