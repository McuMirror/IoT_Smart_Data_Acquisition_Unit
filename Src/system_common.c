#include "system_common.h"
#include "EEPROM_24C256.h"
#include "gpio.h"

#define RESEND_COUNT_LIMITS    10
#define RESET_COUNT_LIMITS     10

/**
  * @brief  STM32L4оƬ��ƷΨһ��ݱ�ʶ 96λ
  */
uint8_t unique_ID_STM32L4[12]; 

/**
  * @brief  �����������洢�Ĵ�����̬�����ڴ��ַ���ָ��
  */
Linked_List *memory_data_sensor_point_start_address;
	
	
///**
//  * @brief  �ڴ���Ԥ�õ�ͨ����Կ
//  */
//uint8_t init_communication_key[16] = {0xE6, 0XD8, 0x25, 0x6A, 0x02, 0xE8, 0x46, 0x36,
//																			0x9F, 0x21, 0x93, 0xA1, 0x30, 0x2D, 0xBE, 0x73}; 	

///**	
//  * @brief  �ڴ���Ԥ�õĻָ�����������Կ
//  */
//uint8_t init_wipe_key[16] = {0x52, 0XDE, 0x86, 0x6C, 0x7D, 0x15, 0xE8, 0x33,
//														 0xF7, 0xF0, 0xA8, 0xA3, 0x19, 0xC5, 0xE1, 0x55}; 	


/**
  * @brief  ϵͳ�����ӵ�ͨ��ģ�鶨�壬�ýṹ���е�ֵ��ͨ��ģ��ѡ����صĲ��뿪�ؾ���
  */
com_mod_phy_connected communication_module_physical_connected = {
	.lora_sx1278_connected                  = 0,                 //lora sx1278ģ����������AP��
	.wifi_esp8266_connected                 = 0,                 //wifi esp8266ģ����������AP��
	.bluetooth_nrf52832_connected           = 0,                 //bluetooth nrf52832ģ����������AP��
	.nb_iot_bc26_connected                  = 0                  //NB-IoT BC26ģ����������AP��
};

/**
  * @brief  ϵͳ�����ӵ�ͨ��ģ�鶨�壬�ýṹ���е�ֵ��ͨ��ģ��ѡ����صĲ��뿪�ؾ���
  */
sen_mod_phy_connected sensor_module_physical_connected = {
	.electric_meter_connected                  = 0,              //���ģ����������AP��
	.water_meter_connected                     = 0,              //ˮ��ģ����������AP��
	.gas_meter_connected                       = 0               //����ģ����������AP��
};

/**
  * @brief  ϵͳ����Ĺ���״̬��־λ
  */
sys_task_flag system_task_flag = {
	.task_sensor_data_collect    =   0,                 //���ݲɼ�����
	.task_sensor_data_update     =   0,                 //�����ϴ�����
	.task_control_command_get    =   0,                 //���������������
	.task_sleep                  =   0                  //��������ֹͣģʽ2��	
};


/**
  * @name         init_instruction_resend_procedure
  * @brief        ��ʼ��ָ���ط���λ����
  * @param        resend_count:���ط�����
  * @retval       void
	* @lastModify   2018/9/25  19:57
	* @author       JackWilliam
  */
void init_instruction_resend_procedure(uint8_t resend_count){
	if(resend_count <= RESEND_COUNT_LIMITS) return;
	else{
		if(ee_ReadBytes(&register_data_warning_store.InitResetCount, register_address_warning_store.InitResetCount, 1))
			error_code_handle(ERROR_CODE_EEPROM_Read_Bytes_FAIL);
		
		if(register_data_warning_store.InitResetCount <= RESET_COUNT_LIMITS){
			register_data_warning_store.InitResetCount++;
			register_data_warning_store.InitAlarm = 1;
			if(ee_WriteBytes(&register_data_warning_store.InitResetCount, register_address_warning_store.InitResetCount, 1))
				error_code_handle(ERROR_CODE_EEPROM_Write_Bytes_FAIL);
			if(ee_WriteBytes(&register_data_warning_store.InitAlarm, register_address_warning_store.InitAlarm, 1))
				error_code_handle(ERROR_CODE_EEPROM_Write_Bytes_FAIL);
			NVIC_SystemReset();
		}
		
		else{
			HAL_GPIO_WritePin(GPIOA, state_initialization_failed_LED_Pin, GPIO_PIN_SET);     //������ʼ��ʧ��ָʾ�ƣ���ָʾ��Ϊ����ָʾ��
			while(1);        //�������򣬵�����ʼ��ʧ��ָʾ�Ƶȴ�������Ա����ά��
		}
	}
}




