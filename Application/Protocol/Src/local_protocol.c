#include "local_protocol.h"
#include "system_common.h"
#include "linked_list.h"
#include "sensor_electric_meter.h"
#include "rtc.h"
#include "tim.h"
#include "encyption_aes.h"
#include "encyption_util.h"

#define   PROTOCOL_HEADER               0xFF          //Э�����ݰ�֡ͷ
#define   PROTOCOL_VERSION              0x01          //Э�����ݰ��汾��
#define   PROTOCOL_MINIMUM_LENGTH       11            //Э�����ݰ���С���ȣ����������ݲ��ֵĳ���
#define   SENSOR_ADDRESS_FINDING_TIMES  10            //������Ѱַ�ظ�����
#define   MAX_ACK_MESSAGE_LEN           255           //������·������ACK���������

#define   ENCYPTION_ENABLE               1            //������Ϣ���ܽ���

extern SystemStateDefinition system_state_definition; 
extern reg_data_sensor_parm_and_ID    register_data_sensor_parm_and_ID;
extern Linked_List *memory_data_sensor_point_start_address;
extern RTC_HandleTypeDef hrtc;
extern tim_data_update_time_settings system_data_update_time_settings;
/********************************************************************************************************/
/*                            ��������        START                                                   */
/********************************************************************************************************/

/********************************************************************************************************/
/*                                                                                                      */
/*                      AP������������·������Э�鶨��    AP�˷���                                      */
/*                                   ����ο�ʼ                                                         */
/*                                                                                                      */
/*                                                                                                      */
/********************************************************************************************************/

/**
  * @name         protocol_main_package_encode
  * @brief        ��װ��Ҫ����·��������Э�����ݰ�
  * @param        *msg_serial_ID:ָ��Э���������е�����У�����ݰ�����˳��������ŵ�ָ��
  * @param        *srcID: ָ�������ݰ�һ�����豸ID��ָ��
  * @param        *destID: ָ��������ݰ�һ�����豸ID��ָ��
  * @param        type: Э�����ݰ����Ͷ���
  * @param        datalen: Э�����ݰ������ݲ�����ռ����
  * @param        *data: ָ��Э�����ݰ����ݲ��ֵ�ָ��
  * @param        *package_entity: ָ�����������ݰ���ƴ��ת����ɵ��ַ�����ָ��
  * @param        *package_len: ָ�����������ݰ���ƴ��ת����ɵ��ַ����ĳ��ȵ�ָ��
  * @retval       Э�����ݰ��Ƿ�ɹ��Ƴ�  �ɹ��򷵻�0  ʧ���򷵻�1
	* @author       JackWilliam
	* @lastModify   2018/9/26  17:20
  */
uint8_t protocol_main_package_encode(uint8_t * msg_serial_ID, uint8_t *srcID, uint8_t *destID, 
				uint8_t type, uint8_t datalen, uint8_t *data, uint8_t *package_entity, uint8_t *package_len)
{
	ProtocolDefinition package;
	
	uint8_t i;
	uint8_t len;
	static uint8_t package_sequence_number = 0x00;
	
#if ENCYPTION_ENABLE                  //ʹ���˼����㷨��ķ������
	uint8_t encyption_message_buf[MAX_ACK_MESSAGE_LEN];
	uint8_t j;
	AES_KEY aes_key;
	uint8_t cipher[MAX_ACK_MESSAGE_LEN];
#endif	
	
	
	//˳��ID
	package_sequence_number++;
	if(package_sequence_number == 0xFF) package_sequence_number = 0x00;
	if(package_sequence_number == 0x0D) package_sequence_number++;
	
	package.frame_header.message_header = PROTOCOL_HEADER;
	package.frame_header.protocol_version = PROTOCOL_VERSION;
	package.frame_header.message_serial_number = package_sequence_number;
	for(i = 0; i < 2; i++) package.frame_header.source_ID[i] = srcID[i];
	for(i = 0; i < 2; i++) package.frame_header.destination_ID[i] = destID[i];
	package.frame_header.message_type = type;
	package.frame_header.date_entity_length = datalen;
	for(i = 0; i < datalen; i++) package.date_entity[i] = data[i];
	
	
	package_entity[0] = package.frame_header.message_header;
	package_entity[1] = package.frame_header.protocol_version;
	package_entity[2] = package.frame_header.message_serial_number;
	for(i = 0; i < 2; i++) package_entity[3 + i] = package.frame_header.source_ID[i];
	for(i = 0; i < 2; i++) package_entity[5 + i] = package.frame_header.destination_ID[i];
	package_entity[7] = package.frame_header.message_type;
	package_entity[8] = package.frame_header.date_entity_length;
	for(i = 0; i < datalen; i++) package_entity[9 + i] = package.date_entity[i];
	
#if ENCYPTION_ENABLE
	if(datalen != 0){
		for(i = 9, j = 0; i < 9 + datalen; i++, j++)
			encyption_message_buf[j] = package_entity[i];
		AES_set_encrypt_key(register_data_communication_key.CommunicationKey, 128, &aes_key);
		AES_encrypt(encyption_message_buf, cipher, &aes_key);
		for(i = 9, j = 0; i < 9 + datalen; i++, j++)
			package_entity[i] = cipher[j];
	}
#endif
	

	package_entity[9 + datalen] = 0x0D;
	len = 9 + datalen + 1;
	
	/**********LoRa����ʱ  β���Դ�һ��0x0A   ����ͨ��ģ�����Դ�֡β********/
	if(communication_module_physical_connected.bluetooth_nrf52832_connected == 1 || 
		 communication_module_physical_connected.nb_iot_bc26_connected == 1 ||
	   communication_module_physical_connected.wifi_esp8266_connected == 1){
		package_entity[10 + datalen] = 0x0A;
		len++;	 
	}
//	len = 9 + datalen;

	*package_len = len;
	*msg_serial_ID = package_sequence_number;
	
	return 0;
}

