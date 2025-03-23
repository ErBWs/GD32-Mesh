/*!
 * Copyright (c) 2025, ErBW_s
 * All rights reserved.
 * 
 * @author  Baohan
 */

#include "aht10.h"
#include <stdint.h>
#include "soft_iic.h"

static soft_iic_info_struct aht10_iic = {
        .scl_port = AHT10_SCL_GPIO_Port,
        .scl_pin = AHT10_SCL_Pin,
        .sda_port = AHT10_SDA_GPIO_Port,
        .sda_pin = AHT10_SDA_Pin,
        .addr = AHT10_ADDR,
        .delay = AHT10_DELAY
};

/**
 * @brief   Trigger aht10 measurement and read value
 *
 * @return  aht10_info_t    aht10 info struct
 */
aht10_info_t aht10_read()
{
    const uint8_t start_cmd[3] = {0xac, 0x33, 0x00};
    uint8_t res[6] = {0};
    aht10_info_t info = {
            .temperature = 0,
            .humidity = 0,
    };

    // Send cmd to trigger aht10 measurement
    soft_iic_write_8bit_array(&aht10_iic, start_cmd, 3);

    HAL_Delay(10);

    // Read aht10 data
    soft_iic_read_8bit_array(&aht10_iic, res, 6);

    // Convert
    const int hum = (((res[1] << 12) | (res[2] << 4)) | (res[3] >> 4));
    info.humidity = (double) hum / 1048576.0 * 100.0;

    const int temp = ((res[3] & 0x0F) << 16) | (res[4] << 8) | res[5];
    info.temperature = (double) temp / 1048576.0 * 200 - 50;

    return info;
}