/*********************************************************************************************************************
 * CH32V307VCT6 Opensourec Library ����CH32V307VCT6 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
 * Copyright (c) 2022 SEEKFREE ��ɿƼ�
 *
 * ���ļ���CH32V307VCT6 ��Դ���һ����
 *
 * CH32V307VCT6 ��Դ�� ��������
 * �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
 * �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
 *
 * ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
 * ����û�������������Ի��ʺ��ض���;�ı�֤
 * ����ϸ����μ� GPL
 *
 * ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
 * ���û�У������<https:*www.gnu.org/licenses/>
 *
 * ����ע����
 * ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
 * �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
 * ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
 * ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
 *
 * �ļ�����          zf_driver_soft_iic
 * ��˾����          �ɶ���ɿƼ����޹�˾
 * �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
 * ��������          MounRiver Studio V1.8.1
 * ����ƽ̨          CH32V307VCT6
 * ��������          https:*seekfree.taobao.com/
 *
 * �޸ļ�¼
 * ����                                      ����                             ��ע
 * 2022-09-15        ��W            first version
********************************************************************************************************************/

#include "soft_iic.h"

// �Ƿ���Ҫ SDA ���� I/O �л� 0-����Ҫ 1-��Ҫ
#define SOFT_IIC_SDA_IO_SWITCH      (0)

