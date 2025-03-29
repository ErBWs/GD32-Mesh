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

typedef struct {
    uint8_t addr;
    float val;
}__attribute__((packed)) lora_frame_t;

extern lora_frame_t smoke;
extern lora_frame_t temperature;
extern lora_frame_t humidity;

void lora_parse_frame(const uint8_t *buffer);

#ifdef __cplusplus
}
#endif

#endif
