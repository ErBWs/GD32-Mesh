/*
 * Copyright (c) 2023-2025. ErBW_s
 * All rights reserved.
 */

#ifndef VOFA_H
#define VOFA_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "usart.h"

#define CHANNEL_NUM     3

extern float vofaData[CHANNEL_NUM];

void VofaSendFrame();

#ifdef __cplusplus
}
#endif

#endif