/**
  * @name         protocol_device_registere_request_part_encode
  * @brief        ��װ��Ҫ����·������ע��������Э�����ݰ��е������ֶ�
  * @param        AP_device_unique_ID_len:AP�����ص�Ψһʶ����ĳ���
  * @param        *AP_device_unique_ID: ָ��AP�����ص�Ψһʶ�����ָ��
  * @param        communication_module_num: AP�������ӵĲ�ͬ��ͨ���豸������
  * @param        *communication_module_type: ָ��AP�������ӵĲ�ͬ��ͨ���豸���͵�ָ��
  * @param        sensor_module_num: AP�������ӵĲ�ͬ�Ĵ�����������
  * @param        *sensor_module_type: ָ��AP�������ӵĲ�ͬ�Ĵ��������͵�ָ��
  * @param        *part_entity: ָ�������ֶ���ƴ��ת����ɵ��ַ�����ָ��
  * @param        *part_len: ָ�������ֶ���ƴ��ת����ɵ��ַ����ĳ��ȵ�ָ��
  * @retval       Э�����ݰ��Ƿ�ɹ��Ƴ�  �ɹ��򷵻�0  ʧ���򷵻�1
	* @author       JackWilliam
	* @lastModify   2018/9/26  19:13
  */
uint8_t protocol_device_registere_request_part_encode(uint8_t AP_device_unique_ID_len,
			uint8_t * AP_device_unique_ID, uint8_t communication_module_num, uint8_t * communication_module_type, 
			uint8_t sensor_module_num, uint8_t * sensor_module_type, uint8_t *part_entity, uint8_t *part_len)
{	
	uint8_t i;
	uint8_t len;

	part_entity[0] = AP_device_unique_ID_len;
	for(i = 0; i < AP_device_unique_ID_len; i++) part_entity[i + 1] = AP_device_unique_ID[i];
	part_entity[AP_device_unique_ID_len + 1] = communication_module_num;
	for(i = 0; i < communication_module_num; i++) part_entity[AP_device_unique_ID_len + 2 + i] = communication_module_type[i];
	part_entity[AP_device_unique_ID_len + communication_module_num + 2] = sensor_module_num;
	for(i = 0; i < sensor_module_num; i++) part_entity[AP_device_unique_ID_len + communication_module_num + 3 + i] = sensor_module_type[i];
	
	len = AP_device_unique_ID_len + communication_module_num + sensor_module_num + 3;
	*part_len = len;

	return 0;
}

/**
  * @name         protocol_data_entity_upload_part_encode
  * @brief        ��װ��Ҫ����·�����������ϴ���Э�����ݰ��е������ֶ�
	* @param        *senser_data_entity_linked_list:ָ�������͸�·�����ĸ�������������ݰ�����
  * @param        *part_entity: ָ�������ֶ���ƴ��ת����ɵ��ַ�����ָ��
  * @param        *part_len: ָ�������ֶ���ƴ��ת����ɵ��ַ����ĳ��ȵ�ָ��
  * @retval       Э�����ݰ��Ƿ�ɹ��Ƴ�  �ɹ��򷵻�0  ʧ���򷵻�1
	* @author       JackWilliam
	* @lastModify   2018/9/27  09:50    
  */
uint8_t protocol_data_entity_upload_part_encode(Linked_List * senser_data_entity_linked_list, uint8_t *part_entity, uint8_t *part_len)
{	
	uint16_t i, j, k;
	uint16_t q = 0;
	uint8_t len;
	Linked_List *senser_data_entity_point;
	uint8_t sensor_count = GetNodeNum(senser_data_entity_linked_list);

	part_entity[0] = sensor_count;
	
	for(i = 0; i < sensor_count; i++){
		senser_data_entity_point = GetNode(senser_data_entity_linked_list, i);
		part_entity[1+q] = senser_data_entity_point->sensor_data_package_point->entity_length; q++;
		part_entity[1+q] = senser_data_entity_point->sensor_data_package_point->sensor_type;   q++;
		part_entity[1+q] = senser_data_entity_point->sensor_data_package_point->data_num;      q++;
		for(j = 0; j < senser_data_entity_point->sensor_data_package_point->data_num; j++){
			part_entity[1+j+q] = senser_data_entity_point->sensor_data_package_point->sensor_data_entity_detail_point->sensor_data_item_type; q++;
			part_entity[1+j+q] = senser_data_entity_point->sensor_data_package_point->sensor_data_entity_detail_point->sensor_data_item_length; q++;
			for(k = 0; k < senser_data_entity_point->sensor_data_package_point->sensor_data_entity_detail_point->sensor_data_item_length; k++){
				part_entity[1+j+k+q] = senser_data_entity_point->sensor_data_package_point->sensor_data_entity_detail_point->sensor_data_item_entity[k];
			}
			q += k;
		}
	}
	
	len = q;
	*part_len = len;
	
	return 0;
}


/**
  * @name         protocol_warning_upload_part_encode
  * @brief        ��װ��Ҫ����·�����ĸ澯��Ϣ�ϱ���Э�����ݰ��е������ֶ�
	* @param        warning_count:����·�����ϱ��ľ�������
	* @param        *warning_list:ָ������·�����ϱ��ľ���ʵ���ָ��
  * @param        *part_entity: ָ�������ֶ���ƴ��ת����ɵ��ַ�����ָ��
  * @param        *part_len: ָ�������ֶ���ƴ��ת����ɵ��ַ����ĳ��ȵ�ָ��
  * @retval       Э�����ݰ��Ƿ�ɹ��Ƴ�  �ɹ��򷵻�0  ʧ���򷵻�1
	* @author       JackWilliam
	* @lastModify   2018/9/27  09:50    
  */
