/**
 * @file    motor_driver.c
 * @brief   HVAC Flap Control - Motor Driver implementation
 *
 * Hardware mapping (from CubeMX project):
 *   IN1 = PB0   IN2 = PB1
 *   Forward (open):  IN1=RESET, IN2=SET
 *   Reverse (close): IN1=SET,   IN2=RESET
 *   Stop:            IN1=RESET, IN2=RESET
 *   PWM: TIM3 CH1 (PC6, configured in MspPostInit)
 *
 * SWE-REQ: 030, 031, 032, 033, 034, 042, 043, 044, 054
 */

#include "motor_driver.h"
#include "main.h"

/* ---- Extern HAL handle (defined in main.c) ----------------------- */
extern TIM_HandleTypeDef htim3;

/* ---- Module-static data ------------------------------------------ */
static uint8_t motor_enabled_flag;
static uint8_t motor_direction;

/* ---- Internal helpers -------------------------------------------- */

static void set_direction_pins(uint8_t dir)
{
    switch (dir)
    {
        case MOTOR_DIR_FORWARD:
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);  /* IN1 */
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);    /* IN2 */
            break;

        case MOTOR_DIR_REVERSE:
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);    /* IN1 */
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);  /* IN2 */
            break;

        case MOTOR_DIR_STOP:
        default:
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);  /* IN1 */
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);  /* IN2 */
            break;
    }
}

/* ---- Public API -------------------------------------------------- */

void motor_driver_init(void)
{
    motor_enabled_flag = 0U;
    motor_direction    = MOTOR_DIR_STOP;

    /* Ensure safe state on startup (SWE-REQ-034). */
    set_direction_pins(MOTOR_DIR_STOP);
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
}

void motor_driver_set_direction(uint8_t direction)
{
    motor_direction = direction;
    set_direction_pins(direction);
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
    set_direction_pins(MOTOR_DIR_STOP);
    motor_enabled_flag = 0U;
    motor_direction    = MOTOR_DIR_STOP;
}
