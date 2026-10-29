/**
 * @file    motor_driver.h
 * @brief   Motor Driver module interface for HVAC flap control.
 *
 * Controls DC motor direction via GPIO (PB0/PB1) and PWM enable
 * via TIM3 CH1.
 */

#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <stdint.h>

/** Motor direction constants */
#define MOTOR_DIR_FORWARD   ((uint8_t)0U)  /* Open direction  : IN1=RESET, IN2=SET   */
#define MOTOR_DIR_REVERSE   ((uint8_t)1U)  /* Close direction : IN1=SET,   IN2=RESET */

/**
 * @brief  Initialize the motor driver module.
 *         Sets motor to safe stopped state (IN1=LOW, IN2=LOW, PWM stopped).
 */
void motor_driver_init(void);

/**
 * @brief  Set the motor direction.
 * @param  direction  MOTOR_DIR_FORWARD or MOTOR_DIR_REVERSE.
 */
void motor_driver_set_direction(uint8_t direction);

/**
 * @brief  Enable motor actuation (start PWM output).
 */
void motor_driver_enable(void);

/**
 * @brief  Stop the motor (disable PWM, set direction GPIOs low).
 */
void motor_driver_stop(void);

#endif /* MOTOR_DRIVER_H */
