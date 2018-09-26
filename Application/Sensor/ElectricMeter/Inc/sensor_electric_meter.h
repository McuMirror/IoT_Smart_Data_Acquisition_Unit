#ifndef __SENSOR_ELECTRIC_METER_H__
#define __SENSOR_ELECTRIC_METER_H__

#include "main.h"
#include "stm32l4xx_hal.h"

#define ID_MAX_LENGTH         1300

/**
  * @brief  �����ص�ͨ�ò������ýṹ��
  */
typedef struct{
	uint64_t    baudrate;                 //������ڵ�RS485���߲�����
	uint16_t    amount;                   //��ɼ����ݵĵ�����
	uint16_t    ID_single_length;         //��������ID����
	uint16_t    ID_total_length;          //��������ID����
	uint8_t     ID_data[ID_MAX_LENGTH];   //���IDʵ������
}sen_electric_meter_gen_para;

extern sen_electric_meter_gen_para sensor_electric_meter_general_parameters;


uint8_t Electric_Meter_Baudrate_Auto_Get(sen_electric_meter_gen_para * sen_para);



#endif