uint8_t protocol_warning_upload_part_encode(uint8_t warning_count, uint8_t * warning_list, uint8_t *part_entity, uint8_t *part_len)
{	
	uint16_t i;
	uint8_t len;

	part_entity[0] = warning_count;
	
	for(i = 0; i < warning_count; i++) part_entity[1 + i] = warning_list[i];
	
	len = warning_count + 1;
	*part_len = len;
	
	return 0;
}
/********************************************************************************************************/
/*************************AP������������·������Э�鶨��   ����ν���************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/*                                                                                                      */
/*                        AP������������·������Э�鶨��    AP�˷���                                    */
/*                                   ����ο�ʼ                                                         */
/*                                                                                                      */
/*                                                                                                      */
/********************************************************************************************************/
/**
  * @name         protocol_system_status_upload_part_encode(������)
  */
uint8_t protocol_system_status_upload_part_encode()
{	
	
	return 0;
}

/********************************************************************************************************/
/***********************AP������������·������Э�鶨��    ·��������   ����ν���************************/
/********************************************************************************************************/

/********************************************************************************************************/
/*                            ��������        END                                                     */
/********************************************************************************************************/



/********************************************************************************************************/
/*                            ��������        START                                                   */
/********************************************************************************************************/
/********************************************************************************************************/
/*                                                                                                      */
/*                        ·��������������AP�˶�Э�鶨��    AP�˷���                                    */
/*                                   ����ο�ʼ                                                         */
/*                                                                                                      */
/*                                                                                                      */
/********************************************************************************************************/
/**
  * @name         protocol_device_registere_responce_part_decode
  * @brief        ·������������Э�����ݰ���ע����Ӧ����ʵ�����
  * @param        *package_entity: ָ����յ������ݰ�����ʵ�岿�ֵ�ָ��
  * @param        package_len: ���յ������ݰ�ʵ�岿�ֵĳ���
  * @retval       Э�����ݰ�ʵ�岿���Ƿ�ɹ�����  �ɹ��򷵻�0  ʧ���򷵻�1
	* @author       JackWilliam
	* @lastModify   2018/9/26  17:20
  */
uint8_t protocol_device_registere_responce_part_decode(uint8_t *package_entity, uint8_t package_len){
	register_data_basic_settings.DataExistFlag = 0x01;
	register_data_basic_settings.MasterID[0] = package_entity[3];
	register_data_basic_settings.MasterID[1] = package_entity[4];
	register_data_basic_settings.SlaveID[0] = package_entity[5];
	register_data_basic_settings.SlaveID[1] = package_entity[6];
	
	//�������ݶ�ȡ�ɹ���ͳһд��
	if(ee_WriteBytes(&register_data_basic_settings.DataExistFlag, register_address_basic_settings.DataExistFlag, 1))
		error_code_handle(ERROR_CODE_EEPROM_Write_Bytes_FAIL);
	if(ee_WriteBytes(register_data_basic_settings.MasterID, register_address_basic_settings.MasterID, 2))
		error_code_handle(ERROR_CODE_EEPROM_Write_Bytes_FAIL);
	if(ee_WriteBytes(register_data_basic_settings.SlaveID, register_address_basic_settings.SlaveID, 2))
		error_code_handle(ERROR_CODE_EEPROM_Write_Bytes_FAIL);
	
	system_state_definition = SYSTEM_STATE_REGISTERED;
	
	return 0;
}

/**
  * @name         protocol_sensor_parameter_responce_part_decode
  * @brief        ·������������Э�����ݰ��д���������ȷ��/��������ʵ�����
  * @param        *package_entity: ָ����յ������ݰ�����ʵ�岿�ֵ�ָ��
  * @param        package_len: ���յ������ݰ�ʵ�岿�ֵĳ���
  * @retval       Э�����ݰ�ʵ�岿���Ƿ�ɹ�����  �ɹ��򷵻�0  ʧ���򷵻�1
	* @author       JackWilliam
	* @lastModify   2018/9/29  15:25
  */
