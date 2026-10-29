/**
 * @file    feedback_processor.h
 * @brief   ADC feedback processor for HVAC flap potentiometer.
 *
 * Acquires ADC1 CH0 samples, validates them against the coarse
 * position table, maps them to logical positions 0-5, and exposes
 * the raw ADC value for precise stop detection.
 */

#ifndef FEEDBACK_PROCESSOR_H
#define FEEDBACK_PROCESSOR_H

#include <stdint.h>

/**
 * @brief  Initialize the feedback processor module.
 */
void feedback_processor_init(void);

/**
 * @brief  Acquire a new ADC sample and update internal state.
 *
 * Starts a single conversion on ADC1, polls for completion,
 * validates the result, and maps it to a coarse flap position.
 */
void feedback_processor_update(void);

/**
 * @brief  Get the current coarse flap position (0 .. 5).
 *
 * @return  Current logical position derived from the coarse table.
 */
uint8_t feedback_processor_get_position(void);

/**
 * @brief  Check whether the latest feedback reading is valid.
 *
 * @return  1 if valid, 0 if the last ADC value was out of range.
 */
uint8_t feedback_processor_is_valid(void);

/**
 * @brief  Get the latest raw 12-bit ADC value.
 *
 * @return  Raw ADC reading (0 .. 4095), or 0xFFFF if invalid.
 */
uint16_t feedback_processor_get_raw_adc(void);

#endif /* FEEDBACK_PROCESSOR_H */
