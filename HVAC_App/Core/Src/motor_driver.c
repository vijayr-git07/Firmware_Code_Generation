/**
 * @file    motor_driver.c
 * @brief   Motor Driver module implementation for HVAC flap control.
 *
 * Controls the L298N H-bridge via:
 *   - PB0 (IN1) and PB1 (IN2) for direction
 *   - TIM3 CH1 PWM for motor enable/speed
 *
 * Direction truth table:
 *   Forward (open)  : IN1 = RESET, IN2 = SET
 *   Reverse (close) : IN1 = SET,   IN2 = RESET
 *   Stop            : IN1 = RESET, IN2 = RESET
 */

#include "motor_driver.h"
#include "main.h"

/* ---------------------------------------------------------------------------
 * External HAL handle (defined in main.c)
 * -----------------------------------------------------------------------*/
extern TIM_HandleTypeDef htim3;

/* ---------------------------------------------------------------------------
 * Module constants
 * -----------------------------------------------------------------------*/
#define MOTOR_IN1_PORT    GPIOB
#define MOTOR_IN1_PIN     GPIO_PIN_0
#define MOTOR_IN2_PORT    GPIOB
#define MOTOR_IN2_PIN     GPIO_PIN_1

/* ---------------------------------------------------------------------------
 * Module-static variables
 * -----------------------------------------------------------------------*/
static uint8_t motor_enabled_flag = 0U;
static uint8_t motor_direction    = MOTOR_DIR_FORWARD;

/* ---------------------------------------------------------------------------
 * Public functions
 * -----------------------------------------------------------------------*/

void motor_driver_init(void)
{
    /* Ensure safe stopped state */
    HAL_GPIO_WritePin(MOTOR_IN1_PORT, MOTOR_IN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN2_PORT, MOTOR_IN2_PIN, GPIO_PIN_RESET);
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);

    motor_enabled_flag = 0U;
    motor_direction    = MOTOR_DIR_FORWARD;
}

void motor_driver_set_direction(uint8_t direction)
{
    if (direction == MOTOR_DIR_FORWARD)
    {
        /* Forward (open): IN1 = RESET, IN2 = SET */
        HAL_GPIO_WritePin(MOTOR_IN1_PORT, MOTOR_IN1_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MOTOR_IN2_PORT, MOTOR_IN2_PIN, GPIO_PIN_SET);
        motor_direction = MOTOR_DIR_FORWARD;
    }
    else if (direction == MOTOR_DIR_REVERSE)
    {
        /* Reverse (close): IN1 = SET, IN2 = RESET */
        HAL_GPIO_WritePin(MOTOR_IN1_PORT, MOTOR_IN1_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MOTOR_IN2_PORT, MOTOR_IN2_PIN, GPIO_PIN_RESET);
        motor_direction = MOTOR_DIR_REVERSE;
    }
    else
    {
        /* Invalid direction — stop for safety */
        HAL_GPIO_WritePin(MOTOR_IN1_PORT, MOTOR_IN1_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MOTOR_IN2_PORT, MOTOR_IN2_PIN, GPIO_PIN_RESET);
    }
}

void motor_driver_enable(void)
{
    if (motor_enabled_flag == 0U)
    {
        HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
        motor_enabled_flag = 1U;
    }
}

void motor_driver_stop(void)
{
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
    HAL_GPIO_WritePin(MOTOR_IN1_PORT, MOTOR_IN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN2_PORT, MOTOR_IN2_PIN, GPIO_PIN_RESET);
    motor_enabled_flag = 0U;
}