uint8_t protocol_sensor_parameter_responce_part_decode(uint8_t *package_entity, uint8_t package_len, Linked_List *sensor_parameter_recovery_linked_list_header){
//	reg_data_sensor_parm sensor_parm;
//	reg_data_sensor_ID sensor_ID;
	uint16_t offset_address = 0;                                      //EEPROMƫ�Ƶ�ַ 
	
	uint8_t sensor_parameter_message_length = 0;       //���������ò����ܳ���
	uint8_t start_stone = 0;                           //ÿ�����ݶ������
	uint8_t i, j;


	register_data_basic_settings.SensorNum = package_entity[9];
	register_data_basic_settings.SenConAuto = package_entity[10];
	
	if(register_data_basic_settings.SenConAuto == 0x01){
		register_data_basic_settings.ComModuleType = package_entity[12];
		register_data_sensor_parm.SensorType = SensorType_Electric_Meter_Connected;
		register_data_sensor_parm.SensorParaNum = 1;
		register_data_sensor_parm.SensorIDLen = 6;
		register_data_sensor_parm.sensorParameter[0] = 0x01;
		
		electric_meter_address_auto_get(register_data_sensor_ID.sensorID, SENSOR_ADDRESS_FINDING_TIMES);  //���ID�Զ���ȡ
	}
	else if(register_data_basic_settings.SenConAuto == 0x02){
		start_stone = 11;

		for(i = 0; i < register_data_basic_settings.SensorNum; i++){
			sensor_parameter_recovery_linked_list_header->sensor_data_package_point = NULL;
			sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID = mymalloc(sizeof(sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID));
			sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID->sensor_parm = mymalloc(sizeof(sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID->sensor_parm));
			sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID->sensor_ID = mymalloc(sizeof(sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID->sensor_ID));
			
			/*************������������****************/
			sensor_parameter_message_length = package_entity[start_stone];
			sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID->sensor_parm->SensorType = package_entity[start_stone + 1];
			sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID->sensor_parm->SensorIDLen = package_entity[start_stone + 3];
			for(j = 0; j < sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID->sensor_parm->SensorIDLen; j++) sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID->sensor_ID->sensorID[j] = package_entity[start_stone + 4 + j];
			sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID->sensor_parm->SensorParaNum = package_entity[start_stone + sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID->sensor_parm->SensorIDLen + 4];
			
			memset(sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID->sensor_parm->sensorParameter, 0, sizeof(sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID->sensor_parm->sensorParameter));
			for(j = 0; j < sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID->sensor_parm->SensorParaNum; j++) {                      //��ʱֻ�����������й�������ת��
				switch(package_entity[start_stone + sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID->sensor_parm->SensorIDLen + 5 + j]){
					case 0x01:
						sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID->sensor_parm->sensorParameter[0] |= 0x01;
						break;
				}
			}
			
			if(register_data_basic_settings.SensorNum != 0) InsertNode(sensor_parameter_recovery_linked_list_header, TAIL);    //��β������ڵ�
			start_stone += sensor_parameter_message_length;
			/*************������������****************/
			
			
			/*************���ݴ洢����  д��EEPROM****************/
			if(ee_WriteBytes(&sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID->sensor_parm->SensorType, offset_address + register_address_sensor_parm.SensorType, 1))
				error_code_handle(ERROR_CODE_EEPROM_Write_Bytes_FAIL);
			offset_address += AP_BYTE_OFFSET;
			
			offset_address += AP_BYTE_OFFSET;
			
			if(ee_WriteBytes(&sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID->sensor_parm->SensorIDLen, offset_address + register_address_sensor_parm.SensorIDLen, 1))
				error_code_handle(ERROR_CODE_EEPROM_Write_Bytes_FAIL);
			offset_address += AP_BYTE_OFFSET;
			
			offset_address += AP_BYTE_OFFSET * 5;
			
			if(ee_WriteBytes(sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID->sensor_parm->sensorParameter, offset_address + register_address_sensor_parm.sensorParameter, 8))
				error_code_handle(ERROR_CODE_EEPROM_Write_Bytes_FAIL);
			offset_address += AP_BYTE_OFFSET * 8;
			
			if(ee_WriteBytes(sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID->sensor_ID->sensorID, offset_address + register_address_sensor_ID.sensorID - 16, 16))
				error_code_handle(ERROR_CODE_EEPROM_Write_Bytes_FAIL);
			offset_address += AP_BYTE_OFFSET * 16;
			/*************���ݴ洢����  д��EEPROM****************/
		}
	
	}
	
	system_state_definition = SYSTEM_STATE_SENSOR_PARAMETER_GET_DOWN;
	
	return 0;
}

/**
  * @name         protocol_time_sync_responce_part_decode
  * @brief        ·������������Э�����ݰ���ʱ��ͬ��ȷ�Ͻ���
  * @param        *package_entity: ָ����յ������ݰ�����ʵ�岿�ֵ�ָ��
  * @param        package_len: ���յ������ݰ�ʵ�岿�ֵĳ���
  * @retval       Э�����ݰ�ʵ�岿���Ƿ�ɹ�����  �ɹ��򷵻�0  ʧ���򷵻�1
	* @author       JackWilliam
	* @lastModify   2018/9/29  15:25
  */
