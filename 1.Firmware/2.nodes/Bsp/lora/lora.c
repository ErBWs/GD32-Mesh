/*
 * Copyright (c) 2025. ErBW_s
 * All rights reserved.
 */

#include "lora.h"
#include <stdint.h>
#include "main.h"
#include "usart.h"

// frame head send to gateway
static uint8_t gateway_frame[3] = {0x00, 0x01, 0x0A};

void lora_send_to_gateway(const uint8_t addr, float val)
{
    HAL_UART_Transmit(&huart2, gateway_frame, 3, 0xFFFF);
    HAL_UART_Transmit(&huart2, &addr, 1, 0xFFFF);
    HAL_UART_Transmit(&huart2, (uint8_t *) &val, 4, 0xFFFF);
}