/**
  * @name         clear_alarm_and_count
  * @brief        ��ո澯�Ĵ����еĸ澯λ������صļ�����
  * @param        count_reg_exist: ��λ�Ƿ����������صļ�������������˲���Ϊ1����֮Ϊ0
  * @param        *alarm_flag:     ָ��Ĵ����澯λ�������ڴ��б����ַ��ָ��
  * @param        alarm_flag_addr: �Ĵ����澯λ��EEPROM�еı����ַ
  * @param        alarm_flag_size: �Ĵ����澯λ�ĳ���
  * @param        *alarm_count:     ָ��Ĵ����澯λ��صļ������������ڴ��б����ַ��ָ��
  * @param        alarm_count_addr: �Ĵ����澯λ��صļ�������EEPROM�еı����ַ
  * @param        alarm_count_size: �Ĵ����澯λ��صļ������ĳ���
  * @retval       ��ճɹ��򷵻�0   ʧ���򷵻�1
	* @lastModify   2018/9/25  19:57
	* @author       JackWilliam
  */
uint8_t clear_alarm_and_count(uint8_t count_reg_exist, 
	  uint8_t * alarm_flag, uint8_t alarm_flag_addr, uint8_t alarm_flag_size, 
		uint8_t * alarm_count, uint8_t alarm_count_addr, uint8_t alarm_count_size)
{
	if(ee_ReadBytes(alarm_flag, alarm_flag_addr, alarm_flag_size))
			error_code_handle(ERROR_CODE_EEPROM_Read_Bytes_FAIL);
	
	if(*alarm_flag == 1){
		*alarm_flag = 0;
		if(ee_WriteBytes(alarm_flag, alarm_flag_addr, alarm_flag_size))
			error_code_handle(ERROR_CODE_EEPROM_Write_Bytes_FAIL);
		
		if(count_reg_exist == 1){
			*alarm_count = 0;
			if(ee_WriteBytes(alarm_count, alarm_count_addr, alarm_flag_size))
					error_code_handle(ERROR_CODE_EEPROM_Write_Bytes_FAIL);
		}
	}
	return 0;
}

///**
//  * @name         clear_init_alarm_and_count
//  * @brief        ��ո澯�Ĵ����еĸ澯λ������صļ�����
//  * @param        resend_count:���ط�����
//  * @retval       void
//	* @lastModify   2018/9/25  19:57
//	* @author       JackWilliam
//  */
//void clear_init_alarm_and_count(void){
//	ee_ReadBytes(&register_data_warning_store.InitAlarm, register_address_warning_store.InitAlarm, 1);
//	if(register_data_warning_store.InitAlarm == 1){
//		register_data_warning_store.InitAlarm = 0;
//		register_data_warning_store.InitResetCount = 0;
//		ee_WriteBytes(&register_data_warning_store.InitResetCount, register_address_warning_store.InitResetCount, 1);
//		ee_WriteBytes(&register_data_warning_store.InitAlarm, register_address_warning_store.InitAlarm, 1);
//	}
//}


/**
  * @name         error_code_handle
  * @brief        ������봦�����
  * @param        void
  * @retval       �������Error Code
	* @lastModify   2018/9/25  14:14
	* @author       JackWilliam
  */