uint8_t protocol_time_sync_responce_part_decode(uint8_t *package_entity, uint8_t package_len){
	//ʱ���ʽУ��	
		if(package_entity[9] > 0x99 || 
		 package_entity[10] < 0x01 || package_entity[10] > 0x12 ||
  	 package_entity[11] < 0x01   || package_entity[11] > 0x31 || 
		 package_entity[12] > 0x24 ||
		 package_entity[13] > 0x60 ||
		 package_entity[14] > 0x60 ||
		 package_entity[15] < 0x01   || package_entity[15] > 0x07) return 1;  
//	if(package_entity[9] < 0x00 || package_entity[9] > 0x99 || 
//		 package_entity[10] < 0x01 || package_entity[10] > 0x12 ||
//  	 package_entity[11] < 0x01   || package_entity[11] > 0x31 || 
//		 package_entity[12] < 0x00   || package_entity[12] > 0x24 ||
//		 package_entity[13] < 0x00 || package_entity[13] > 0x60 ||
//		 package_entity[14] < 0x00 || package_entity[14] > 0x60 ||
//		 package_entity[15] < 0x01   || package_entity[15] > 0x07) return 1;  
	
	register_data_time_settings.Year = package_entity[9];
	register_data_time_settings.Month = package_entity[10];
	register_data_time_settings.Day = package_entity[11];
	register_data_time_settings.Hour = package_entity[12];
	register_data_time_settings.Minute = package_entity[13];
	register_data_time_settings.Second = package_entity[14];
	register_data_time_settings.Week = package_entity[15];
	
	if(RTC_CalendarConfig(&hrtc, &register_data_time_settings))
		error_code_handle(ERROR_CODE_RTC_Init_Set_FAIL);
	
	register_data_time_settings.GapTime[0] = package_entity[16];
	register_data_time_settings.GapTime[1] = package_entity[17];
	register_data_time_settings.GapTime[2] = package_entity[18];
	
	register_data_time_settings.WindowLength = package_entity[19];
       
	system_data_update_time_settings.gap_time = (uint32_t)register_data_time_settings.GapTime[0] << 16 | 
																							(uint32_t)register_data_time_settings.GapTime[1] << 8  |
	                                            (uint32_t)register_data_time_settings.GapTime[2];
	
	system_data_update_time_settings.window_length = register_data_time_settings.WindowLength;
	
	if(system_data_update_time_settings.window_length > system_data_update_time_settings.gap_time){
		error_code_handle(ERROR_CODE_Data_UpdateTime_Settings_Set_FAIL);
		return 1;
	}
	
	//�������ݶ�ȡ�ɹ���ͳһд��
	if(ee_WriteBytes(&register_data_time_settings.Year, register_address_time_settings.Year, 1))
		error_code_handle(ERROR_CODE_EEPROM_Write_Bytes_FAIL);
	if(ee_WriteBytes(&register_data_time_settings.Month, register_address_time_settings.Month, 1))
		error_code_handle(ERROR_CODE_EEPROM_Write_Bytes_FAIL);
	if(ee_WriteBytes(&register_data_time_settings.Day, register_address_time_settings.Day, 1))
		error_code_handle(ERROR_CODE_EEPROM_Write_Bytes_FAIL);
	if(ee_WriteBytes(&register_data_time_settings.Hour, register_address_time_settings.Hour, 1))
		error_code_handle(ERROR_CODE_EEPROM_Write_Bytes_FAIL);
	if(ee_WriteBytes(&register_data_time_settings.Minute, register_address_time_settings.Minute, 1))
		error_code_handle(ERROR_CODE_EEPROM_Write_Bytes_FAIL);
	if(ee_WriteBytes(&register_data_time_settings.Second, register_address_time_settings.Second, 1))
		error_code_handle(ERROR_CODE_EEPROM_Write_Bytes_FAIL);
	if(ee_WriteBytes(&register_data_time_settings.Week, register_address_time_settings.Week, 1))
		error_code_handle(ERROR_CODE_EEPROM_Write_Bytes_FAIL);
	if(ee_WriteBytes(register_data_time_settings.GapTime, register_address_time_settings.GapTime, 3))
		error_code_handle(ERROR_CODE_EEPROM_Write_Bytes_FAIL);
	if(ee_WriteBytes(&register_data_time_settings.WindowLength, register_address_time_settings.WindowLength, 1))
		error_code_handle(ERROR_CODE_EEPROM_Write_Bytes_FAIL);

	system_state_definition = SYSTEM_STATE_TIME_SYNC_ACK_GET_DOWN;
	
	return 0;
}

/**
  * @name         protocol_data_entity_ack_part_decode
  * @brief        ·������������Э�����ݰ��������ϴ�ȷ�Ͻ���
  * @param        *package_entity: ָ����յ������ݰ�����ʵ�岿�ֵ�ָ��
  * @param        package_len: ���յ������ݰ�ʵ�岿�ֵĳ���
  * @retval       Э�����ݰ�ʵ�岿���Ƿ�ɹ�����  �ɹ��򷵻�0  ʧ���򷵻�1
	* @author       JackWilliam
	* @lastModify   2018/9/26  17:20
  */
uint8_t protocol_data_entity_ack_part_decode(uint8_t *package_entity, uint8_t package_len){
	//�������κν������
	system_state_definition = SYSTEM_STATE_DATA_ENTITY_ACK_GET_DOWN;
	
	return 0;
}

/**
  * @name         protocol_system_warning_ack_part_decode
  * @brief        ·������������Э�����ݰ��и澯��Ϣȷ�Ͻ���
  * @param        *package_entity: ָ����յ������ݰ�����ʵ�岿�ֵ�ָ��
  * @param        package_len: ���յ������ݰ�ʵ�岿�ֵĳ���
  * @retval       Э�����ݰ�ʵ�岿���Ƿ�ɹ�����  �ɹ��򷵻�0  ʧ���򷵻�1
	* @author       JackWilliam
	* @lastModify   2018/9/26  17:20
  */
uint8_t protocol_system_warning_ack_part_decode(uint8_t *package_entity, uint8_t package_len){
	//�������κν������
	
	return 0;
}



/*******************************��������������ݰ���***********************************/


/**
  * @name         protocol_period_change_request_decode
  * @brief        ·������������Э�����ݰ������ڸ����������
  * @param        *package_entity: ָ����յ������ݰ�����ʵ�岿�ֵ�ָ��
  * @param        package_len: ���յ������ݰ�ʵ�岿�ֵĳ���
  * @retval       Э�����ݰ�ʵ�岿���Ƿ�ɹ�����  �ɹ��򷵻�0  ʧ���򷵻�1
	* @author       JackWilliam
	* @lastModify   2018/9/26  17:20
  */
uint8_t protocol_period_change_request_decode(uint8_t package_serial_ID, uint8_t *package_entity, uint8_t package_len){
	uint8_t ack_message_entity[MAX_ACK_MESSAGE_LEN];
	uint8_t ack_message_length;
	
	if(protocol_time_sync_responce_part_decode(package_entity, package_len))
			error_code_handle(ERROR_CODE_Period_Change_FAIL);
	
	protocol_main_package_encode(&package_serial_ID, register_data_basic_settings.SlaveID, register_data_basic_settings.MasterID,
			PROTOCOL_PERIOD_CHANGE_RESPONCE_UP, 0, NULL, ack_message_entity, &ack_message_length);
	message_send_to_router(ack_message_entity, ack_message_length);
	
	return 0;
}


/**
  * @name         protocol_sensor_parameter_change_request_part_decode
  * @brief        ·������������Э�����ݰ��д�������������������
  * @param        *package_entity: ָ����յ������ݰ�����ʵ�岿�ֵ�ָ��
  * @param        package_len: ���յ������ݰ�ʵ�岿�ֵĳ���
  * @retval       Э�����ݰ�ʵ�岿���Ƿ�ɹ�����  �ɹ��򷵻�0  ʧ���򷵻�1
	* @author       JackWilliam
	* @lastModify   2018/9/26  17:20
  */
