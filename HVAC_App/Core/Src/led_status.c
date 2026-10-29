/**
 * @file    led_status.c
 * @brief   LED status handler for HVAC flap control
 *
 * Controls:
 *   PC0 — Power LED (always ON during runtime)
 *   PC1 — Position 1 LED
 *   PC2 — Position 2 LED
 *   PC3 — Position 3 LED
 *   PC4 — Position 4 LED
 *   PC5 — Position 5 LED
 *
 * Only one position LED is active at any time.
 * Position 0 (fully closed) has no dedicated LED.
 *
 * Traces to: SWE-REQ-035..039
 */

#include "led_status.h"
#include "main.h"

/* ---------------------------------------------------------------------------
 * LED pin definitions (from env-setup / .ioc)
 * --------------------------------------------------------------------------- */
#define POWER_LED_PORT       GPIOC
#define POWER_LED_PIN        GPIO_PIN_0

#define POS_LED_PORT         GPIOC

/* Position LED pins indexed 0..4 → PC1..PC5 */
static const uint16_t pos_led_pins[5] =
{
    GPIO_PIN_1,  /* Position 1 */
    GPIO_PIN_2,  /* Position 2 */
    GPIO_PIN_3,  /* Position 3 */
    GPIO_PIN_4,  /* Position 4 */
    GPIO_PIN_5   /* Position 5 */
};

#define POS_LED_COUNT  ((uint8_t)5U)

/* ---------------------------------------------------------------------------
 * Module-static data
 * --------------------------------------------------------------------------- */
static uint8_t current_led_state;  /* Last indicated position (0 = none) */

/* ---------------------------------------------------------------------------
 * Private helpers
 * --------------------------------------------------------------------------- */

/** Turn OFF all position LEDs (PC1-PC5). */
static void clear_all_position_leds(void)
{
    uint8_t i;
    for (i = 0U; i < POS_LED_COUNT; i++)
    {
        HAL_GPIO_WritePin(POS_LED_PORT, pos_led_pins[i], GPIO_PIN_RESET);
    }
}

/* ---------------------------------------------------------------------------
 * Public API
 * --------------------------------------------------------------------------- */

void led_status_init(void)
{
    current_led_state = 0U;

    /* Power LED ON */
    HAL_GPIO_WritePin(POWER_LED_PORT, POWER_LED_PIN, GPIO_PIN_SET);

    /* All position LEDs OFF */
    clear_all_position_leds();
}

void led_status_update(uint8_t position, uint8_t feedback_valid)
{
    /* If feedback is invalid, turn off all position LEDs */
    if (feedback_valid == 0U)
    {
        clear_all_position_leds();
        current_led_state = 0U;
        return;
    }

    /* Avoid redundant GPIO writes */
    if (position == current_led_state)
    {
        return;
    }

    /* Turn off all position LEDs first */
    clear_all_position_leds();

    /* Position 0 has no dedicated LED; positions 1-5 map to PC1-PC5 */
    if ((position >= 1U) && (position <= 5U))
    {
        HAL_GPIO_WritePin(POS_LED_PORT, pos_led_pins[position - 1U], GPIO_PIN_SET);
    }

    current_led_state = position;
}
