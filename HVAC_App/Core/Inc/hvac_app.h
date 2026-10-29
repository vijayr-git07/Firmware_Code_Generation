/**
 * @file    hvac_app.h
 * @brief   HVAC Flap Control - Application integration interface
 *
 * Provides the two fixed entry points that main.c calls:
 *   hvac_app_init()  — once after peripheral init
 *   hvac_app_task()  — once per superloop iteration
 */

#ifndef HVAC_APP_H
#define HVAC_APP_H

/* ---------------- Public API -------------------------------------- */

/**
 * @brief  Initialise the HVAC application.
 *
 * Delegates to system_init() which initialises every application
 * module and reads the initial flap position.
 *
 * Call once inside USER CODE BEGIN 2 in main().
 */
void hvac_app_init(void);

/**
 * @brief  Execute one HVAC application processing cycle.
 *
 * Sequence per iteration:
 *   1. command_parser_process()   — poll UART
 *   2. feedback_processor_update() — read ADC
 *   3. flap_control_process()      — control logic + motor + LEDs
 *
 * Returns to caller; contains no infinite loop.
 * Call inside USER CODE BEGIN 3 (while-1 body) in main().
 */
void hvac_app_task(void);

#endif /* HVAC_APP_H */