uint8_t protocol_sensor_parameter_change_request_part_decode(uint8_t package_serial_ID, uint8_t *package_entity, uint8_t package_len, Linked_List *sensor_parameter_recovery_linked_list_header){
	uint8_t ack_message_entity[MAX_ACK_MESSAGE_LEN];
	uint8_t ack_message_length;
	
	if(protocol_sensor_parameter_responce_part_decode(package_entity, package_len, sensor_parameter_recovery_linked_list_header))
		error_code_handle(ERROR_CODE_Sensor_Parameter_Change_FAIL);
	
	protocol_main_package_encode(&package_serial_ID, register_data_basic_settings.SlaveID, register_data_basic_settings.MasterID,
			PROTOCOL_SENSOR_PARAMETER_CHANGE_RESPONCE_UP, 0, NULL, ack_message_entity, &ack_message_length);
	message_send_to_router(ack_message_entity, ack_message_length);
	
	return 0;
}

/**
  * @name         protocol_communication_key_update_request_part_decode
  * @brief        ·������������Э�����ݰ��и���ͨ����Կ�������
  * @param        *package_entity: ָ����յ������ݰ�����ʵ�岿�ֵ�ָ��
  * @param        package_len: ���յ������ݰ�ʵ�岿�ֵĳ���
  * @retval       Э�����ݰ�ʵ�岿���Ƿ�ɹ�����  �ɹ��򷵻�0  ʧ���򷵻�1
	* @author       JackWilliam
	* @lastModify   2018/9/26  17:20
  */
uint8_t protocol_communication_key_update_request_part_decode(uint8_t package_serial_ID, uint8_t *package_entity, uint8_t package_len){
	uint8_t i;
	uint8_t ack_message_entity[MAX_ACK_MESSAGE_LEN];
	uint8_t ack_message_length;
	
	for(i = 0; i < 16; i++) register_data_communication_key.CommunicationKey[i] = package_entity[9 + i];
	
	if(ee_WriteBytes(register_data_communication_key.CommunicationKey, register_address_communication_key.CommunicationKey, 16))
		error_code_handle(ERROR_CODE_EEPROM_Write_Bytes_FAIL);
	
	protocol_main_package_encode(&package_serial_ID, register_data_basic_settings.SlaveID, register_data_basic_settings.MasterID,
			PROTOCOL_COMMUNICATION_KEY_UPDATE_RESPONCE_UP, 0, NULL, ack_message_entity, &ack_message_length);
	message_send_to_router(ack_message_entity, ack_message_length);
	
	return 0;
}

/**
  * @name         protocol_network_switching_request_part_decode(��δ����������)
  * @brief        ·������������Э�����ݰ������������������(��δ����������)
  * @param        *package_entity: ָ����յ������ݰ�����ʵ�岿�ֵ�ָ��
  * @param        package_len: ���յ������ݰ�ʵ�岿�ֵĳ���
  * @retval       Э�����ݰ�ʵ�岿���Ƿ�ɹ�����  �ɹ��򷵻�0  ʧ���򷵻�1
	* @author       JackWilliam
	* @lastModify   2018/9/26  17:20
  */
uint8_t protocol_network_switching_request_part_decode(uint8_t package_serial_ID, uint8_t *package_entity, uint8_t package_len){
	//�ù�����δ����������
	
	return 0;
}

/**
  * @name         protocol_reboot_request_part_decode
  * @brief        ·������������Э�����ݰ��������������
  * @param        *package_entity: ָ����յ������ݰ�����ʵ�岿�ֵ�ָ��
  * @param        package_len: ���յ������ݰ�ʵ�岿�ֵĳ���
  * @retval       Э�����ݰ�ʵ�岿���Ƿ�ɹ�����  �ɹ��򷵻�0  ʧ���򷵻�1
	* @author       JackWilliam
	* @lastModify   2018/9/26  17:20
  */
uint8_t protocol_reboot_request_part_decode(uint8_t package_serial_ID, uint8_t *package_entity, uint8_t package_len){
	uint8_t ack_message_entity[MAX_ACK_MESSAGE_LEN];
	uint8_t ack_message_length;
	
	protocol_main_package_encode(&package_serial_ID, register_data_basic_settings.SlaveID, register_data_basic_settings.MasterID,
			PROTOCOL_REBOOT_RESPONCE_UP, 0, NULL, ack_message_entity, &ack_message_length);
	message_send_to_router(ack_message_entity, ack_message_length);
	
	NVIC_SystemReset();
	
	return 0;
}

/**
  * @name         protocol_device_state_request_part_decode
  * @brief        ·������������Э�����ݰ��л�ȡAP�豸״̬����
  * @param        *package_entity: ָ����յ������ݰ�����ʵ�岿�ֵ�ָ��
  * @param        package_len: ���յ������ݰ�ʵ�岿�ֵĳ���
  * @retval       Э�����ݰ�ʵ�岿���Ƿ�ɹ�����  �ɹ��򷵻�0  ʧ���򷵻�1
	* @author       JackWilliam
	* @lastModify   2018/9/26  17:20
  */
uint8_t protocol_device_state_request_part_decode(uint8_t package_serial_ID, uint8_t *package_entity, uint8_t package_len){
		//�ù�����δ����������
	
	return 0;
}

/**
  * @name         protocol_energy_saving_mode_request_part_decode
  * @brief        ·������������Э�����ݰ��н���ģʽ�����������
  * @param        *package_entity: ָ����յ������ݰ�����ʵ�岿�ֵ�ָ��
  * @param        package_len: ���յ������ݰ�ʵ�岿�ֵĳ���
  * @retval       Э�����ݰ�ʵ�岿���Ƿ�ɹ�����  �ɹ��򷵻�0  ʧ���򷵻�1
	* @author       JackWilliam
	* @lastModify   2018/9/26  17:20
  */
