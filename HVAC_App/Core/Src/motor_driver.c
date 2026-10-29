/**
 * @file    motor_driver.c
 * @brief   Motor driver for HVAC flap control
 *
 * Controls the L298N H-Bridge via:
 *   - PB0 (IN1) and PB1 (IN2) for direction
 *   - TIM3 CH1 PWM for motor enable/speed
 *
 * Direction truth table (from env-setup):
 *   Forward (open) : IN1=RESET, IN2=SET
 *   Reverse (close): IN1=SET,   IN2=RESET
 *   Stop           : IN1=RESET, IN2=RESET
 *
 * Traces to: SWE-REQ-030..034
 */

#include "motor_driver.h"
#include "main.h"

/* ---------------------------------------------------------------------------
 * External HAL handle (defined in main.c)
 * --------------------------------------------------------------------------- */
extern TIM_HandleTypeDef htim3;

/* ---------------------------------------------------------------------------
 * GPIO pin definitions for motor direction (from .ioc / env-setup)
 * PB0 = IN1, PB1 = IN2
 * --------------------------------------------------------------------------- */
#define MOTOR_IN1_PORT   GPIOB
#define MOTOR_IN1_PIN    GPIO_PIN_0
#define MOTOR_IN2_PORT   GPIOB
#define MOTOR_IN2_PIN    GPIO_PIN_1

/* ---------------------------------------------------------------------------
 * Module-static data
 * --------------------------------------------------------------------------- */
static uint8_t motor_enabled_flag;
static uint8_t motor_direction;

/* ---------------------------------------------------------------------------
 * Public API
 * --------------------------------------------------------------------------- */

void motor_driver_init(void)
{
    motor_enabled_flag = 0U;
    motor_direction    = MOTOR_DIR_STOP;

    /* Ensure safe state: direction GPIOs LOW, PWM stopped */
    HAL_GPIO_WritePin(MOTOR_IN1_PORT, MOTOR_IN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN2_PORT, MOTOR_IN2_PIN, GPIO_PIN_RESET);
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
}

void motor_driver_set_direction(uint8_t direction)
{
    motor_direction = direction;

    switch (direction)
    {
        case MOTOR_DIR_FORWARD:
            /* Forward (open): IN1=RESET, IN2=SET */
            HAL_GPIO_WritePin(MOTOR_IN1_PORT, MOTOR_IN1_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(MOTOR_IN2_PORT, MOTOR_IN2_PIN, GPIO_PIN_SET);
            break;

        case MOTOR_DIR_REVERSE:
            /* Reverse (close): IN1=SET, IN2=RESET */
            HAL_GPIO_WritePin(MOTOR_IN1_PORT, MOTOR_IN1_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(MOTOR_IN2_PORT, MOTOR_IN2_PIN, GPIO_PIN_RESET);
            break;

        case MOTOR_DIR_STOP:
        default:
            /* Stop: IN1=RESET, IN2=RESET */
            HAL_GPIO_WritePin(MOTOR_IN1_PORT, MOTOR_IN1_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(MOTOR_IN2_PORT, MOTOR_IN2_PIN, GPIO_PIN_RESET);
            break;
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
    motor_enabled_flag = 0U;

    /* Safe state: both direction pins LOW */
    motor_direction = MOTOR_DIR_STOP;
    HAL_GPIO_WritePin(MOTOR_IN1_PORT, MOTOR_IN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN2_PORT, MOTOR_IN2_PIN, GPIO_PIN_RESET);
}
