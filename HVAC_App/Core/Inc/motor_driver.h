/**
 * @file    motor_driver.h
 * @brief   Motor driver interface for HVAC flap control
 */

#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/** Motor direction enumeration. */
#define MOTOR_DIR_STOP      ((uint8_t)0U)
#define MOTOR_DIR_FORWARD   ((uint8_t)1U)   /* Open direction  */
#define MOTOR_DIR_REVERSE   ((uint8_t)2U)   /* Close direction */

/**
 * @brief  Initialize the motor driver to safe OFF state.
 */
void motor_driver_init(void);

/**
 * @brief  Set motor direction.
 * @param  direction  MOTOR_DIR_FORWARD, MOTOR_DIR_REVERSE, or MOTOR_DIR_STOP.
 */
void motor_driver_set_direction(uint8_t direction);

/**
 * @brief  Enable motor output (start PWM).
 */
void motor_driver_enable(void);

/**
 * @brief  Stop motor: disable PWM and set direction GPIOs to safe state.
 */
void motor_driver_stop(void);

#ifdef __cplusplus
}
#endif

#endif /* MOTOR_DRIVER_H */