uint8_t protocol_energy_saving_mode_request_part_decode(uint8_t package_serial_ID, uint8_t *package_entity, uint8_t package_len){
	uint8_t ack_message_entity[MAX_ACK_MESSAGE_LEN];
	uint8_t ack_message_length;
	
	register_data_basic_settings.SavePower = package_entity[9];
	
	if(ee_WriteBytes(&register_data_basic_settings.SavePower, register_address_basic_settings.SavePower, 1))
		error_code_handle(ERROR_CODE_EEPROM_Write_Bytes_FAIL);
	
	protocol_main_package_encode(&package_serial_ID, register_data_basic_settings.SlaveID, register_data_basic_settings.MasterID,
		PROTOCOL_ENERGY_SAVING_MODE_RESPONCE_UP, 0, NULL, ack_message_entity, &ack_message_length);
	message_send_to_router(ack_message_entity, ack_message_length);
	
	return 0;
}

/**
  * @name         protocol_factory_setting_reset_request_part_decode
  * @brief        ·������������Э�����ݰ��лָ����������������
  * @param        *package_entity: ָ����յ������ݰ�����ʵ�岿�ֵ�ָ��
  * @param        package_len: ���յ������ݰ�ʵ�岿�ֵĳ���
  * @retval       Э�����ݰ�ʵ�岿���Ƿ�ɹ�����  �ɹ��򷵻�0  ʧ���򷵻�1
	* @author       JackWilliam
	* @lastModify   2018/9/26  17:20
  */
uint8_t protocol_factory_setting_reset_request_part_decode(uint8_t package_serial_ID, uint8_t *package_entity, uint8_t package_len, Linked_List *sensor_parameter_recovery_linked_list_header){
	uint8_t i;
	uint8_t sensor_node_num = 0;
	uint8_t ack_message_entity[MAX_ACK_MESSAGE_LEN];
	uint8_t ack_message_length;
	
	ee_ReadBytes(register_data_wipe_key.WipeKey, register_address_wipe_key.WipeKey, 16);
	for(i = 0; i < 16; i++){
		if(register_data_wipe_key.WipeKey[i] != package_entity[9 + i]){
			error_code_handle(ERROR_CODE_Wipe_Key_Check_FAIL);
			return 1;
		}
	}
	
	protocol_main_package_encode(&package_serial_ID, register_data_basic_settings.SlaveID, register_data_basic_settings.MasterID,
		PROTOCOL_FACTORY_SETTING_RESET_RESPONCE_UP, 0, NULL, ack_message_entity, &ack_message_length);
	message_send_to_router(ack_message_entity, ack_message_length);
	
	sensor_node_num = GetNodeNum(sensor_parameter_recovery_linked_list_header);
	
	ee_Erase(AP_REG_BASIC_SETTINGS, (5 * 16 + 2 * 16 * sensor_node_num));
	NVIC_SystemReset();
	
	return 0;
}



/**
  * @name         protocol_main_package_decode
  * @brief        ������·������������Э�����ݰ�
  * @param        package_serial_ID:Э���������е�����У�����ݰ�����˳���������
  * @param        *package_entity: ָ����յ������ݰ�ʵ���ָ��
  * @param        package_len: ���յ������ݰ��ĳ���
  * @retval       Э�����ݰ��Ƿ�ɹ�����  �ɹ��򷵻�0  ʧ���򷵻�1
	* @author       JackWilliam
	* @lastModify   2018/9/26  17:20
  */
