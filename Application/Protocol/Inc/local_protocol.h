#ifndef __LOCAL_PROTOCOL_H__
#define __LOCAL_PROTOCOL_H__

#include "main.h"

/**
  * @brief  ��������Э�����ݰ�ͷ����  ��Ӧ������·������������Э���ֲ� v1.0.1��
  */
typedef struct {
	uint8_t message_header;                  //֡ͷ 0xFF
	uint8_t protocol_version;                //�汾��Ϣ 
	uint8_t message_serial_number;           //��Ϣ���
	uint8_t source_ID[2];                    //��Ϣ��Դ���豸ID
	uint8_t destination_ID[2];               //��ϢĿ�ĵص��豸ID
	uint8_t message_type;                    //��Ϣ���
	uint8_t date_entity_length;              //��Ϣ��Я�������ݳ���  
}ProtocolHeaderDefinition;

/**
  * @brief  ��������Э�����ݰ�����  ��Ӧ������·������������Э���ֲ� v1.0.1��
  */
typedef struct {
	ProtocolHeaderDefinition frame_header;                 //��Ϣͷ��
	uint8_t date_entity[255];                            //��Ϣ����
}ProtocolDefinition;

//��Ϣ���������  ��ӦЭ����е�type
//UP  AP->Router    DOWN  Router->AP
/**
  * @brief  ��������Э�����ݰ�����  �ɰ�ͷ�е�message_type���� ��Ӧ������·������������Э���ֲ� v1.0.1������UP�������ݰ���AP�˷���������·����  DOWN��ʾ���ݰ���·�����·���AP��
  */
typedef enum {
	PROTOCOL_DEVICE_REGISTERE_REQUEST_UP                  = 0x01,              //AP��ע������
	PROTOCOL_DEVICE_REGISTERE_RESPONCE_DOWN               = 0x02,              //·����ע����Ӧ
	PROTOCOL_SENSOR_PARAMETER_REQUEST_UP                  = 0x03,              //AP�˴�������������
	PROTOCOL_SENSOR_PARAMETER_RESPONCE_DOWN               = 0x04,              //·��������������ȷ��/����
	PROTOCOL_TIME_SYNC_REQUEST_UP                         = 0x05,              //AP��ʱ��ͬ������
	PROTOCOL_TIME_SYNC_RESPONCE_DOWN                      = 0x06,              //·����ʱ��ͬ��ȷ��                   
	PROTOCOL_DATA_ENTITY_UPLOAD_UP                        = 0x07,              //AP�������ϴ�
	PROTOCOL_DATA_ENTITY_ACK_DOWN                         = 0x08,              //·���������ϴ�ȷ��
	PROTOCOL_SYSTEM_WARNING_UPLOAD_UP                     = 0x09,              //AP�˸澯��Ϣ�ϱ�
	PROTOCOL_SYSTEM_WARNING_ACK_DOWN                      = 0x0A,              //·�����澯��Ϣȷ��
	
	PROTOCOL_PERIOD_CHANGE_REQUEST_DOWN                   = 0x10,              //·�������ڸ�������
	PROTOCOL_PERIOD_CHANGE_RESPONCE_UP                    = 0x11,              //AP�����ڸ���ȷ��
	PROTOCOL_SENSOR_PARAMETER_CHANGE_REQUEST_DOWN         = 0x12,              //·���������������������
	PROTOCOL_SENSOR_PARAMETER_CHANGE_RESPONCE_UP          = 0x13,              //AP�˴������������ȷ��
	PROTOCOL_COMMUNICATION_KEY_UPDATE_REQUEST_DOWN        = 0x14,              //·��������ͨ����Կ����
	PROTOCOL_COMMUNICATION_KEY_UPDATE_RESPONCE_UP         = 0x15,              //AP�˸���ͨ����Կȷ��	
	PROTOCOL_NETWORK_SWITCHING_REQUEST_DOWN               = 0x16,              //·����������������
	PROTOCOL_NETWORK_SWITCHING_RESPONCE_UP                = 0x17,              //AP����������ȷ��	
	PROTOCOL_REBOOT_REQUEST_DOWN                          = 0x18,              //·������������	
	PROTOCOL_REBOOT_RESPONCE_UP                           = 0x19,              //AP����������ȷ��	
	PROTOCOL_DEVICE_STATE_REQUEST_DOWN                    = 0x1A,              //·������ȡAP�豸״̬
	PROTOCOL_DEVICE_STATE_RESPONCE_UP                     = 0x1B,              //AP���豸״̬����		
	PROTOCOL_ENERGY_SAVING_MODE_REQUEST_DOWN              = 0x1C,              //·��������ģʽ��������
	PROTOCOL_ENERGY_SAVING_MODE_RESPONCE_UP               = 0x1E,              //AP�˽���ģʽ����ȷ��	
	
	PROTOCOL_FACTORY_SETTING_RESET_REQUEST_DOWN           = 0x30,              //·�����ָ�������������
	PROTOCOL_FACTORY_SETTING_RESET_RESPONCE_UP            = 0x31,              //AP�˻ָ���������ȷ��	
}ProtocolMessageTypeDefinition;			

#endif
