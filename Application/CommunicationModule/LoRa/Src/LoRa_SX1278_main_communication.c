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

/**
  * @name         message_send_to_router_by_lora_sx1278
  * @brief        ͨ��LoRa SX1278 ����Э��������Ϣ��·����
  * @param        void
  * @retval       ͨ��LoRa SX1278 Э��������Ϣ�Ƿ��ͳɹ����ɹ��򷵻�0��ʧ���򷵻�1
	* @lastModify   2018/9/29  19:14
	* @author       JackWilliam
  */
uint8_t message_send_to_router_by_lora_sx1278(uint8_t * message_entity, uint8_t message_entity_length){

	return 0;
}
