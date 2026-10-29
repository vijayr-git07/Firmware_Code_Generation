/**
 * @file    led_status.c
 * @brief   HVAC Flap Control - LED Status Handler implementation
 *
 * Pin mapping (from CubeMX MX_GPIO_Init):
 *   PC0 = Power LED
 *   PC1 = Position 1 LED
 *   PC2 = Position 2 LED
 *   PC3 = Position 3 LED
 *   PC4 = Position 4 LED
 *   PC5 = Position 5 LED
 *
 * SWE-REQ: 035, 036, 037, 038, 039, 044, 054
 */

#include "led_status.h"
#include "main.h"

/* ---- Position LED pin look-up table ------------------------------ */

/** GPIO pins for position LEDs 1..5 (index 0 unused). */
static const uint16_t position_led_pins[6] =
{
    0U,             /* position 0 — no dedicated LED */
    GPIO_PIN_1,     /* position 1 — PC1 */
    GPIO_PIN_2,     /* position 2 — PC2 */
    GPIO_PIN_3,     /* position 3 — PC3 */
    GPIO_PIN_4,     /* position 4 — PC4 */
    GPIO_PIN_5      /* position 5 — PC5 */
};

/* ---- Module-static data ------------------------------------------ */
static uint8_t current_led_state;

/* ---- Internal helpers -------------------------------------------- */

/** Turn OFF all position LEDs (PC1 .. PC5). */
static void all_position_leds_off(void)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
}

/* ---- Public API -------------------------------------------------- */

void led_status_init(void)
{
    current_led_state = 0U;

    /* Power LED ON (SWE-REQ-035). */
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);

    /* All position LEDs OFF. */
    all_position_leds_off();
}

void led_status_update(uint8_t position)
{
    /* Turn off all position LEDs first (one-active rule, SWE-REQ-037). */
    all_position_leds_off();

    if ((position >= 1U) && (position <= 5U))
    {
        HAL_GPIO_WritePin(GPIOC, position_led_pins[position], GPIO_PIN_SET);
        current_led_state = position;
    }
    else
    {
        /* Position 0 or invalid — all position LEDs remain OFF. */
        current_led_state = 0U;
    }

    /* Power LED stays ON always (SWE-REQ-035). */
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
}
