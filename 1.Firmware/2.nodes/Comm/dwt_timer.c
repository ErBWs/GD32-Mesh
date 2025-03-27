/*
 * Copyright (c) 2024-2025. ErBW_s
 * All rights reserved.
 */

#include "../Bsp/dwt/dwt_timer.h"

uint32_t dwt_time_us = 0;

void dwt_timer_start()
{
    DEM_CR |= (uint32_t) (1u << 24);    /* Set DEMCR.TRCENA */
    DWT_CYCCNT &= (uint32_t) (0u);      /* Reset DWT_CYCCNT */
    DWT_CR |= (uint32_t) (1u << 0);     /* Set DWT_CTRL.CYCCNTENA */
}

void dwt_timer_stop()
{
    /* Calculate real time(us) */
    const uint32_t freq = SystemCoreClock / 1000000;
    dwt_time_us = (uint32_t) DWT_CYCCNT / freq;

    DEM_CR |= (uint32_t) (0u << 24);    /* Reset DEMCR.TRCENA */
    DWT_CR |= (uint32_t) (0u << 0);     /* Reset DWT_CTRL.CYCCNTENA */
}