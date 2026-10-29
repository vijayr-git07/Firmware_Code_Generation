/**
 * @file    hvac_app.h
 * @brief   HVAC application integration interface
 *
 * Provides the fixed application integration contract:
 *   hvac_app_init()  — called once after platform startup
 *   hvac_app_task()  — called repeatedly from the superloop
 */

#ifndef HVAC_APP_H
#define HVAC_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  Initialize the HVAC application.
 *         Call once from main() after all peripheral initialization.
 */
void hvac_app_init(void);

/**
 * @brief  Execute one HVAC application processing cycle.
 *         Call from the main while(1) loop.
 *         Returns to caller; contains no infinite loop.
 */
void hvac_app_task(void);

#ifdef __cplusplus
}
#endif

#endif /* HVAC_APP_H */
