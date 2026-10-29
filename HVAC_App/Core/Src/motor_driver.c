/**
 * @file    motor_driver.c
 * @brief   Motor driver implementation for HVAC flap DC motor.
 */

#include "motor_driver.h"
#include "main.h"

/* --------------- external HAL handle (defined in main.c) --------------- */
extern TIM_HandleTypeDef htim3;

/* --------------- GPIO mapping ------------------------------------------ */
/* IN1 = PB0,  IN2 = PB1  (L298N direction inputs)                       */
#define MOTOR_IN1_PORT   GPIOB
#define MOTOR_IN1_PIN    GPIO_PIN_0
#define MOTOR_IN2_PORT   GPIOB
#define MOTOR_IN2_PIN    GPIO_PIN_1

/* --------------- module-static state ----------------------------------- */
static uint8_t           motor_enabled_flag;
static motor_direction_t motor_direction;

/* ======================================================================= */
void motor_driver_init(void)
{
    motor_enabled_flag = 0U;
    motor_direction    = MOTOR_DIR_STOP;

    /* Ensure safe-off state */
    HAL_GPIO_WritePin(MOTOR_IN1_PORT, MOTOR_IN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN2_PORT, MOTOR_IN2_PIN, GPIO_PIN_RESET);
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
}

/* ======================================================================= */
void motor_driver_set_direction(motor_direction_t dir)
{
    motor_direction = dir;

    switch (dir)
    {
        case MOTOR_DIR_FORWARD:
            /* Forward (open): IN1 = RESET, IN2 = SET */
            HAL_GPIO_WritePin(MOTOR_IN1_PORT, MOTOR_IN1_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(MOTOR_IN2_PORT, MOTOR_IN2_PIN, GPIO_PIN_SET);
            break;

        case MOTOR_DIR_REVERSE:
            /* Reverse (close): IN1 = SET, IN2 = RESET */
            HAL_GPIO_WritePin(MOTOR_IN1_PORT, MOTOR_IN1_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(MOTOR_IN2_PORT, MOTOR_IN2_PIN, GPIO_PIN_RESET);
            break;

        case MOTOR_DIR_STOP:
        default:
            /* Stop: IN1 = RESET, IN2 = RESET */
            HAL_GPIO_WritePin(MOTOR_IN1_PORT, MOTOR_IN1_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(MOTOR_IN2_PORT, MOTOR_IN2_PIN, GPIO_PIN_RESET);
            break;
    }
}

/* ======================================================================= */
void motor_driver_enable(void)
{
    if (motor_enabled_flag == 0U)
    {
        HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
        motor_enabled_flag = 1U;
    }
}

/* ======================================================================= */
void motor_driver_stop(void)
{
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
    motor_enabled_flag = 0U;

    /* Safe-off direction GPIOs */
    HAL_GPIO_WritePin(MOTOR_IN1_PORT, MOTOR_IN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN2_PORT, MOTOR_IN2_PIN, GPIO_PIN_RESET);
    motor_direction = MOTOR_DIR_STOP;
}
