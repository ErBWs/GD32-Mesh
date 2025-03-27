/*
 * Copyright (c) 2024-2025. ErBW_s
 * All rights reserved.
 */

#ifndef STM32F407IGT6_DWT_TIMER_H
#define STM32F407IGT6_DWT_TIMER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "stm32f4xx_hal.h"

#define DWT_CR          *(__IO uint32_t *) 0xE0001000
#define DWT_CYCCNT      *(__IO uint32_t *) 0xE0001004
#define DEM_CR          *(__IO uint32_t *) 0xE000EDFC

extern uint32_t dwt_time_us;

void dwt_timer_start();
void dwt_timer_stop();

#ifdef __cplusplus
}
#endif

#endif
