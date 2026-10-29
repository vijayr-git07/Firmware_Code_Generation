/**
 * @file    feedback_processor.h
 * @brief   ADC feedback processor interface for HVAC flap control
 */

#ifndef FEEDBACK_PROCESSOR_H
#define FEEDBACK_PROCESSOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/** Total number of supported flap positions (0-5). */
#define FLAP_POSITION_COUNT  6U

/**
 * @brief  Initialize the feedback processor module.
 */
void feedback_processor_init(void);

/**
 * @brief  Acquire and process a new ADC sample. Call periodically.
 */
void feedback_processor_update(void);

/**
 * @brief  Get the current coarse flap position (0-5).
 * @return Current logical flap position.
 */
uint8_t feedback_processor_get_position(void);

/**
 * @brief  Check whether the latest feedback is valid.
 * @return 1 if valid, 0 if invalid.
 */
uint8_t feedback_processor_is_valid(void);

/**
 * @brief  Get the latest raw ADC value.
 * @return 12-bit ADC reading (0-4095).
 */
uint16_t feedback_processor_get_raw_adc(void);

/**
 * @brief  Check if the raw ADC value is within the precise stop window
 *         for the given target position.
 * @param  target_position  Target flap position (0-5).
 * @return 1 if within the precise stop window, 0 otherwise.
 */
uint8_t feedback_processor_in_stop_window(uint8_t target_position);

#ifdef __cplusplus
}
#endif

#endif /* FEEDBACK_PROCESSOR_H */
