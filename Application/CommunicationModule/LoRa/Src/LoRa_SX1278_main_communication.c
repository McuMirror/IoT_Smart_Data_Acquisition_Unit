#include "LoRa_SX1278_main_communication.h"
#include "EEPROM_24C256.h"
#include "system_common.h"

tRadioDriver *Radio = NULL;                            //LoRa����ָ�붨��

/**
  * @name         LoRa_SX1278_Radio_Driver_Init
  * @brief        LoRa SX1278ģ��Ƶ�ʼ�������ʼ��
  * @param        void
  * @retval       LoRa SX1278ģ��Ƶ�ʼ������Ƿ��ʼ���ɹ�  �ɹ��򷵻�0  ʧ���򷵻�1
	* @lastModify   2018/9/25  15:27
	* @author       JackWilliam
  */
uint8_t LoRa_SX1278_Radio_Driver_Init(void){
	Radio = RadioDriverInit( );           //LoRa������ʼ��
	Radio->Init( );                        //LoRaƵ�ʳ�ʼ��
	
	return 0;
}
