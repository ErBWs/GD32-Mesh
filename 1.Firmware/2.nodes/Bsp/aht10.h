/*!
 * Copyright (c) 2025, ErBW_s
 * All rights reserved.
 * 
 * @author  Baohan
 */

#ifndef AHT10_H
#define AHT10_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "soft_iic.h"

#define AHT10_ADDR      0x38
#define AHT10_DELAY     100

extern double aht10_temperature;
extern double aht10_humidity;

void aht10_read();

#ifdef __cplusplus
}
#endif

#endif
