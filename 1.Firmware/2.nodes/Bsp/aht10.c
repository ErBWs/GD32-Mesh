/*!
 * Copyright (c) 2025, ErBW_s
 * All rights reserved.
 * 
 * @author  Baohan
 */

#include "aht10.h"

static soft_iic_info_struct aht10_iic = {
        .scl_port = AHT10_SCL_GPIO_Port,
        .scl_pin = AHT10_SCL_Pin,
        .sda_port = AHT10_SDA_GPIO_Port,
        .sda_pin = AHT10_SDA_Pin,
        .addr = AHT10_ADDR,
        .delay = AHT10_DELAY
};

double aht10_temperature = 0.0;
double aht10_humidity = 0.0;

void aht10_read()
{
    uint8_t start_cmd[3] = {0xac, 0x33, 0x00};
    uint8_t res[6] = {0};

    // Send cmd to trigger aht10 measurement
    soft_iic_write_8bit_array(&aht10_iic, start_cmd, 3);

    HAL_Delay(10);

    // Read aht10 data
    soft_iic_read_8bit_array(&aht10_iic, res, 6);

    // Convert
    int hum = (((res[1] << 12) | (res[2] << 4)) | (res[3] >> 4));
    aht10_humidity = (double) hum / 1048576.0 * 100.0;

    int temp = ((res[3] & 0x0F) << 16) | (res[4] << 8) | res[5];
    aht10_temperature = (double) temp / 1048576.0 * 200 - 50;
}