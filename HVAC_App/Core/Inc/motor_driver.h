/**
 * @file    motor_driver.h
 * @brief   HVAC Flap Control - Motor Driver interface
 *
 * Controls the DC motor via L298N H-bridge:
 *   - Direction: GPIOB PB0 (IN1), PB1 (IN2)
 *   - Enable/speed: TIM3 CH1 PWM
 *
 * SWE-REQ: 030, 031, 032, 033, 034
 */

#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <stdint.h>

/* ---------------- Direction Enumeration --------------------------- */

/** Motor direction identifiers. */
#define MOTOR_DIR_STOP      ((uint8_t)0U)
#define MOTOR_DIR_FORWARD   ((uint8_t)1U)   /* open  — IN1=RESET, IN2=SET  */
#define MOTOR_DIR_REVERSE   ((uint8_t)2U)   /* close — IN1=SET,   IN2=RESET */

/* ---------------- Public API -------------------------------------- */

/**
 * @brief  Initialise the motor driver to safe-off state.
 */
void motor_driver_init(void);

/**
 * @brief  Set motor direction (does not start PWM).
 * @param  direction  MOTOR_DIR_FORWARD, MOTOR_DIR_REVERSE, or MOTOR_DIR_STOP.
 */
void motor_driver_set_direction(uint8_t direction);

/**
 * @brief  Enable motor output (start PWM).
 */
void motor_driver_enable(void);

/**
 * @brief  Stop motor immediately (PWM off, direction pins low).
 */
void motor_driver_stop(void);

#endif /* MOTOR_DRIVER_H */
