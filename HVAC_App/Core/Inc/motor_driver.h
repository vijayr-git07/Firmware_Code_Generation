/**
 * @file    motor_driver.h
 * @brief   Motor driver interface for HVAC flap DC motor.
 *
 * Controls the L298N H-bridge via PB0 (IN1), PB1 (IN2) for direction
 * and TIM3 CH1 PWM on PC6 for enable/speed.
 */

#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <stdint.h>

/** Motor direction enumeration. */
typedef enum
{
    MOTOR_DIR_STOP    = 0U,
    MOTOR_DIR_FORWARD = 1U,   /**< Open direction  (IN1=RESET, IN2=SET)  */
    MOTOR_DIR_REVERSE = 2U    /**< Close direction (IN1=SET,   IN2=RESET) */
} motor_direction_t;

/**
 * @brief  Initialize the motor driver to safe-off state.
 */
void motor_driver_init(void);

/**
 * @brief  Set motor direction GPIO outputs.
 * @param  dir  Desired direction.
 */
void motor_driver_set_direction(motor_direction_t dir);

/**
 * @brief  Enable motor by starting PWM output.
 */
void motor_driver_enable(void);

/**
 * @brief  Stop motor: disable PWM and set direction GPIOs to safe state.
 */
void motor_driver_stop(void);

#endif /* MOTOR_DRIVER_H */
