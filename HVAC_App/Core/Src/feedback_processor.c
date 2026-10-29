/**
 * @file    feedback_processor.c
 * @brief   ADC feedback processor for HVAC flap control
 *
 * Acquires potentiometer feedback via ADC1 CH0 (PA0), validates the reading,
 * and maps it to a logical flap position using coarse and precise tables.
 *
 * Traces to: SWE-REQ-024..029, SWE-REQ-052
 */

#include "feedback_processor.h"
#include "main.h"

/* ---------------------------------------------------------------------------
 * External HAL handle (defined in main.c)
 * --------------------------------------------------------------------------- */
extern ADC_HandleTypeDef hadc1;

/* ---------------------------------------------------------------------------
 * ADC conversion constants
 * --------------------------------------------------------------------------- */
#define ADC_POLL_TIMEOUT_MS  ((uint32_t)10U)
#define ADC_MAX_VALUE        ((uint16_t)4095U)

/* ---------------------------------------------------------------------------
 * Coarse position detection table (Section 10.1)
 * Evaluated top-down; first match wins.
 * --------------------------------------------------------------------------- */
typedef struct
{
    uint16_t adc_min;
    uint16_t adc_max;
    uint8_t  position;
} CoarseEntry_t;

static const CoarseEntry_t coarse_table[FLAP_POSITION_COUNT] =
{
    { 4001U, 4095U, 0U },   /* Position 0: > 4000        */
    { 3750U, 4000U, 1U },   /* Position 1: 3750 – 4000   */
    { 3480U, 3749U, 2U },   /* Position 2: 3480 – 3750   */
    { 2500U, 3479U, 3U },   /* Position 3: 2500 – 3480   */
    { 1000U, 2499U, 4U },   /* Position 4: 1000 – 2500   */
    {    0U,  999U, 5U }    /* Position 5: 0    – 1000   */
};

/* ---------------------------------------------------------------------------
 * Precise stop detection table (Section 10.2)
 * --------------------------------------------------------------------------- */
typedef struct
{
    uint16_t adc_min;
    uint16_t adc_max;
} PreciseEntry_t;

static const PreciseEntry_t precise_table[FLAP_POSITION_COUNT] =
{
    { 4055U, 4065U },  /* Position 0 */
    { 3837U, 3857U },  /* Position 1 */
    { 3563U, 3593U },  /* Position 2 */
    { 3309U, 3329U },  /* Position 3 */
    { 2022U, 2042U },  /* Position 4 */
    {  315U,  335U }   /* Position 5 */
};

/* ---------------------------------------------------------------------------
 * Module-static data
 * --------------------------------------------------------------------------- */
static uint16_t latest_adc_value;
static uint8_t  current_flap_position;
static uint8_t  feedback_valid_flag;

/* ---------------------------------------------------------------------------
 * Private helpers
 * --------------------------------------------------------------------------- */

/**
 * @brief  Map a raw ADC value to a coarse logical position.
 * @param  adc_val  Raw 12-bit ADC reading.
 * @param  p_pos    Output position (0-5).
 * @return 1 if a valid mapping was found, 0 otherwise.
 */
static uint8_t map_adc_to_position(uint16_t adc_val, uint8_t *p_pos)
{
    uint8_t i;
    for (i = 0U; i < FLAP_POSITION_COUNT; i++)
    {
        if ((adc_val >= coarse_table[i].adc_min) &&
            (adc_val <= coarse_table[i].adc_max))
        {
            *p_pos = coarse_table[i].position;
            return 1U;
        }
    }
    return 0U;  /* ADC value outside all coarse ranges */
}

/* ---------------------------------------------------------------------------
 * Public API
 * --------------------------------------------------------------------------- */

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

    /* Wait for conversion */
    status = HAL_ADC_PollForConversion(&hadc1, ADC_POLL_TIMEOUT_MS);
    if (status != HAL_OK)
    {
        HAL_ADC_Stop(&hadc1);
        feedback_valid_flag = 0U;
        return;
    }

    adc_raw = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);

    /* Clamp to 12-bit range */
    if (adc_raw > (uint32_t)ADC_MAX_VALUE)
    {
        feedback_valid_flag = 0U;
        return;
    }

    latest_adc_value = (uint16_t)adc_raw;

    /* Map to coarse position */
    if (map_adc_to_position(latest_adc_value, &mapped_pos) != 0U)
    {
        current_flap_position = mapped_pos;
        feedback_valid_flag   = 1U;
    }
    else
    {
        feedback_valid_flag = 0U;  /* Invalid / out-of-range */
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

uint8_t feedback_processor_in_stop_window(uint8_t target_position)
{
    if (target_position >= FLAP_POSITION_COUNT)
    {
        return 0U;
    }

    if ((latest_adc_value >= precise_table[target_position].adc_min) &&
        (latest_adc_value <= precise_table[target_position].adc_max))
    {
        return 1U;
    }

    return 0U;
}