uint8_t protocol_main_package_decode(uint8_t package_serial_ID, uint8_t *package_entity, uint8_t package_len)
{
#if ENCYPTION_ENABLE                  //ʹ���˼����㷨��Ľ������
	uint8_t decyption_message_buf[MAX_ACK_MESSAGE_LEN];
	uint8_t i, j;
	AES_KEY aes_key;
	uint8_t datelen = 0;
	uint8_t cipher[MAX_ACK_MESSAGE_LEN];
#endif	
	
	/***************���ݰ�У��********************/
	//�յ������ݰ�����С����СЭ�������
	if(package_len < PROTOCOL_MINIMUM_LENGTH){ 
		error_code_handle(ERROR_CODE_Protocol_Main_Package_Decode_Header_Length_Check_FAIL);   
		return 1;
	}  
  
	//�յ������ݰ���֡ͷ����ȷ	
	if(package_entity[0] != PROTOCOL_HEADER){ 
		error_code_handle(ERROR_CODE_Protocol_Main_Package_Decode_Header_Top_Check_FAIL);   
		return 1;
	}   
	
	//�յ������ݰ��İ汾�Ų���ȷ
	if(package_entity[1] != PROTOCOL_VERSION){ 
		error_code_handle(ERROR_CODE_Protocol_Main_Package_Decode_Header_Version_Check_FAIL);   
		return 1;
	}    
	
	//�յ������ݰ�����Ų���ȷ	
	if(package_entity[2] != package_serial_ID){ 
		error_code_handle(ERROR_CODE_Protocol_Main_Package_Decode_Header_Serial_ID_Check_FAIL);   
		return 1;
	}    
	
	if(system_state_definition != SYSTEM_STATE_UNREGISTERED){   
	//��ע��״̬���յ������ݰ�Դ��ַ����		
		if(package_entity[3] != register_data_basic_settings.MasterID[0]  || 
				package_entity[4] != register_data_basic_settings.MasterID[1]){
			error_code_handle(ERROR_CODE_Protocol_Main_Package_Decode_Header_Master_ID_Check_FAIL);   
			return 1;
				
		}
	
	//��ע��״̬���յ������ݰ�Ŀ�ĵ�ַ����		
		if(package_entity[5] != register_data_basic_settings.SlaveID[0]  || 
				package_entity[6] != register_data_basic_settings.SlaveID[1]){ 
					error_code_handle(ERROR_CODE_Protocol_Main_Package_Decode_Header_Slave_ID_Check_FAIL);   
					return 1;
		}  
	}	
	
	
#if ENCYPTION_ENABLE                  //ʹ���˼����㷨��Ľ������
	datelen = package_entity[8];
	if(datelen != 0){
		for(i = 9, j = 0; i < 9 + datelen; i++, j++)
			cipher[j] = package_entity[i];
		AES_set_decrypt_key(register_data_communication_key.CommunicationKey, 128, &aes_key);
		AES_decrypt(cipher, decyption_message_buf, &aes_key);
		for(i = 9, j = 0; i < 9 + datelen; i++, j++)
			package_entity[i] = decyption_message_buf[j];
	}
#endif
	
	/***************���ݰ�У��********************/

		
	/***************���ݰ�����  ͨ��Э���е�Typeλ********************/
	switch(package_entity[7]){
		//AP����������
		case PROTOCOL_DEVICE_REGISTERE_RESPONCE_DOWN:
			if(protocol_device_registere_responce_part_decode(package_entity, package_len))
				error_code_handle(ERROR_CODE_Protocol_Device_Registere_Responce_Part_Decode_FAIL);   //·����ע����Ӧ����ʧ��
			break;
	
		case PROTOCOL_SENSOR_PARAMETER_RESPONCE_DOWN:
			if(protocol_sensor_parameter_responce_part_decode(package_entity, package_len, memory_data_sensor_point_start_address))
				error_code_handle(ERROR_CODE_Protocol_Sensor_Parameter_Responce_Part_Decode_FAIL);   //·��������������ȷ��/����ʧ��			
			break;
	
		case PROTOCOL_TIME_SYNC_RESPONCE_DOWN:
			if(protocol_time_sync_responce_part_decode(package_entity, package_len))
				error_code_handle(ERROR_CODE_Protocol_Time_Sync_Responce_Part_Decode_FAIL);   //·����ʱ��ͬ��ȷ��ʧ��			
			break;

		case PROTOCOL_DATA_ENTITY_ACK_DOWN:
			if(protocol_data_entity_ack_part_decode(package_entity, package_len))
				error_code_handle(ERROR_CODE_Protocol_Data_Entity_Ack_Decode_FAIL);                 //·���������ϴ�ȷ��ʧ��			
			break;	

		case PROTOCOL_SYSTEM_WARNING_ACK_DOWN:
			if(protocol_system_warning_ack_part_decode(package_entity, package_len))
				error_code_handle(ERROR_CODE_Protocol_System_Warning_Ack_Part_Decode_FAIL);   //·�����澯��Ϣȷ��ʧ��			
			break;	
			
		//·������������
		case PROTOCOL_PERIOD_CHANGE_REQUEST_DOWN:
			if(protocol_period_change_request_decode(package_serial_ID, package_entity, package_len))
				error_code_handle(ERROR_CODE_Protocol_Period_Change_Request_Part_Decode_FAIL);    //·�������ڸ�������ʧ��			
			break;

		case PROTOCOL_SENSOR_PARAMETER_CHANGE_REQUEST_DOWN:
			if(protocol_sensor_parameter_change_request_part_decode(package_serial_ID, package_entity, package_len, memory_data_sensor_point_start_address))
				error_code_handle(ERROR_CODE_Protocol_Sensor_Parameter_Change_Request_Part_Decode_FAIL);   //·���������������������ʧ��			
			break;	

		case PROTOCOL_COMMUNICATION_KEY_UPDATE_REQUEST_DOWN:
			if(protocol_communication_key_update_request_part_decode(package_serial_ID, package_entity, package_len))
				error_code_handle(ERROR_CODE_Protocol_Communication_Key_Update_Request_Part_Decode_FAIL);   //·��������ͨ����Կ����ʧ��			
			break;	

		case PROTOCOL_NETWORK_SWITCHING_REQUEST_DOWN:
			if(protocol_network_switching_request_part_decode(package_serial_ID, package_entity, package_len))
				error_code_handle(ERROR_CODE_Protocol_Network_Switching_Request_Part_Decode_FAIL);   //·����������������ʧ��			
			break;		
		
		case PROTOCOL_REBOOT_REQUEST_DOWN:
			if(protocol_reboot_request_part_decode(package_serial_ID, package_entity, package_len))
				error_code_handle(ERROR_CODE_Protocol_Reboot_Request_Part_Decode_FAIL);   //·������������ʧ��			
			break;

		case PROTOCOL_DEVICE_STATE_REQUEST_DOWN:
			if(protocol_device_state_request_part_decode(package_serial_ID, package_entity, package_len))
				error_code_handle(ERROR_CODE_Protocol_Device_State_Request_Part_Decode_FAIL);   //·������ȡAP�豸״̬ʧ��			
			break;

		case PROTOCOL_ENERGY_SAVING_MODE_REQUEST_DOWN:
			if(protocol_energy_saving_mode_request_part_decode(package_serial_ID, package_entity, package_len))
				error_code_handle(ERROR_CODE_Protocol_Energy_Saving_Mode_Request_Part_Decode_FAIL);    //·��������ģʽ��������ʧ��			
			break;		
		
		case PROTOCOL_FACTORY_SETTING_RESET_REQUEST_DOWN:
			if(protocol_factory_setting_reset_request_part_decode(package_serial_ID, package_entity, package_len, memory_data_sensor_point_start_address))
				error_code_handle(ERROR_CODE_Protocol_Factory_Setting_Reset_Request_Part_Decode_FAIL);    //·�����ָ�������������ʧ��			
			break;	
		
	}
	/***************���ݰ�����  ͨ��Э���е�Typeλ********************/
	
	
	return 0;
}