uint8_t error_code_handle(uint8_t error_code){
	switch(error_code){
		//��·��������״ָ̬ʾ��LED��ʼ��ʧ��
		case ERROR_CODE_GPIO_LED_Init_FAIL:
			printf(" Error Code: %d \r\n GPIO LED init fail\r\n",  ERROR_CODE_GPIO_LED_Init_FAIL);      
			break;
		
		//���Դ���1��ʼ��ʧ��
		case ERROR_CODE_DEBUG_USART1_Init_FAIL:
			printf(" Error Code: %d \r\n DEBUG USART1 init fail\r\n",  ERROR_CODE_DEBUG_USART1_Init_FAIL);    
			break; 
		
		//���ڶ������Ź�ι����TIM3��ʼ��ʧ��
		case ERROR_CODE_IWDG_TIM3_Init_FAIL:
			printf(" Error Code: %d \r\n IWDG TIM3 init fail\r\n", ERROR_CODE_IWDG_TIM3_Init_FAIL);      
			break;
		
		//�������Ź���ʼ��ʧ��
		case ERROR_CODE_IWDG_Init_FAIL:
			printf(" Error Code: %d \r\n IWDG init\r\n", ERROR_CODE_IWDG_Init_FAIL);
			break;
		
		//EEPROM 24C256��ʼ��ʧ��
		case ERROR_CODE_EEPROM_24C256_Init_FAIL:
			printf(" Error Code: %d \r\n EEPROM 24C256 init\r\n", ERROR_CODE_EEPROM_24C256_Init_FAIL);
			break;
		
		//���ڲ��뿪�����GPIO�ܽų�ʼ��
		case ERROR_CODE_GPIO_DIP_Switch_Init_FAIL:
			printf(" Error Code: %d \r\n GPIO DIP Switch init\r\n", ERROR_CODE_GPIO_DIP_Switch_Init_FAIL);
			break;
		
		//���ڶ�ȡͨ��ģ��ѡ����ص�GPIO��ʧ��
		case ERROR_CODE_GPIO_DIP_Switch_Com_Mod_Read_FAIL:
			printf(" Error Code: %d \r\n GPIO DIP Switch com mod read fail\r\n", ERROR_CODE_GPIO_DIP_Switch_Init_FAIL);
			break;
		
		//LoRa SX1278ģ���SPIͨ�ű�Ҫ�ܽ������������GPIO�ܽų�ʼ��ʧ��
		case ERROR_CODE_GPIO_LoRa_SX1278_Extra_Init_FAIL:
			printf(" Error Code: %d \r\n GPIO LoRa SX1278 Extra init fail\r\n", ERROR_CODE_GPIO_LoRa_SX1278_Extra_Init_FAIL);
			break;
		
		//LoRa SX1278ģ��SPI��ʼ��
		case ERROR_CODE_LoRa_SX1278_SPI_Init_FAIL:
			printf(" Error Code: %d \r\n LoRa SX1278 SPI init fail\r\n", ERROR_CODE_LoRa_SX1278_SPI_Init_FAIL);
			break;
		
		//LoRa SX1278ģ��Ƶ�ʼ�������ʼ��  
		case ERROR_CODE_LoRa_SX1278_Radio_Driver_Init_FAIL:
			printf(" Error Code: %d \r\n LoRa SX1278 Radio Driver init fail\r\n", ERROR_CODE_LoRa_SX1278_Radio_Driver_Init_FAIL);
			break;
		
		//LoRa SX1278ģ��Ƶ�ʼ�������ʼ��  
		case ERROR_CODE_GPIO_WiFi_ESP8266_Extra_Init_FAIL:
			printf(" Error Code: %d \r\n GPIO WiFi ESP8266 Extra init fail\r\n", ERROR_CODE_GPIO_WiFi_ESP8266_Extra_Init_FAIL);
			break;

		//WiFi ESP8266ģ��USART3�����ʼ��ʧ��
		case ERROR_CODE_WiFi_ESP8266_USART3_Init_FAIL:
			printf(" Error Code: %d \r\n WiFi ESP8266 Usart3 init fail\r\n", ERROR_CODE_WiFi_ESP8266_USART3_Init_FAIL);
			break;
		
		//WiFi ESP8266ģ��������ʼ��ʧ��
		case ERROR_CODE_WiFi_ESP8266_Driver_Init_FAIL:
			printf(" Error Code: %d \r\n GPIO WiFi ESP8266 Driver init fail\r\n", ERROR_CODE_WiFi_ESP8266_Driver_Init_FAIL);
			break;
		
		//BlueTooth nRF52832ģ��USART3�����ʼ��ʧ��
		case ERROR_CODE_BlueTooth_nRF52832_USART3_Init_FAIL:
			printf(" Error Code: %d \r\n BlueTooth nRF52832 USART3 init fail\r\n", ERROR_CODE_BlueTooth_nRF52832_USART3_Init_FAIL);
			break;
		
	  //BlueTooth nRF52832ģ��������ʼ��ʧ��
		case ERROR_CODE_BlueTooth_nRF52832_Driver_Init_FAIL:
			printf(" Error Code: %d \r\n BlueTooth nRF52832 Driver init fail\r\n", ERROR_CODE_BlueTooth_nRF52832_Driver_Init_FAIL);
			break;
		
		//NB-IoT BC26ģ��USART3�����ʼ��ʧ��
		case ERROR_CODE_NB_IoT_BC26_USART3_Init_FAIL:
			printf(" Error Code: %d \r\n NB-IoT BC26 USART3 init fail\r\n", ERROR_CODE_NB_IoT_BC26_USART3_Init_FAIL);
			break;
		
		//NB-IoT BC26ģ��USART3�����ʼ��ʧ��
		case ERROR_CODE_NB_IoT_BC26_Driver_Init_FAIL:
			printf(" Error Code: %d \r\n NB-IoT BC26 Driver init fail\r\n", ERROR_CODE_NB_IoT_BC26_Driver_Init_FAIL);
			break;
		
		
		
		
		//��ȡ������ģ��ѡ����ص�GPIO��ʧ��
		case ERROR_CODE_GPIO_DIP_Switch_Sen_Mod_Read_FAIL:
			printf(" Error Code: %d \r\n GPIO DIP Switch Sen Mod Read init fail\r\n", ERROR_CODE_GPIO_DIP_Switch_Sen_Mod_Read_FAIL);
			break;
		
		
		//�����ص�GPIO�ڳ�ʼ��ʧ��
		case ERROR_CODE_GPIO_Electric_Meter_Extra_Init_FAIL:
			printf(" Error Code: %d \r\n GPIO electric meter extra init fail\r\n", ERROR_CODE_GPIO_Electric_Meter_Extra_Init_FAIL);
			break;
		
		//SP3485ģ����ʹ�õ�USART2�ǳ�ʼ��ʧ��
		case ERROR_CODE_Electric_Meter_USART2_Init_FAIL:
			printf(" Error Code: %d \r\n electric meter USART2 init fail\r\n", ERROR_CODE_Electric_Meter_USART2_Init_FAIL);
			break;
		
		//��������Զ������ʻ�ȡʧ��
		case ERROR_CODE_Electric_Meter_Baudrate_Auto_Get_FAIL:
			printf(" Error Code: %d \r\n electric meter baudrate auto get fail\r\n", ERROR_CODE_Electric_Meter_Baudrate_Auto_Get_FAIL);
			break;
		
		
		
		
		//����ϵͳ��ʼ������ʧ��
		case ERROR_CODE_Other_System_Initialization_Process_FAIL:
			printf(" Error Code: %d \r\n other system initialization process fail\r\n", ERROR_CODE_Other_System_Initialization_Process_FAIL);
			break;
		
		
		
		
		//��ȡSTM32�ڲ���96λ��ƷΨһ��ݱ�ʶʧ��
		case ERROR_CODE_Get_STM32L4_Chip_ID_FAIL:
			printf(" Error Code: %d \r\n get STM32L4 chip ID fail\r\n", ERROR_CODE_Get_STM32L4_Chip_ID_FAIL);
			break;
		
		//��ȡAP���Ƿ����ɹ���·�������������ӵ�EEPROM��־λʧ��
		case ERROR_CODE_Read_Connection_History_Byte_From_EEPROM_FAIL:
			printf(" Error Code: %d \r\n read connection history byte from EEPROM fail\r\n", ERROR_CODE_Read_Connection_History_Byte_From_EEPROM_FAIL);
			break;
		
		//���״�ע��ʱ��EEPROM����ʷע����Ϣʧ��
		case ERROR_CODE_EEPROM_24C256_REGISTRATION_RECOVRY_READ_PROCEDURE_FAIL:
			printf(" Error Code: %d \r\n EEPROM 24c256 registration recovry read procedure fail\r\n", ERROR_CODE_EEPROM_24C256_REGISTRATION_RECOVRY_READ_PROCEDURE_FAIL);
			break;	 
		
		//RTCʱ�ӳ�ʼ��ʧ��
		case ERROR_CODE_RTC_Init_Set_FAIL:
			printf(" Error Code: %d \r\n RTC init set fail\r\n", ERROR_CODE_RTC_Init_Set_FAIL);
			break;

		//���ʱ��ʹ���ʱ�����ʧ��
		case ERROR_CODE_Data_UpdateTime_Settings_Set_FAIL:
			printf(" Error Code: %d \r\n data updateTime settings set fail\r\n", ERROR_CODE_Data_UpdateTime_Settings_Set_FAIL);
			break;			
		
		
		
		
		
		//�յ������ݰ�����С����СЭ�������
		case ERROR_CODE_Protocol_Main_Package_Decode_Header_Length_Check_FAIL:
			printf(" Error Code: %d \r\n protocol main package decode header length check fail\r\n", ERROR_CODE_Protocol_Main_Package_Decode_Header_Length_Check_FAIL);
			break;	 
		
		//�յ������ݰ���֡ͷ����ȷ
		case ERROR_CODE_Protocol_Main_Package_Decode_Header_Top_Check_FAIL:
			printf(" Error Code: %d \r\n protocol main package decode header top check fail\r\n", ERROR_CODE_Protocol_Main_Package_Decode_Header_Top_Check_FAIL);
			break;	

		//�յ������ݰ��İ汾�Ų���ȷ
		case ERROR_CODE_Protocol_Main_Package_Decode_Header_Version_Check_FAIL:
			printf(" Error Code: %d \r\n protocol main package decode header version check fail\r\n", ERROR_CODE_Protocol_Main_Package_Decode_Header_Version_Check_FAIL);
			break;	 		
		
		//�յ������ݰ�����Ų���ȷ
		case ERROR_CODE_Protocol_Main_Package_Decode_Header_Serial_ID_Check_FAIL:
			printf(" Error Code: %d \r\n protocol main package decode header serial ID check fail\r\n", ERROR_CODE_Protocol_Main_Package_Decode_Header_Serial_ID_Check_FAIL);
			break;
		
		//��ע��״̬���յ������ݰ�Դ��ַ����
		case ERROR_CODE_Protocol_Main_Package_Decode_Header_Master_ID_Check_FAIL:
			printf(" Error Code: %d \r\n protocol main package decode header master ID check fail\r\n", ERROR_CODE_Protocol_Main_Package_Decode_Header_Master_ID_Check_FAIL);
			break;
		
		//��ע��״̬���յ������ݰ�Ŀ�ĵ�ַ����	
		case ERROR_CODE_Protocol_Main_Package_Decode_Header_Slave_ID_Check_FAIL:
			printf(" Error Code: %d \r\n protocol main package decode header slave ID check fail\r\n", ERROR_CODE_Protocol_Main_Package_Decode_Header_Slave_ID_Check_FAIL);
			break;
		
		//·����ע����Ӧ����ʧ��	
		case ERROR_CODE_Protocol_Device_Registere_Responce_Part_Decode_FAIL:
			printf(" Error Code: %d \r\n protocol device registere responce part decode fail\r\n", ERROR_CODE_Protocol_Device_Registere_Responce_Part_Decode_FAIL);
			break;
		
		//·��������������ȷ��/����ʧ��	
		case ERROR_CODE_Protocol_Sensor_Parameter_Responce_Part_Decode_FAIL:
			printf(" Error Code: %d \r\n protocol sensor parameter responce part decode fail\r\n", ERROR_CODE_Protocol_Sensor_Parameter_Responce_Part_Decode_FAIL);
			break;
		
		//·����ʱ��ͬ��ȷ��ʧ��
		case ERROR_CODE_Protocol_Time_Sync_Responce_Part_Decode_FAIL:
			printf(" Error Code: %d \r\n protocol time sync responce part decode fail\r\n", ERROR_CODE_Protocol_Time_Sync_Responce_Part_Decode_FAIL);
			break;
		
		//·���������ϴ�ȷ��ʧ��
		case ERROR_CODE_Protocol_Data_Entity_Ack_Decode_FAIL:
			printf(" Error Code: %d \r\n protocol data entity ack decode fail\r\n", ERROR_CODE_Protocol_Data_Entity_Ack_Decode_FAIL);
			break;

		//·�����澯��Ϣȷ��ʧ��
		case ERROR_CODE_Protocol_System_Warning_Ack_Part_Decode_FAIL:
			printf(" Error Code: %d \r\n protocol system warning ack decode fail\r\n", ERROR_CODE_Protocol_System_Warning_Ack_Part_Decode_FAIL);
			break;
		
		//·�������ڸ�������ʧ��
		case ERROR_CODE_Protocol_Period_Change_Request_Part_Decode_FAIL:
			printf(" Error Code: %d \r\n protocol period change request part decode fail\r\n", ERROR_CODE_Protocol_Period_Change_Request_Part_Decode_FAIL);
			break;
		
		//·���������������������ʧ��
		case ERROR_CODE_Protocol_Sensor_Parameter_Change_Request_Part_Decode_FAIL:
			printf(" Error Code: %d \r\n protocol sensor parameter change request part decode fail\r\n", ERROR_CODE_Protocol_Sensor_Parameter_Change_Request_Part_Decode_FAIL);
			break;
		
		//·��������ͨ����Կ����ʧ��
		case ERROR_CODE_Protocol_Communication_Key_Update_Request_Part_Decode_FAIL:
			printf(" Error Code: %d \r\n protocol communication key update request part decode fail\r\n", ERROR_CODE_Protocol_Communication_Key_Update_Request_Part_Decode_FAIL);
			break;
	
		//·����������������ʧ��
		case ERROR_CODE_Protocol_Network_Switching_Request_Part_Decode_FAIL:
			printf(" Error Code: %d \r\n protocol network switching request part decode fail\r\n", ERROR_CODE_Protocol_Network_Switching_Request_Part_Decode_FAIL);
			break;

		//·������������ʧ��
		case ERROR_CODE_Protocol_Reboot_Request_Part_Decode_FAIL:
			printf(" Error Code: %d \r\n protocol reboot request part decode fail\r\n", ERROR_CODE_Protocol_Reboot_Request_Part_Decode_FAIL);
			break;

		//·������ȡAP�豸״̬ʧ��
		case ERROR_CODE_Protocol_Device_State_Request_Part_Decode_FAIL:
			printf(" Error Code: %d \r\n protocol device state request part decode fail\r\n", ERROR_CODE_Protocol_Device_State_Request_Part_Decode_FAIL);
			break;
		
	//·��������ģʽ��������ʧ��
		case ERROR_CODE_Protocol_Energy_Saving_Mode_Request_Part_Decode_FAIL:
			printf(" Error Code: %d \r\n protocol energy saving mode request part decode fail\r\n", ERROR_CODE_Protocol_Energy_Saving_Mode_Request_Part_Decode_FAIL);
			break;
		
	//·�����ָ�������������ʧ��
		case ERROR_CODE_Protocol_Factory_Setting_Reset_Request_Part_Decode_FAIL:
			printf(" Error Code: %d \r\n protocol factory setting reset request part decode fail\r\n", ERROR_CODE_Protocol_Factory_Setting_Reset_Request_Part_Decode_FAIL);
			break;
		

		
		
		
		
		
		
		
		
		//����1�������
		case ERROR_CODE_UART1_RX_BUFFER_OVERFLOW:
			printf(" Error Code: %d \r\n Uart1 Receive Buffer OverFlow\r\n", ERROR_CODE_UART1_RX_BUFFER_OVERFLOW); 
			break;
		
		//����2�������
		case ERROR_CODE_UART2_RX_BUFFER_OVERFLOW:
			printf(" Error Code: %d \r\n Uart2 Receive Buffer OverFlow\r\n", ERROR_CODE_UART2_RX_BUFFER_OVERFLOW);
			break;
		
		//����3�������
		case ERROR_CODE_UART3_RX_BUFFER_OVERFLOW:
			printf(" Error Code: %d \r\n Uart3 Receive Buffer OverFlow\r\n", ERROR_CODE_UART3_RX_BUFFER_OVERFLOW);  
			break;
		
		//EEPROM��ȡ��������صļĴ���ʧ��
		case ERROR_CODE_EEPROM_24C256_SENSOR_PARAMETER_READ_FAIL:
			printf(" Error Code: %d \r\n EEPROM 24c256 sensor parameter read fail\r\n", ERROR_CODE_EEPROM_24C256_SENSOR_PARAMETER_READ_FAIL);  
			break; 
		
		//EEPROM��ȡ�ֽ�����ʧ��
		case ERROR_CODE_EEPROM_Read_Bytes_FAIL:
			printf(" Error Code: %d \r\n EEPROM Read Bytes fail\r\n", ERROR_CODE_UART3_RX_BUFFER_OVERFLOW);  
			break;
		
		//EEPROMд���ֽ�����ʧ��                      
		case ERROR_CODE_EEPROM_Write_Bytes_FAIL:  
			printf(" Error Code: %d \r\n EEPROM Write Bytes fail\r\n", ERROR_CODE_UART3_RX_BUFFER_OVERFLOW);  
			break;
	}
	/*********************������        ��ʽ����������ɾ��**************/
	//������ʼ��ʧ�ܵ�
	HAL_GPIO_WritePin(GPIOA, state_initialization_failed_LED_Pin, GPIO_PIN_SET);
	while(1);
	/*********************������        ��ʽ����������ɾ��**************/
	
	return error_code;
}
