#ifndef __SYSTEM_COMMON_H__
#define __SYSTEM_COMMON_H__

#include "main.h"
#include "stm32l4xx_hal.h"

extern com_mod_phy_connected communication_module_physical_connected;
extern sen_mod_phy_connected sensor_module_physical_connected;
extern sys_task_flag system_task_flag;
extern uint8_t unique_ID_STM32L4[12]; 
/**
  * @brief  ERROR_CODE ��������
  */
typedef enum {
	ERROR_CODE_GPIO_LED_Init_FAIL,                         //LED���GPIO�ܽų�ʼ��ʧ��
	ERROR_CODE_DEBUG_USART1_Init_FAIL,                     //���Դ���1��ʼ��ʧ��
	ERROR_CODE_IWDG_TIM3_Init_FAIL,                        //���ڶ������Ź�ι����TIM3��ʼ��ʧ��
	ERROR_CODE_IWDG_Init_FAIL,                             //�������Ź���ʼ��ʧ��
	ERROR_CODE_EEPROM_24C256_Init_FAIL,                    //EEPROM 24C256��ʼ��ʧ��
	ERROR_CODE_GPIO_DIP_Switch_Init_FAIL,                  //���뿪�����GPIO�ܽų�ʼ��ʧ��
	ERROR_CODE_GPIO_DIP_Switch_Com_Mod_Read_FAIL,          //��ȡͨ��ģ��ѡ����ص�GPIO��ʧ��
	ERROR_CODE_GPIO_LoRa_SX1278_Extra_Init_FAIL,           //LoRa SX1278ģ���SPIͨ�ű�Ҫ�ܽ������������GPIO�ܽų�ʼ��ʧ��
	ERROR_CODE_LoRa_SX1278_SPI_Init_FAIL,                  //LoRa SX1278ģ��SPI2��ʼ��ʧ��
	ERROR_CODE_LoRa_SX1278_Radio_Driver_Init_FAIL,         //LoRa SX1278ģ��Ƶ�ʼ�������ʼ��ʧ��
	ERROR_CODE_GPIO_WiFi_ESP8266_Extra_Init_FAIL,          //WiFi ESP8266ģ���USART3ͨ�ű�Ҫ�ܽ������������GPIO�ܽų�ʼ��ʧ��
	ERROR_CODE_WiFi_ESP8266_USART3_Init_FAIL,              //WiFi ESP8266ģ��USART3�����ʼ��ʧ��
	ERROR_CODE_WiFi_ESP8266_Driver_Init_FAIL,              //WiFi ESP8266ģ��������ʼ��ʧ��
	ERROR_CODE_BlueTooth_nRF52832_USART3_Init_FAIL,        //BlueTooth nRF52832ģ��USART3�����ʼ��ʧ��
	ERROR_CODE_BlueTooth_nRF52832_Driver_Init_FAIL,        //BlueTooth nRF52832ģ��������ʼ��ʧ��
	ERROR_CODE_NB_IoT_BC26_USART3_Init_FAIL,               //NB-IoT BC26ģ��USART3�����ʼ��ʧ��
	ERROR_CODE_NB_IoT_BC26_Driver_Init_FAIL,               //NB-IoT BC26ģ��������ʼ��ʧ��
	
	ERROR_CODE_GPIO_DIP_Switch_Sen_Mod_Read_FAIL,          //��ȡ������ģ��ѡ����ص�GPIO��ʧ��
	ERROR_CODE_Electric_Meter_USART2_Init_FAIL,            //SP3485ģ����ʹ�õ�USART2�ǳ�ʼ��ʧ��
	ERROR_CODE_Electric_Meter_Baudrate_Auto_Get_FAIL,      //��������Զ������ʻ�ȡʧ��
	ERROR_CODE_Other_System_Initialization_Process_FAIL,   //����ϵͳ��ʼ������ʧ��
	
	ERROR_CODE_Get_STM32L4_Chip_ID_FAIL,                   //��ȡSTM32�ڲ���96λ��ƷΨһ��ݱ�ʶʧ��
	
	ERROR_CODE_Read_Connection_History_Byte_From_EEPROM_FAIL,  //��ȡAP���Ƿ����ɹ���·�������������ӵ�EEPROM��־λʧ��
	
	ERROR_CODE_EEPROM_24C256_REGISTRATION_RECOVRY_READ_PROCEDURE_FAIL,  //���״�ע��ʱ��EEPROM����ʷע����Ϣʧ��
	
	ERROR_CODE_UART1_RX_BUFFER_OVERFLOW,                   //����1���ջ������
	ERROR_CODE_UART2_RX_BUFFER_OVERFLOW,                   //����2���ջ������
	ERROR_CODE_UART3_RX_BUFFER_OVERFLOW,                   //����3���ջ������
	
	ERROR_CODE_EEPROM_Read_Bytes_FAIL,                      //EEPROM��ȡ�ֽ�����ʧ��
	ERROR_CODE_EEPROM_Write_Bytes_FAIL,                     //EEPROMд���ֽ�����ʧ��
}ErrorCodeDefinition;

uint8_t error_code_handle(uint8_t error_code);                     //�����봦����

void init_instruction_resend_procedure(uint8_t resend_count);   //��ʼ��ָ���ط���λ����
uint8_t clear_alarm_and_count(uint8_t count_reg_exist, 
	  uint8_t * alarm_flag, uint8_t alarm_flag_addr, uint8_t alarm_flag_size, 
		uint8_t *  alarm_count, uint8_t alarm_count_addr, uint8_t alarm_count_size);    //��ո澯�Ĵ����еĸ澯λ������صļ�����

#endif
