/**
 * @file    led_status.h
 * @brief   LED status handler for HVAC flap position indication.
 *
 * Controls the power LED (PC0) and five position LEDs (PC1 .. PC5).
 * Only one position LED is active at a time; position 0 has no
 * dedicated LED so all position LEDs are off.
 */

#ifndef LED_STATUS_H
#define LED_STATUS_H

#include <stdint.h>

/**
 * @brief  Initialize LED status handler.
 *
 * Turns the power LED ON and all position LEDs OFF.
 */
void led_status_init(void);

/**
 * @brief  Update position LEDs to reflect the given flap position.
 *
 * @param  position  Logical flap position (0 .. 5).
 *                   Position 0 turns all position LEDs off.
 *                   Positions 1-5 light the corresponding LED.
 */
void led_status_update(uint8_t position);

#endif /* LED_STATUS_H */
