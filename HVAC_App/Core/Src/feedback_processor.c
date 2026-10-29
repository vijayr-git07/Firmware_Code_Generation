/**
 * @file    feedback_processor.c
 * @brief   ADC feedback processor implementation.
 */

#include "feedback_processor.h"
#include "main.h"

/* --------------- external HAL handle (defined in main.c) --------------- */
extern ADC_HandleTypeDef hadc1;

/* --------------- module constants -------------------------------------- */
#define ADC_POLL_TIMEOUT_MS   10U
#define ADC_MAX_VALID         4095U
#define NUM_POSITIONS         6U
#define ADC_INVALID_RAW       0xFFFFU

/* Coarse position detection table (descending ADC order).
 * Each entry: { lower_bound, upper_bound, position }.
 * Position 0: ADC > 4000  => we use 4001..4095
 * Position 1: 3750..4000
 * Position 2: 3480..3750  (upper bound inclusive at boundary)
 * Position 3: 2500..3480
 * Position 4: 1000..2500
 * Position 5: 0..1000
 */
typedef struct
{
    uint16_t adc_min;
    uint16_t adc_max;
    uint8_t  position;
} coarse_entry_t;

static const coarse_entry_t coarse_table[NUM_POSITIONS] =
{
    { 4001U, 4095U, 0U },
    { 3750U, 4000U, 1U },
    { 3480U, 3749U, 2U },
    { 2500U, 3479U, 3U },
    { 1000U, 2499U, 4U },
    {    0U,  999U, 5U }
};

/* --------------- module-static state ----------------------------------- */
static uint16_t latest_adc_value;
static uint8_t  current_flap_position;
static uint8_t  feedback_valid_flag;

/* ======================================================================= */
void feedback_processor_init(void)
{
    latest_adc_value     = ADC_INVALID_RAW;
    current_flap_position = 0U;
    feedback_valid_flag   = 0U;
}

/* ======================================================================= */
void feedback_processor_update(void)
{
    HAL_StatusTypeDef status;
    uint32_t adc_raw;

    /* Start single conversion */
    status = HAL_ADC_Start(&hadc1);
    if (status != HAL_OK)
    {
        feedback_valid_flag = 0U;
        latest_adc_value    = ADC_INVALID_RAW;
        return;
    }

    /* Poll for conversion complete */
    status = HAL_ADC_PollForConversion(&hadc1, ADC_POLL_TIMEOUT_MS);
    if (status != HAL_OK)
    {
        HAL_ADC_Stop(&hadc1);
        feedback_valid_flag = 0U;
        latest_adc_value    = ADC_INVALID_RAW;
        return;
    }

    adc_raw = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);

    if (adc_raw > ADC_MAX_VALID)
    {
        feedback_valid_flag = 0U;
        latest_adc_value    = ADC_INVALID_RAW;
        return;
    }

    latest_adc_value = (uint16_t)adc_raw;

    /* Map through coarse table */
    uint8_t mapped = 0U;
    uint8_t i;
    for (i = 0U; i < NUM_POSITIONS; i++)
    {
        if ((latest_adc_value >= coarse_table[i].adc_min) &&
            (latest_adc_value <= coarse_table[i].adc_max))
        {
            current_flap_position = coarse_table[i].position;
            mapped = 1U;
            break;
        }
    }

    if (mapped != 0U)
    {
        feedback_valid_flag = 1U;
    }
    else
    {
        /* ADC value falls outside all coarse ranges – invalid */
        feedback_valid_flag = 0U;
    }
}

/* ======================================================================= */
uint8_t feedback_processor_get_position(void)
{
    return current_flap_position;
}

/* ======================================================================= */
uint8_t feedback_processor_is_valid(void)
{
    return feedback_valid_flag;
}

/* ======================================================================= */
uint16_t feedback_processor_get_raw_adc(void)
{
    return latest_adc_value;
}
