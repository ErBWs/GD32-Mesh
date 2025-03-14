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
* ���û�У������<https://www.gnu.org/licenses/>
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
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����                                      ����                             ��ע
* 2022-09-15        ��W            first version
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
 * @brief       ��� IIC ��ʱ
 * @param       delay   ��ʱ����
 * @return      void
 * @example     SOFT_IIC_DELAY(1);
 */
#define SOFT_IIC_DELAY(x)  for(uint32_t i = x; i--; )

typedef struct {
    GPIO_TypeDef *scl_port;
    uint16_t scl_pin;
    GPIO_TypeDef *sda_port;
    uint16_t sda_pin;
    uint8_t addr;                                                   // ������ַ ��λ��ַģʽ
    uint32_t delay;                                                  // ģ�� IIC ����ʱʱ��
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