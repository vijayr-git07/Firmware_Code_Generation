/**
 * @file    hvac_app.h
 * @brief   HVAC Application integration interface.
 *
 * Provides the fixed application integration contract:
 *   - hvac_app_init()  : one-time initialization after platform startup
 *   - hvac_app_task()  : one processing cycle, called from the superloop
 */

#ifndef HVAC_APP_H
#define HVAC_APP_H

/**
 * @brief  Initialize the HVAC application.
 *         Calls system_init() to initialize all application modules.
 *         Must be called once after all HAL/CubeMX initialization is complete.
 */
void hvac_app_init(void);

/**
 * @brief  Execute one HVAC application processing cycle.
 *         Coordinates the runtime sequence:
 *           1. Process UART commands
 *           2. Update ADC feedback
 *           3. Execute flap control logic
 *         Returns to caller — contains no infinite loop.
 */
void hvac_app_task(void);

#endif /* HVAC_APP_H */
