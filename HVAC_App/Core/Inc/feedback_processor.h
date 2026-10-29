/**
 * @file    feedback_processor.h
 * @brief   Feedback Processor module interface for HVAC flap control.
 *
 * Acquires ADC feedback from the potentiometer, validates the reading,
 * and maps it to a logical flap position (0-5).
 */

#ifndef FEEDBACK_PROCESSOR_H
#define FEEDBACK_PROCESSOR_H

#include <stdint.h>

/**
 * @brief  Initialize the feedback processor module.
 *         Clears internal state.
 */
void feedback_processor_init(void);

/**
 * @brief  Acquire a new ADC sample and update the current flap position.
 *         Uses single-conversion polling mode on ADC1 CH0.
 */
void feedback_processor_update(void);

/**
 * @brief  Get the current logical flap position.
 * @return Logical flap position (0-5).  Returns last valid position
 *         if feedback is currently invalid.
 */
uint8_t feedback_processor_get_position(void);

/**
 * @brief  Check whether the latest feedback reading is valid.
 * @return 1 if valid, 0 if invalid.
 */
uint8_t feedback_processor_is_valid(void);

/**
 * @brief  Get the latest raw ADC value.
 * @return 12-bit ADC value (0-4095).
 */
uint16_t feedback_processor_get_raw_adc(void);

#endif /* FEEDBACK_PROCESSOR_H */
