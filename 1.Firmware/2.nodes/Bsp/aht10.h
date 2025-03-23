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

#define AHT10_ADDR      0x38
#define AHT10_DELAY     100

typedef struct {
    double temperature;
    double humidity;
} aht10_info_t;

aht10_info_t aht10_read();

#ifdef __cplusplus
}
#endif

#endif
