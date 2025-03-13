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
 * 如果没有，请参阅<https:*www.gnu.org/licenses/>
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
 * 店铺链接          https:*seekfree.taobao.com/
 *
 * 修改记录
 * 日期                                      作者                             备注
 * 2022-09-15        大W            first version
********************************************************************************************************************/

#include "soft_iic.h"

// 是否需要 SDA 进行 I/O 切换 0-不需要 1-需要
#define SOFT_IIC_SDA_IO_SWITCH      (0)

/**
 * @brief       软件 IIC START 信号
 * @param       *iic    软件 IIC 结构体指针
 * @return      void
 * @example     soft_iic_start(iic);
 * @note        内部调用
 */
static void soft_iic_start(soft_iic_info_struct *iic)
{
    HAL_GPIO_WritePin(iic->scl_port, iic->scl_pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(iic->sda_port, iic->sda_pin, GPIO_PIN_SET);

    SOFT_IIC_DELAY(iic->delay);
    HAL_GPIO_WritePin(iic->sda_port, iic->sda_pin, GPIO_PIN_RESET);
    SOFT_IIC_DELAY(iic->delay);
    HAL_GPIO_WritePin(iic->scl_port, iic->scl_pin, GPIO_PIN_RESET);
}

/**
 * @brief       软件 IIC STOP 信号
 * @param       *iic    软件 IIC 结构体指针
 * @return      void
 * @example     soft_iic_stop(iic);
 * @note        内部调用
 */
static void soft_iic_stop(soft_iic_info_struct *iic)
{
    HAL_GPIO_WritePin(iic->sda_port, iic->sda_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(iic->scl_port, iic->scl_pin, GPIO_PIN_RESET);

    SOFT_IIC_DELAY(iic->delay);
    HAL_GPIO_WritePin(iic->scl_port, iic->scl_pin, GPIO_PIN_SET);
    SOFT_IIC_DELAY(iic->delay);
    HAL_GPIO_WritePin(iic->sda_port, iic->sda_pin, GPIO_PIN_SET);
    SOFT_IIC_DELAY(iic->delay);
}

/**
 * @brief       软件 IIC 发送 ACK/NAKC 信号 内部调用
 * @param       *iic    软件 IIC 结构体指针
 * @param       ack     ACK 电平
 * @return      void
 * @example     soft_iic_send_ack(iic, 1);
 * @note        内部调用
 */
static void soft_iic_send_ack(soft_iic_info_struct *iic, uint8_t ack)
{
    HAL_GPIO_WritePin(iic->scl_port, iic->scl_pin, GPIO_PIN_RESET);

    if (ack) {
        HAL_GPIO_WritePin(iic->sda_port, iic->sda_pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(iic->sda_port, iic->sda_pin, GPIO_PIN_RESET);
    }

    SOFT_IIC_DELAY(iic->delay);
    HAL_GPIO_WritePin(iic->scl_port, iic->scl_pin, GPIO_PIN_SET);
    SOFT_IIC_DELAY(iic->delay);
    HAL_GPIO_WritePin(iic->scl_port, iic->scl_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(iic->sda_port, iic->sda_pin, GPIO_PIN_SET);
}

/**
 * @brief       软件 IIC 获取 ACK/NAKC 信号
 * @param       *iic    软件 IIC 结构体指针
 * @return      uint8_t ACK 状态
 * @example     soft_iic_wait_ack(iic);
 * @note        内部调用
 */
static uint8_t soft_iic_wait_ack(soft_iic_info_struct *iic)
{
    uint8_t temp = 0;
    HAL_GPIO_WritePin(iic->scl_port, iic->scl_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(iic->sda_port, iic->sda_pin, GPIO_PIN_SET);
#if SOFT_IIC_SDA_IO_SWITCH
    gpio_set_dir(iic->sda_pin, GPI, GPI_FLOATING_IN);
#endif
    SOFT_IIC_DELAY(iic->delay);

    HAL_GPIO_WritePin(iic->scl_port, iic->scl_pin, GPIO_PIN_SET);
    SOFT_IIC_DELAY(iic->delay);
    if (HAL_GPIO_ReadPin(iic->sda_port, iic->sda_pin)) {
        temp = 1;
    }
    HAL_GPIO_WritePin(iic->scl_port, iic->scl_pin, GPIO_PIN_RESET);
#if SOFT_IIC_SDA_IO_SWITCH
    gpio_set_dir(iic->sda_pin, GPO, GPO_OPEN_DTAIN);
#endif
    SOFT_IIC_DELAY(iic->delay);

    return temp;
}

/**
 * @brief       软件 IIC 发送 8bit 数据
 * @param       *iic    软件 IIC 结构体指针
 * @param       data    数据
 * @return      uint8_t ACK 状态
 * @note        内部调用
 */
static uint8_t soft_iic_send_data(soft_iic_info_struct *iic, const uint8_t data)
{
    uint8_t temp = 0x80;
    while (temp) {
        ((data & temp) ? (HAL_GPIO_WritePin(iic->sda_port, iic->sda_pin, GPIO_PIN_SET)) : (HAL_GPIO_WritePin(
                iic->sda_port, iic->sda_pin, GPIO_PIN_RESET)));
        temp >>= 1;

        SOFT_IIC_DELAY(iic->delay);
        HAL_GPIO_WritePin(iic->scl_port, iic->scl_pin, GPIO_PIN_SET);
        SOFT_IIC_DELAY(iic->delay);
        HAL_GPIO_WritePin(iic->scl_port, iic->scl_pin, GPIO_PIN_RESET);
    }
    return ((soft_iic_wait_ack(iic) == 1) ? 0 : 1);
}

/**
 * @brief       软件 IIC 读取 8bit 数据
 * @param       *iic    软件 IIC 结构体指针
 * @param       ack     ACK 或 NACK
 * @return      uint8_t 数据
 * @note        内部调用
 */
static uint8_t soft_iic_read_data(soft_iic_info_struct *iic, uint8_t ack)
{
    uint8_t data = 0x00;
    uint8_t temp = 8;
    HAL_GPIO_WritePin(iic->scl_port, iic->scl_pin, GPIO_PIN_RESET);
    SOFT_IIC_DELAY(iic->delay);
    HAL_GPIO_WritePin(iic->sda_port, iic->sda_pin, GPIO_PIN_SET);
#if SOFT_IIC_SDA_IO_SWITCH
    gpio_set_dir(iic->sda_pin, GPI, GPI_FLOATING_IN);
#endif

    while (temp--) {
        HAL_GPIO_WritePin(iic->scl_port, iic->scl_pin, GPIO_PIN_RESET);
        SOFT_IIC_DELAY(iic->delay);
        HAL_GPIO_WritePin(iic->scl_port, iic->scl_pin, GPIO_PIN_SET);
        SOFT_IIC_DELAY(iic->delay);
        data = ((data << 1) | HAL_GPIO_ReadPin(iic->sda_port, iic->sda_pin));
    }
    HAL_GPIO_WritePin(iic->scl_port, iic->scl_pin, GPIO_PIN_RESET);
#if SOFT_IIC_SDA_IO_SWITCH
    gpio_set_dir(iic->sda_pin, GPO, GPO_OPEN_DTAIN);
#endif
    SOFT_IIC_DELAY(iic->delay);
    soft_iic_send_ack(iic, ack);
    return data;
}

/**
 * @brief       软件 IIC 接口写 8bit 数据
 * @param       *iic    软件 IIC 结构体指针
 * @param       data    要写入的数据
 * @return      void            
 * @example     soft_iic_write_8bit_register(iic, 0x01);
 */
void soft_iic_write_8bit(soft_iic_info_struct *iic, const uint8_t data)
{
    soft_iic_start(iic);
    soft_iic_send_data(iic, iic->addr << 1);
    soft_iic_send_data(iic, data);
    soft_iic_stop(iic);
}

/**
 * @brief       软件 IIC 接口写 8bit 数组
 * @param       *iic    软件 IIC 结构体指针
 * @param       *data   数据存放缓冲区
 * @param       len     缓冲区长度
 * @return      void            
 * @example     soft_iic_write_8bit_array(iic, data, 6);
 */
void soft_iic_write_8bit_array(soft_iic_info_struct *iic, const uint8_t *data, uint32_t len)
{
    soft_iic_start(iic);
    soft_iic_send_data(iic, iic->addr << 1);
    while (len--) {
        soft_iic_send_data(iic, *data++);
    }
    soft_iic_stop(iic);
}

/**
 * @brief       软件 IIC 接口器写 16bit 数据
 * @param       *iic    软件 IIC 结构体指针
 * @param       data    要写入的数据
 * @return      void            
 * @example     soft_iic_write_16bit(iic, 0x0101);
 */
void soft_iic_write_16bit(soft_iic_info_struct *iic, const uint16_t data)
{
    soft_iic_start(iic);
    soft_iic_send_data(iic, iic->addr << 1);
    soft_iic_send_data(iic, (uint8_t) ((data & 0xFF00) >> 8));
    soft_iic_send_data(iic, (uint8_t) (data & 0x00FF));
    soft_iic_stop(iic);
}

/**
 * @brief       软件 IIC 接口写 16bit 数组
 * @param       *iic    软件 IIC 结构体指针
 * @param       *data   数据存放缓冲区
 * @param       len     缓冲区长度
 * @return      void            
 * @example     soft_iic_write_16bit_array(iic, data, 6);
 */
void soft_iic_write_16bit_array(soft_iic_info_struct *iic, const uint16_t *data, uint32_t len)
{
    soft_iic_start(iic);
    soft_iic_send_data(iic, iic->addr << 1);
    while (len--) {
        soft_iic_send_data(iic, (uint8_t) ((*data & 0xFF00) >> 8));
        soft_iic_send_data(iic, (uint8_t) (*data++ & 0x00FF));
    }
    soft_iic_stop(iic);
}

/**
 * @brief       软件 IIC 接口向传感器寄存器写 8bit 数据
 * @param       *iic            软件 IIC 结构体指针
 * @param       register_name   传感器的寄存器地址
 * @param       data            要写入的数据
 * @return      void            
 * @example     soft_iic_write_8bit_register(iic, 0x01, 0x01);
 */
void soft_iic_write_8bit_register(soft_iic_info_struct *iic, const uint8_t register_name, const uint8_t data)
{
    soft_iic_start(iic);
    soft_iic_send_data(iic, iic->addr << 1);
    soft_iic_send_data(iic, register_name);
    soft_iic_send_data(iic, data);
    soft_iic_stop(iic);
}

/**
 * @brief       软件 IIC 接口向传感器寄存器写 8bit 数组
 * @param       *iic            软件 IIC 结构体指针
 * @param       register_name   传感器的寄存器地址
 * @param       *data           数据存放缓冲区
 * @param       len             缓冲区长度
 * @return      void            
 * @example     soft_iic_write_8bit_registers(iic, 0x01, data, 6);
 */
void soft_iic_write_8bit_registers(soft_iic_info_struct *iic, const uint8_t register_name, const uint8_t *data,
                                   uint32_t len)
{
    soft_iic_start(iic);
    soft_iic_send_data(iic, iic->addr << 1);
    soft_iic_send_data(iic, register_name);
    while (len--) {
        soft_iic_send_data(iic, *data++);
    }
    soft_iic_stop(iic);
}

/**
 * @brief       软件 IIC 接口向传感器寄存器写 16bit 数据
 * @param       *iic            软件 IIC 结构体指针
 * @param       register_name   传感器的寄存器地址
 * @param       data            要写入的数据
 * @return      void            
 * @example     soft_iic_write_16bit_register(iic, 0x0101, 0x0101);
 */
void soft_iic_write_16bit_register(soft_iic_info_struct *iic, const uint16_t register_name, const uint16_t data)
{
    soft_iic_start(iic);
    soft_iic_send_data(iic, iic->addr << 1);
    soft_iic_send_data(iic, (uint8_t) ((register_name & 0xFF00) >> 8));
    soft_iic_send_data(iic, (uint8_t) (register_name & 0x00FF));
    soft_iic_send_data(iic, (uint8_t) ((data & 0xFF00) >> 8));
    soft_iic_send_data(iic, (uint8_t) (data & 0x00FF));
    soft_iic_stop(iic);
}

/**
 * @brief       软件 IIC 接口向传感器寄存器写 16bit 数组
 * @param       *iic            软件 IIC 结构体指针
 * @param       register_name   传感器的寄存器地址
 * @param       *data           数据存放缓冲区
 * @param       len             缓冲区长度
 * @return      void            
 * @example     soft_iic_write_16bit_registers(iic, 0x0101, data, 6);
 */
void soft_iic_write_16bit_registers(soft_iic_info_struct *iic, const uint16_t register_name, const uint16_t *data,
                                    uint32_t len)
{
    soft_iic_start(iic);
    soft_iic_send_data(iic, iic->addr << 1);
    soft_iic_send_data(iic, (uint8_t) ((register_name & 0xFF00) >> 8));
    soft_iic_send_data(iic, (uint8_t) (register_name & 0x00FF));
    while (len--) {
        soft_iic_send_data(iic, (uint8_t) ((*data & 0xFF00) >> 8));
        soft_iic_send_data(iic, (uint8_t) (*data++ & 0x00FF));
    }
    soft_iic_stop(iic);
}

/**
 * @brief       软件 IIC 接口读取 8bit 数据
 * @param       *iic        软件 IIC 结构体指针
 * @return      uint8_t     返回读取的 8bit 数据
 * @example     soft_iic_read_8bit(iic);
 */
uint8_t soft_iic_read_8bit(soft_iic_info_struct *iic)
{
    uint8_t temp = 0;
    soft_iic_start(iic);
    soft_iic_send_data(iic, iic->addr << 1 | 0x01);
    temp = soft_iic_read_data(iic, 1);
    soft_iic_stop(iic);
    return temp;
}

/**
 * @brief       软件 IIC 接口从传感器寄存器读取 8bit 数组
 * @param       *iic            软件 IIC 结构体指针
 * @param       register_name   传感器的寄存器地址
 * @param       *data           要读取的数据的缓冲区指针
 * @param       len             要读取的数据长度
 * @return      void            
 * @example     soft_iic_read_8bit_array(iic, data, 8);
 */
void soft_iic_read_8bit_array(soft_iic_info_struct *iic, uint8_t *data, uint32_t len)
{
    soft_iic_start(iic);
    soft_iic_send_data(iic, iic->addr << 1 | 0x01);
    while (len--) {
        *data++ = soft_iic_read_data(iic, len == 0);
    }
    soft_iic_stop(iic);
}

/**
 * @brief       软件 IIC 接口读取 16bit 数据
 * @param       *iic            软件 IIC 结构体指针
 * @param       register_name   传感器的寄存器地址
 * @return      uint16_t        返回读取的 16bit 数据
 * @example     soft_iic_read_16bit(iic);
 */
uint16_t soft_iic_read_16bit(soft_iic_info_struct *iic)
{
    uint16_t temp = 0;
    soft_iic_start(iic);
    soft_iic_send_data(iic, iic->addr << 1 | 0x01);
    temp = soft_iic_read_data(iic, 0);
    temp = ((temp << 8) | soft_iic_read_data(iic, 1));
    soft_iic_stop(iic);
    return temp;
}

/**
 * @brief       软件 IIC 接口读取 16bit 数组
 * @param       *iic    软件 IIC 结构体指针
 * @param       *data   要读取的数据的缓冲区指针
 * @param       len     要读取的数据长度
 * @return      void            
 * @example     soft_iic_read_16bit_array(iic, data, 8);
 */
void soft_iic_read_16bit_array(soft_iic_info_struct *iic, uint16_t *data, uint32_t len)
{
    soft_iic_start(iic);
    soft_iic_send_data(iic, iic->addr << 1 | 0x01);
    while (len--) {
        *data = soft_iic_read_data(iic, 0);
        *data = ((*data << 8) | soft_iic_read_data(iic, len == 0));
        data++;
    }
    soft_iic_stop(iic);
}

/**
 * @brief       软件 IIC 接口从传感器寄存器读取 8bit 数据
 * @param       *iic            软件 IIC 结构体指针
 * @param       register_name   传感器的寄存器地址
 * @return      uint8_t         返回读取的 8bit 数据
 * @example     soft_iic_read_8bit_register(iic, 0x01);
 */
uint8_t soft_iic_read_8bit_register(soft_iic_info_struct *iic, const uint8_t register_name)
{
    uint8_t temp = 0;
    soft_iic_start(iic);
    soft_iic_send_data(iic, iic->addr << 1);
    soft_iic_send_data(iic, register_name);
    soft_iic_start(iic);
    soft_iic_send_data(iic, iic->addr << 1 | 0x01);
    temp = soft_iic_read_data(iic, 1);
    soft_iic_stop(iic);
    return temp;
}

/**
 * @brief       软件 IIC 接口从传感器寄存器读取 8bit 数组
 * @param       *iic            软件 IIC 结构体指针
 * @param       register_name   传感器的寄存器地址
 * @param       *data           要读取的数据的缓冲区指针
 * @param       len             要读取的数据长度
 * @return      void            
 * @example     soft_iic_read_8bit_registers(iic, 0x01, data, 8);
 */
void
soft_iic_read_8bit_registers(soft_iic_info_struct *iic, const uint8_t register_name, uint8_t *data, uint32_t len)
{
    soft_iic_start(iic);
    soft_iic_send_data(iic, iic->addr << 1);
    soft_iic_send_data(iic, register_name);
    soft_iic_start(iic);
    soft_iic_send_data(iic, iic->addr << 1 | 0x01);
    while (len--) {
        *data++ = soft_iic_read_data(iic, len == 0);
    }
    soft_iic_stop(iic);
}

/**
 * @brief       软件 IIC 接口从传感器寄存器读取 16bit 数据
 * @param       *iic            软件 IIC 结构体指针
 * @param       register_name   传感器的寄存器地址
 * @return      uint16_t        返回读取的 16bit 数据
 * @example     soft_iic_read_16bit_register(iic, 0x0101);
 */
uint16_t soft_iic_read_16bit_register(soft_iic_info_struct *iic, const uint16_t register_name)
{
    uint16_t temp = 0;
    soft_iic_start(iic);
    soft_iic_send_data(iic, iic->addr << 1);
    soft_iic_send_data(iic, (uint8_t) ((register_name & 0xFF00) >> 8));
    soft_iic_send_data(iic, (uint8_t) (register_name & 0x00FF));
    soft_iic_start(iic);
    soft_iic_send_data(iic, iic->addr << 1 | 0x01);
    temp = soft_iic_read_data(iic, 0);
    temp = ((temp << 8) | soft_iic_read_data(iic, 1));
    soft_iic_stop(iic);
    return temp;
}

/**
 * @brief       软件 IIC 接口从传感器寄存器读取 16bit 数组
 * @param       *iic            软件 IIC 结构体指针
 * @param       register_name   传感器的寄存器地址
 * @param       *data           要读取的数据的缓冲区指针
 * @param       len             要读取的数据长度
 * @return      void            
 * @example     soft_iic_read_16bit_registers(iic, 0x0101, data, 8);
 */
void
soft_iic_read_16bit_registers(soft_iic_info_struct *iic, const uint16_t register_name, uint16_t *data, uint32_t len)
{
    soft_iic_start(iic);
    soft_iic_send_data(iic, iic->addr << 1);
    soft_iic_send_data(iic, (uint8_t) ((register_name & 0xFF00) >> 8));
    soft_iic_send_data(iic, (uint8_t) (register_name & 0x00FF));
    soft_iic_start(iic);
    soft_iic_send_data(iic, iic->addr << 1 | 0x01);
    while (len--) {
        *data = soft_iic_read_data(iic, 0);
        *data = ((*data << 8) | soft_iic_read_data(iic, len == 0));
        data++;
    }
    soft_iic_stop(iic);
}

/**
 * @brief       软件 IIC 接口传输 8bit 数组 先写后读取
 * @param       *iic            软件 IIC 结构体指针
 * @param       *write_data     发送数据存放缓冲区
 * @param       write_len       发送缓冲区长度
 * @param       *read_data      读取数据存放缓冲区
 * @param       read_len        读取缓冲区长度
 * @return      void            
 * @example     iic_transfer_8bit_array(IIC_1, addr, data, 64, data, 64);
 */
void soft_iic_transfer_8bit_array(soft_iic_info_struct *iic, const uint8_t *write_data, uint32_t write_len,
                                  uint8_t *read_data, uint32_t read_len)
{
    soft_iic_start(iic);
    soft_iic_send_data(iic, iic->addr << 1);
    while (write_len--) {
        soft_iic_send_data(iic, *write_data++);
    }
    soft_iic_start(iic);
    soft_iic_send_data(iic, iic->addr << 1 | 0x01);
    while (read_len--) {
        *read_data++ = soft_iic_read_data(iic, read_len == 0);
    }
    soft_iic_stop(iic);
}

/**
 * @brief       软件 IIC 接口传输 16bit 数组 先写后读取
 * @param       *iic            软件 IIC 结构体指针
 * @param       *write_data     发送数据存放缓冲区
 * @param       write_len       发送缓冲区长度
 * @param       *read_data      读取数据存放缓冲区
 * @param       read_len        读取缓冲区长度
 * @return      void            
 * @example     iic_transfer_16bit_array(IIC_1, addr, data, 64, data, 64);
 */
void soft_iic_transfer_16bit_array(soft_iic_info_struct *iic, const uint16_t *write_data, uint32_t write_len,
                                   uint16_t *read_data, uint32_t read_len)
{
    soft_iic_start(iic);
    soft_iic_send_data(iic, iic->addr << 1);
    while (write_len--) {
        soft_iic_send_data(iic, (uint8_t) ((*write_data & 0xFF00) >> 8));
        soft_iic_send_data(iic, (uint8_t) (*write_data++ & 0x00FF));
    }
    soft_iic_start(iic);
    soft_iic_send_data(iic, iic->addr << 1 | 0x01);
    while (read_len--) {
        *read_data = soft_iic_read_data(iic, 0);
        *read_data = ((*read_data << 8) | soft_iic_read_data(iic, read_len == 0));
        read_data++;
    }
    soft_iic_stop(iic);
}

/**
 * @brief       软件 IIC 接口 SCCB 模式向传感器寄存器写 8bit 数据
 * @param       *iic            软件 IIC 结构体指针
 * @param       register_name   传感器的寄存器地址
 * @param       data            要写入的数据
 * @return      void            
 * @example     soft_iic_sccb_write_register(iic, 0x01, 0x01);
 */
void soft_iic_sccb_write_register(soft_iic_info_struct *iic, const uint8_t register_name, uint8_t data)
{
    soft_iic_start(iic);
    soft_iic_send_data(iic, iic->addr << 1);
    soft_iic_send_data(iic, register_name);
    soft_iic_send_data(iic, data);
    soft_iic_stop(iic);
}

/**
 * @brief       软件 IIC 接口 SCCB 模式从传感器寄存器读取 8bit 数据
 * @param       *iic   软件 IIC 结构体指针
 * @param       register_name   传感器的寄存器地址
 * @return      uint8_t         返回读取的 8bit 数据
 * @example     soft_iic_sccb_read_register(iic, 0x01);
 */
uint8_t soft_iic_sccb_read_register(soft_iic_info_struct *iic, const uint8_t register_name)
{
    uint8_t temp = 0;
    soft_iic_start(iic);
    soft_iic_send_data(iic, iic->addr << 1);
    soft_iic_send_data(iic, register_name);
    soft_iic_stop(iic);

    soft_iic_start(iic);
    soft_iic_send_data(iic, iic->addr << 1 | 0x01);
    temp = soft_iic_read_data(iic, 1);
    soft_iic_stop(iic);
    return temp;
}

/**
 * @brief       软件 IIC 接口初始化 默认 MASTER 模式 不提供 SLAVE 模式
 * @param       *iic        软件 IIC 指定信息存放结构体的指针
 * @param       addr        软件 IIC 地址 这里需要注意 标准七位地址 最高位忽略 写入时请务必确认无误
 * @param       delay       软件 IIC 延时 就是时钟高电平时间 越短 IIC 速率越高
 * @param       scl_port    软件 IIC 时钟引脚
 * @param       scl_pin     软件 IIC 时钟引脚号
 * @param       sda_port    软件 IIC 数据引脚
 * @param       sda_pin     软件 IIC 数据引脚号
 * @return      void
 * @example     soft_iic_init(&iic, addr, 100, GPIOB, 6, GPIOB, 7);
 */
void
soft_iic_init(soft_iic_info_struct *iic, uint8_t addr, uint32_t delay, GPIO_TypeDef *scl_port, uint16_t scl_pin,
              GPIO_TypeDef *sda_port, uint16_t sda_pin)
{
    iic->scl_port = scl_port;
    iic->scl_pin = scl_pin;
    iic->sda_port = sda_port;
    iic->sda_pin = sda_pin;
    iic->addr = addr;
    iic->delay = delay;
}
