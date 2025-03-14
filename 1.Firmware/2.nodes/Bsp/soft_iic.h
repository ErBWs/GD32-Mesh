/*********************************************************************************************************************
* CH32V307VCT6 Opensourec Library 即（CH32V307VCT6 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是CH32V307VCT6 开源库的一部分
*
* CH32V307VCT6 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          zf_driver_soft_iic
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          MounRiver Studio V1.8.1
* 适用平台          CH32V307VCT6
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期                                      作者                             备注
* 2022-09-15        大W            first version
********************************************************************************************************************/

#ifndef SOFT_IIC_H_
#define SOFT_IIC_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "main.h"

/**
 * @brief       软件 IIC 延时
 * @param       delay   延时次数
 * @return      void
 * @example     SOFT_IIC_DELAY(1);
 */
#define SOFT_IIC_DELAY(x)  for(uint32_t i = x; i--; )

typedef struct {
    GPIO_TypeDef *scl_port;
    uint16_t scl_pin;
    GPIO_TypeDef *sda_port;
    uint16_t sda_pin;
    uint8_t addr;                                                   // 器件地址 七位地址模式
    uint32_t delay;                                                  // 模拟 IIC 软延时时长
} soft_iic_info_struct;

void soft_iic_write_8bit(soft_iic_info_struct *iic, uint8_t data);

void soft_iic_write_8bit_array(soft_iic_info_struct *iic, const uint8_t *data, uint32_t len);

void soft_iic_write_16bit(soft_iic_info_struct *iic, uint16_t data);

void soft_iic_write_16bit_array(soft_iic_info_struct *iic, const uint16_t *data, uint32_t len);

void soft_iic_write_8bit_register(soft_iic_info_struct *iic, uint8_t register_name, uint8_t data);

void soft_iic_write_8bit_registers(soft_iic_info_struct *iic, uint8_t register_name, const uint8_t *data, uint32_t len);

void soft_iic_write_16bit_register(soft_iic_info_struct *iic, uint16_t register_name, uint16_t data);

void
soft_iic_write_16bit_registers(soft_iic_info_struct *iic, uint16_t register_name, const uint16_t *data, uint32_t len);

uint8_t soft_iic_read_8bit(soft_iic_info_struct *iic);

void soft_iic_read_8bit_array(soft_iic_info_struct *iic, uint8_t *data, uint32_t len);

uint16_t soft_iic_read_16bit(soft_iic_info_struct *iic);

void soft_iic_read_16bit_array(soft_iic_info_struct *iic, uint16_t *data, uint32_t len);

uint8_t soft_iic_read_8bit_register(soft_iic_info_struct *iic, uint8_t register_name);

void
soft_iic_read_8bit_registers(soft_iic_info_struct *iic, uint8_t register_name, uint8_t *data, uint32_t len);

uint16_t soft_iic_read_16bit_register(soft_iic_info_struct *iic, uint16_t register_name);

void
soft_iic_read_16bit_registers(soft_iic_info_struct *iic, uint16_t register_name, uint16_t *data, uint32_t len);

void soft_iic_transfer_8bit_array(soft_iic_info_struct *iic, const uint8_t *write_data, uint32_t write_len,
                                  uint8_t *read_data, uint32_t read_len);

void soft_iic_transfer_16bit_array(soft_iic_info_struct *iic, const uint16_t *write_data, uint32_t write_len,
                                   uint16_t *read_data, uint32_t read_len);

void soft_iic_sccb_write_register(soft_iic_info_struct *iic, uint8_t register_name, uint8_t data);

uint8_t soft_iic_sccb_read_register(soft_iic_info_struct *iic, uint8_t register_name);

#ifdef __cplusplus
}
#endif

#endif