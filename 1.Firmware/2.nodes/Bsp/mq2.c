/*!
 * Copyright (c) 2025, ErBW_s
 * All rights reserved.
 *
 * @author  Baohan
 */

#include "mq2.h"
#include "main.h"
#include "adc.h"

static uint16_t mq2_adc_val[10] = {0};
float mq2_value = 0;

/**
 * Init mq2
 */
void mq2_init()
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)mq2_adc_val, 10);
}

/**
 * Read mq2 value
 * @return  mq2_info_t    mq2 info struct
 */
mq2_info_t mq2_read()
{
    double adc_val = 0.0;
    mq2_info_t info = {
        .smoke_dense = 0.0,
        .detected = false,
    };

    info.detected = HAL_GPIO_ReadPin(MQ2_GPIO_Port, MQ2_Pin);

    for (uint8_t i = 0; i < 10; i++) {
        adc_val += mq2_adc_val[i];
    }
    info.smoke_dense = adc_val / 10 / 4096 * 100;

    return info;
}
