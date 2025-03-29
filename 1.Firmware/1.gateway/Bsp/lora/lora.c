/*
 * Copyright (c) 2025. ErBW_s
 * All rights reserved.
 */

#include "lora.h"
#include <string.h>
#include "main.h"

// frame head send to nodes by broadcast
static uint8_t nodes_frame[3] = {0xFF, 0xFF, 0x0B};

lora_frame_t smoke = {0x01, 0.0f};
lora_frame_t temperature = {0x02, 0.0f};
lora_frame_t humidity = {0x03, 0.0f};

void lora_parse_frame(const uint8_t *buffer)
{
    lora_frame_t frame;
    memcpy(&frame, buffer, sizeof(lora_frame_t));
    switch (frame.addr) {
    case 0x01:
        smoke.val = frame.val;
        break;
    case 0x02:
        temperature.val = frame.val;
        break;
    case 0x03:
        humidity.val = frame.val;
        break;
    default:
        break;
    }
}
