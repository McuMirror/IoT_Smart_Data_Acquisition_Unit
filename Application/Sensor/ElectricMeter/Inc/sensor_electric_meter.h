#ifndef __SENSOR_ELECTRIC_METER_H__
#define __SENSOR_ELECTRIC_METER_H__

#include "main.h"
#include "stm32l4xx_hal.h"
#include "EEPROM_24C256.h"

#define ID_MAX_LENGTH         1300

/**
  * @brief  �����ص�ͨ�ò������ýṹ��
  */
typedef struct{
	uint32_t    baudrate;                 //������ڵ�RS485���߲�����
	uint16_t    amount;                   //��ɼ����ݵĵ�����
	uint16_t    ID_single_length;         //��������ID����
	uint16_t    ID_total_length;          //��������ID����
	uint8_t     ID_data[ID_MAX_LENGTH];   //���IDʵ������
}sen_electric_meter_gen_para;

extern sen_electric_meter_gen_para sensor_electric_meter_general_parameters;


uint8_t Electric_Meter_Baudrate_Auto_Get(sen_electric_meter_gen_para * sen_para);
uint8_t RS485_Send_Data(uint8_t *buf,uint8_t len);
uint8_t RS485_Receive_Data(uint8_t *buf,uint8_t *len);
uint8_t electric_meter_address_auto_get(uint8_t *sensor_and_ID, uint8_t retry_times);

#endif
