/**
 * @file    feedback_processor.c
 * @brief   HVAC Flap Control - Feedback Processor implementation
 *
 * SWE-REQ: 024, 025, 026, 027, 028, 029, 041, 044, 052, 054
 */

#include "feedback_processor.h"
#include "main.h"

/* ---- Extern HAL handle (defined in main.c) ----------------------- */
extern ADC_HandleTypeDef hadc1;

/* ---- Coarse position detection table ----------------------------- */
typedef struct
{
    uint16_t adc_min;
    uint16_t adc_max;
} adc_range_t;

/**
 * Coarse table — evaluated top-down; first match wins.
 * Position 0: > 4000  =>  4001 .. 4095
 * Position 1: 3750 .. 4000
 * Position 2: 3480 .. 3750  (upper bound inclusive at boundary)
 * Position 3: 2500 .. 3480
 * Position 4: 1000 .. 2500
 * Position 5:    0 .. 1000
 *
 * Boundary note: ranges are inclusive on both ends; the table is
 * ordered from position 0 (highest ADC) to position 5 (lowest ADC).
 * Overlapping boundary values are resolved by first-match.
 */
static const adc_range_t coarse_table[FLAP_POSITION_COUNT] =
{
    { 4001U, 4095U },   /* position 0 */
    { 3750U, 4000U },   /* position 1 */
    { 3480U, 3749U },   /* position 2 */
    { 2500U, 3479U },   /* position 3 */
    { 1000U, 2499U },   /* position 4 */
    {    0U,  999U }    /* position 5 */
};

/* ---- Precise stop detection table -------------------------------- */
static const adc_range_t precise_table[FLAP_POSITION_COUNT] =
{
    { 4055U, 4065U },   /* position 0 */
    { 3837U, 3857U },   /* position 1 */
    { 3563U, 3593U },   /* position 2 */
    { 3309U, 3329U },   /* position 3 */
    { 2022U, 2042U },   /* position 4 */
    {  315U,  335U }    /* position 5 */
};

/* ---- Module-static data ------------------------------------------ */
static uint16_t latest_adc_value;
static uint8_t  current_flap_position;
static uint8_t  feedback_valid_flag;

/* ---- Internal helpers -------------------------------------------- */

/**
 * @brief  Map a raw ADC value to a coarse logical position.
 * @return 0..5 on match, FLAP_POSITION_INVALID if outside all ranges.
 */
static uint8_t map_adc_to_position(uint16_t adc_val)
{
    uint8_t i;
    for (i = 0U; i < FLAP_POSITION_COUNT; i++)
    {
        if ((adc_val >= coarse_table[i].adc_min) &&
            (adc_val <= coarse_table[i].adc_max))
        {
            return i;
        }
    }
    return FLAP_POSITION_INVALID;
}

/* ---- Public API -------------------------------------------------- */

void feedback_processor_init(void)
{
    latest_adc_value      = 0U;
    current_flap_position = FLAP_POSITION_INVALID;
    feedback_valid_flag   = 0U;
}

void feedback_processor_update(void)
{
    HAL_StatusTypeDef status;

    /* Start single conversion. */
    status = HAL_ADC_Start(&hadc1);
    if (status != HAL_OK)
    {
        feedback_valid_flag = 0U;
        return;
    }

    /* Wait for conversion (10 ms timeout). */
    status = HAL_ADC_PollForConversion(&hadc1, 10U);
    if (status != HAL_OK)
    {
        feedback_valid_flag = 0U;
        HAL_ADC_Stop(&hadc1);
        return;
    }

    latest_adc_value = (uint16_t)HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);

    /* Validate and map. */
    {
        uint8_t pos = map_adc_to_position(latest_adc_value);
        if (pos != FLAP_POSITION_INVALID)
        {
            current_flap_position = pos;
            feedback_valid_flag   = 1U;
        }
        else
        {
            feedback_valid_flag = 0U;
        }
    }
}

uint8_t feedback_processor_get_position(void)
{
    if (feedback_valid_flag != 0U)
    {
        return current_flap_position;
    }
    return FLAP_POSITION_INVALID;
}

uint8_t feedback_processor_is_valid(void)
{
    return feedback_valid_flag;
}

uint16_t feedback_processor_get_raw_adc(void)
{
    return latest_adc_value;
}

uint8_t feedback_processor_target_reached(uint8_t target_position)
{
    if (target_position >= FLAP_POSITION_COUNT)
    {
        return 0U;
    }
    if (feedback_valid_flag == 0U)
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
