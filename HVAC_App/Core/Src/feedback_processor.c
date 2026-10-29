/**
 * @file    feedback_processor.c
 * @brief   Feedback Processor module implementation for HVAC flap control.
 *
 * Acquires ADC1 CH0 samples via polling, validates the reading against
 * the coarse position table, and maps it to a logical flap position.
 */

#include "feedback_processor.h"
#include "main.h"

/* ---------------------------------------------------------------------------
 * External HAL handle (defined in main.c)
 * -----------------------------------------------------------------------*/
extern ADC_HandleTypeDef hadc1;

/* ---------------------------------------------------------------------------
 * Module constants — Coarse Position Detection Table
 *
 * Position 0 : ADC > 4000
 * Position 1 : 3750 .. 4000
 * Position 2 : 3480 .. 3750
 * Position 3 : 2500 .. 3480
 * Position 4 : 1000 .. 2500
 * Position 5 : 0    .. 1000
 * -----------------------------------------------------------------------*/
#define COARSE_POS0_MIN   4001U
#define COARSE_POS0_MAX   4095U
#define COARSE_POS1_MIN   3750U
#define COARSE_POS1_MAX   4000U
#define COARSE_POS2_MIN   3480U
#define COARSE_POS2_MAX   3749U
#define COARSE_POS3_MIN   2500U
#define COARSE_POS3_MAX   3479U
#define COARSE_POS4_MIN   1000U
#define COARSE_POS4_MAX   2499U
#define COARSE_POS5_MIN      0U
#define COARSE_POS5_MAX    999U

#define ADC_MAX_VALUE     4095U
#define ADC_POLL_TIMEOUT  ((uint32_t)10U)  /* ms */

#define NUM_POSITIONS     6U
#define POSITION_INVALID  0xFFU

/* Coarse boundary table: [position][0]=min, [position][1]=max */
static const uint16_t coarse_table[NUM_POSITIONS][2] = {
    { COARSE_POS0_MIN, COARSE_POS0_MAX },  /* Position 0 */
    { COARSE_POS1_MIN, COARSE_POS1_MAX },  /* Position 1 */
    { COARSE_POS2_MIN, COARSE_POS2_MAX },  /* Position 2 */
    { COARSE_POS3_MIN, COARSE_POS3_MAX },  /* Position 3 */
    { COARSE_POS4_MIN, COARSE_POS4_MAX },  /* Position 4 */
    { COARSE_POS5_MIN, COARSE_POS5_MAX },  /* Position 5 */
};

/* ---------------------------------------------------------------------------
 * Module-static variables
 * -----------------------------------------------------------------------*/
static uint16_t latest_adc_value      = 0U;
static uint8_t  current_flap_position = 0U;
static uint8_t  feedback_valid_flag   = 0U;

/* ---------------------------------------------------------------------------
 * Private helpers
 * -----------------------------------------------------------------------*/

/**
 * @brief  Map a raw ADC value to a logical flap position using the coarse table.
 * @param  adc_val  12-bit ADC reading.
 * @return Logical position 0-5, or POSITION_INVALID if out of range.
 */
static uint8_t map_adc_to_position(uint16_t adc_val)
{
    uint8_t pos;

    for (pos = 0U; pos < NUM_POSITIONS; pos++)
    {
        if ((adc_val >= coarse_table[pos][0]) && (adc_val <= coarse_table[pos][1]))
        {
            return pos;
        }
    }

    return POSITION_INVALID;
}

/* ---------------------------------------------------------------------------
 * Public functions
 * -----------------------------------------------------------------------*/

void feedback_processor_init(void)
{
    latest_adc_value      = 0U;
    current_flap_position = 0U;
    feedback_valid_flag   = 0U;
}

void feedback_processor_update(void)
{
    HAL_StatusTypeDef status;
    uint32_t adc_raw;
    uint8_t  mapped_pos;

    /* Start single conversion */
    status = HAL_ADC_Start(&hadc1);
    if (status != HAL_OK)
    {
        feedback_valid_flag = 0U;
        return;
    }

    /* Wait for conversion to complete */
    status = HAL_ADC_PollForConversion(&hadc1, ADC_POLL_TIMEOUT);
    if (status != HAL_OK)
    {
        HAL_ADC_Stop(&hadc1);
        feedback_valid_flag = 0U;
        return;
    }

    /* Read converted value */
    adc_raw = HAL_ADC_GetValue(&hadc1);

    /* Stop ADC (single conversion mode) */
    HAL_ADC_Stop(&hadc1);

    /* Validate range */
    if (adc_raw > ADC_MAX_VALUE)
    {
        feedback_valid_flag = 0U;
        return;
    }

    latest_adc_value = (uint16_t)adc_raw;

    /* Map to logical position */
    mapped_pos = map_adc_to_position(latest_adc_value);

    if (mapped_pos != POSITION_INVALID)
    {
        current_flap_position = mapped_pos;
        feedback_valid_flag   = 1U;
    }
    else
    {
        /* ADC value falls outside all coarse ranges — invalid */
        feedback_valid_flag = 0U;
    }
}

uint8_t feedback_processor_get_position(void)
{
    return current_flap_position;
}

uint8_t feedback_processor_is_valid(void)
{
    return feedback_valid_flag;
}

uint16_t feedback_processor_get_raw_adc(void)
{
    return latest_adc_value;
}
