/*
 * Copyright (c) 2025. ErBW_s
 * All rights reserved.
 */

#ifndef LORA_H
#define LORA_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

void lora_send_to_gateway(uint8_t addr, float val);

#ifdef __cplusplus
}
#endif

#endif