/**
 * @brief       ��� IIC START �ź�
 * @param       *iic    ��� IIC �ṹ��ָ��
 * @return      void
 * @example     soft_iic_start(iic);
 * @note        �ڲ�����
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
 * @brief       ��� IIC STOP �ź�
 * @param       *iic    ��� IIC �ṹ��ָ��
 * @return      void
 * @example     soft_iic_stop(iic);
 * @note        �ڲ�����
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
 * @brief       ��� IIC ���� ACK/NAKC �ź� �ڲ�����
 * @param       *iic    ��� IIC �ṹ��ָ��
 * @param       ack     ACK ��ƽ
 * @return      void
 * @example     soft_iic_send_ack(iic, 1);
 * @note        �ڲ�����
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
 * @brief       ��� IIC ��ȡ ACK/NAKC �ź�
 * @param       *iic    ��� IIC �ṹ��ָ��
 * @return      uint8_t ACK ״̬
 * @example     soft_iic_wait_ack(iic);
 * @note        �ڲ�����
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
 * @brief       ��� IIC ���� 8bit ����
 * @param       *iic    ��� IIC �ṹ��ָ��
 * @param       data    ����
 * @return      uint8_t ACK ״̬
 * @note        �ڲ�����
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
 * @brief       ��� IIC ��ȡ 8bit ����
 * @param       *iic    ��� IIC �ṹ��ָ��
 * @param       ack     ACK �� NACK
 * @return      uint8_t ����
 * @note        �ڲ�����
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
 * @brief       ��� IIC �ӿ�д 8bit ����
 * @param       *iic    ��� IIC �ṹ��ָ��
 * @param       data    Ҫд�������
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
 * @brief       ��� IIC �ӿ�д 8bit ����
 * @param       *iic    ��� IIC �ṹ��ָ��
 * @param       *data   ���ݴ�Ż�����
 * @param       len     ����������
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
 * @brief       ��� IIC �ӿ���д 16bit ����
 * @param       *iic    ��� IIC �ṹ��ָ��
 * @param       data    Ҫд�������
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
 * @brief       ��� IIC �ӿ�д 16bit ����
 * @param       *iic    ��� IIC �ṹ��ָ��
 * @param       *data   ���ݴ�Ż�����
 * @param       len     ����������
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
 * @brief       ��� IIC �ӿ��򴫸����Ĵ���д 8bit ����
 * @param       *iic            ��� IIC �ṹ��ָ��
 * @param       register_name   �������ļĴ�����ַ
 * @param       data            Ҫд�������
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
 * @brief       ��� IIC �ӿ��򴫸����Ĵ���д 8bit ����
 * @param       *iic            ��� IIC �ṹ��ָ��
 * @param       register_name   �������ļĴ�����ַ
 * @param       *data           ���ݴ�Ż�����
 * @param       len             ����������
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
 * @brief       ��� IIC �ӿ��򴫸����Ĵ���д 16bit ����
 * @param       *iic            ��� IIC �ṹ��ָ��
 * @param       register_name   �������ļĴ�����ַ
 * @param       data            Ҫд�������
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
 * @brief       ��� IIC �ӿ��򴫸����Ĵ���д 16bit ����
 * @param       *iic            ��� IIC �ṹ��ָ��
 * @param       register_name   �������ļĴ�����ַ
 * @param       *data           ���ݴ�Ż�����
 * @param       len             ����������
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
 * @brief       ��� IIC �ӿڶ�ȡ 8bit ����
 * @param       *iic        ��� IIC �ṹ��ָ��
 * @return      uint8_t     ���ض�ȡ�� 8bit ����
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
 * @brief       ��� IIC �ӿڴӴ������Ĵ�����ȡ 8bit ����
 * @param       *iic            ��� IIC �ṹ��ָ��
 * @param       register_name   �������ļĴ�����ַ
 * @param       *data           Ҫ��ȡ�����ݵĻ�����ָ��
 * @param       len             Ҫ��ȡ�����ݳ���
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
 * @brief       ��� IIC �ӿڶ�ȡ 16bit ����
 * @param       *iic            ��� IIC �ṹ��ָ��
 * @param       register_name   �������ļĴ�����ַ
 * @return      uint16_t        ���ض�ȡ�� 16bit ����
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
 * @brief       ��� IIC �ӿڶ�ȡ 16bit ����
 * @param       *iic    ��� IIC �ṹ��ָ��
 * @param       *data   Ҫ��ȡ�����ݵĻ�����ָ��
 * @param       len     Ҫ��ȡ�����ݳ���
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
 * @brief       ��� IIC �ӿڴӴ������Ĵ�����ȡ 8bit ����
 * @param       *iic            ��� IIC �ṹ��ָ��
 * @param       register_name   �������ļĴ�����ַ
 * @return      uint8_t         ���ض�ȡ�� 8bit ����
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
 * @brief       ��� IIC �ӿڴӴ������Ĵ�����ȡ 8bit ����
 * @param       *iic            ��� IIC �ṹ��ָ��
 * @param       register_name   �������ļĴ�����ַ
 * @param       *data           Ҫ��ȡ�����ݵĻ�����ָ��
 * @param       len             Ҫ��ȡ�����ݳ���
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
 * @brief       ��� IIC �ӿڴӴ������Ĵ�����ȡ 16bit ����
 * @param       *iic            ��� IIC �ṹ��ָ��
 * @param       register_name   �������ļĴ�����ַ
 * @return      uint16_t        ���ض�ȡ�� 16bit ����
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
 * @brief       ��� IIC �ӿڴӴ������Ĵ�����ȡ 16bit ����
 * @param       *iic            ��� IIC �ṹ��ָ��
 * @param       register_name   �������ļĴ�����ַ
 * @param       *data           Ҫ��ȡ�����ݵĻ�����ָ��
 * @param       len             Ҫ��ȡ�����ݳ���
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
 * @brief       ��� IIC �ӿڴ��� 8bit ���� ��д���ȡ
 * @param       *iic            ��� IIC �ṹ��ָ��
 * @param       *write_data     �������ݴ�Ż�����
 * @param       write_len       ���ͻ���������
 * @param       *read_data      ��ȡ���ݴ�Ż�����
 * @param       read_len        ��ȡ����������
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
 * @brief       ��� IIC �ӿڴ��� 16bit ���� ��д���ȡ
 * @param       *iic            ��� IIC �ṹ��ָ��
 * @param       *write_data     �������ݴ�Ż�����
 * @param       write_len       ���ͻ���������
 * @param       *read_data      ��ȡ���ݴ�Ż�����
 * @param       read_len        ��ȡ����������
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
 * @brief       ��� IIC �ӿ� SCCB ģʽ�򴫸����Ĵ���д 8bit ����
 * @param       *iic            ��� IIC �ṹ��ָ��
 * @param       register_name   �������ļĴ�����ַ
 * @param       data            Ҫд�������
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
 * @brief       ��� IIC �ӿ� SCCB ģʽ�Ӵ������Ĵ�����ȡ 8bit ����
 * @param       *iic   ��� IIC �ṹ��ָ��
 * @param       register_name   �������ļĴ�����ַ
 * @return      uint8_t         ���ض�ȡ�� 8bit ����
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
 * @brief       ��� IIC �ӿڳ�ʼ�� Ĭ�� MASTER ģʽ ���ṩ SLAVE ģʽ
 * @param       *iic        ��� IIC ָ����Ϣ��Žṹ���ָ��
 * @param       addr        ��� IIC ��ַ ������Ҫע�� ��׼��λ��ַ ���λ���� д��ʱ�����ȷ������
 * @param       delay       ��� IIC ��ʱ ����ʱ�Ӹߵ�ƽʱ�� Խ�� IIC ����Խ��
 * @param       scl_port    ��� IIC ʱ������
 * @param       scl_pin     ��� IIC ʱ�����ź�
 * @param       sda_port    ��� IIC ��������
 * @param       sda_pin     ��� IIC �������ź�
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
