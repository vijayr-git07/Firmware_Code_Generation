/**
 * @file    led_status.c
 * @brief   LED status handler implementation.
 */

#include "led_status.h"
#include "main.h"

/* --------------- GPIO mapping ------------------------------------------ */
/* Power LED  : PC0
 * Position 1 : PC1
 * Position 2 : PC2
 * Position 3 : PC3
 * Position 4 : PC4
 * Position 5 : PC5
 */
#define LED_PORT             GPIOC
#define LED_POWER_PIN        GPIO_PIN_0
#define LED_POS1_PIN         GPIO_PIN_1
#define LED_POS2_PIN         GPIO_PIN_2
#define LED_POS3_PIN         GPIO_PIN_3
#define LED_POS4_PIN         GPIO_PIN_4
#define LED_POS5_PIN         GPIO_PIN_5

#define NUM_POSITION_LEDS    5U

/* Lookup table: index 0 = position 1 LED pin, ... index 4 = position 5 */
static const uint16_t position_led_pins[NUM_POSITION_LEDS] =
{
    LED_POS1_PIN,
    LED_POS2_PIN,
    LED_POS3_PIN,
    LED_POS4_PIN,
    LED_POS5_PIN
};

/* --------------- module-static state ----------------------------------- */
static uint8_t current_led_state;

/* --------------- internal helper --------------------------------------- */
static void all_position_leds_off(void)
{
    uint8_t i;
    for (i = 0U; i < NUM_POSITION_LEDS; i++)
    {
        HAL_GPIO_WritePin(LED_PORT, position_led_pins[i], GPIO_PIN_RESET);
    }
}

/* ======================================================================= */
void led_status_init(void)
{
    /* Power LED ON */
    HAL_GPIO_WritePin(LED_PORT, LED_POWER_PIN, GPIO_PIN_SET);

    /* All position LEDs OFF */
    all_position_leds_off();

    current_led_state = 0U;
}

/* ======================================================================= */
void led_status_update(uint8_t position)
{
    if (position > 5U)
    {
        return;  /* out of range – preserve current state */
    }

    if (position == current_led_state)
    {
        return;  /* no change needed */
    }

    /* Turn off all position LEDs first */
    all_position_leds_off();

    /* Turn on the LED for the new position (1..5) */
    if ((position >= 1U) && (position <= 5U))
    {
        HAL_GPIO_WritePin(LED_PORT, position_led_pins[position - 1U], GPIO_PIN_SET);
    }
    /* Position 0: no dedicated LED, all remain off */

    current_led_state = position;
}
