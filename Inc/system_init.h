#ifndef __SYSTEM_INIT_H__
#define __SYSTEM_INIT_H__

#include "main.h"
#include "stm32l4xx_hal.h"
#include "linked_list.h"

/******************************AP��Ӳ����ʼ��************************************/
void AP_side_hardware_init(void);                          //AP�����Ӳ����ʼ�� ����Ƭ�����衢ͨ��ģ�鼰������ģ��
void necessary_on_chip_peripherals_init(void);             //��һ�׶�  ��ʼ����ҪƬ������
void communication_module_init(void);                      //�ڶ��׶�  ��ʼ��ͨ��ģ��
void sensor_module_init(void);                             //�����׶�  ��ʼ��������ģ��
void other_initialization_procedure(void);                 //���Ľ׶�  ������ʼ������
uint8_t other_system_initialization_process(void);         //����ϵͳ��ʼ������
/******************************AP��Ӳ����ʼ��************************************/

/******************************·�������ӳ�ʼ��************************************/
void router_connection_initialization(void);               //AP����·�������ӳ�ʼ��
uint8_t get_STM32L4_chip_ID(uint8_t * STM32_ID_point);        //��ȡSTM32�ڲ���96λ��ƷΨһ��ݱ�ʶ
uint8_t read_connection_history_byte_from_eeprom(void);      //��ȡAP���Ƿ����ɹ���·�������������ӵ�EEPROM��־λ
void AP_side_registration_recovry(void);                   //AP���豸���״�ע����̣���ע��ָ�����
void AP_side_registration_for_first_time(void);            //AP���豸�״�ע����� 
uint8_t EEPROM_24C256_registration_recovry_read_procedure(void); //���״�ע��ʱ��EEPROM����ʷע����Ϣ
//uint8_t EEPROM_24C256_sensor_parameter_read(uint8_t sensor_count);   //EEPROM��ȡ��������صļĴ���
uint8_t EEPROM_24C256_sensor_parameter_read(uint8_t sensor_count, Linked_List *sensor_parameter_recovery_linked_list_header);   //EEPROM��ȡ��������صļĴ���
/******************************·�������ӳ�ʼ��************************************/

#endif
