#include "system_init.h"
#include "iwdg.h"
#include "rng.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "malloc.h"


#include "EEPROM_24C256.h"
#include "system_common.h"
#include "LoRa_SX1278_main_communication.h"
#include "BlueTooth_nRF52832_main_communication.h"
#include "NB_IoT_BC26_main_communication.h"
#include "LoRa_SX1278_main_communication.h"
#include "WiFi_ESP8266_main_communication.h"

#include "sensor_electric_meter.h"

extern reg_data_sensor_parm_and_ID register_data_sensor_parm_and_ID;
extern Linked_List *memory_data_sensor_point_start_address;

//reg_data_sensor_parm_and_ID *register_data_sensor_parm_and_ID_address;

/********************************************************************************************************/
/*                                                                                                      */
/*                                 AP��Ӳ����ʼ��                                                       */
/*                                   ����ο�ʼ                                                         */
/*                                                                                                      */
/*                                                                                                      */
/********************************************************************************************************/

/**
  * @name         AP_side_hardware_init
  * @brief        AP�����Ӳ����ʼ�� ����Ƭ�����衢ͨ��ģ�鼰������ģ��
  * @param        void
  * @retval       void
	* @lastModify   2018/9/26  10:25
	* @author       JackWilliam
  */
void AP_side_hardware_init(void){
	necessary_on_chip_peripherals_init();              //��һ�׶�  ��ʼ����ҪƬ������
	communication_module_init();                       //�ڶ��׶�  ��ʼ��ͨ��ģ��
	sensor_module_init();                              //�����׶�  ��ʼ��������ģ��
  other_initialization_procedure();	                 //���Ľ׶�  ������ʼ������
	printf("System Module Init Success!\r\n");         //���
}


/**
  * @name         necessary_on_chip_peripherals_init
  * @brief        ��һ�׶�  ��ʼ����ҪƬ������
  * @param        void
  * @retval       void
	* @lastModify   2018/9/25  14:14
	* @author       JackWilliam
  */
void necessary_on_chip_peripherals_init(void){
	if(DEBUG_USART1_Init())                    //���Դ���USART1��ʼ��
		error_code_handle(ERROR_CODE_DEBUG_USART1_Init_FAIL);
	
	if(GPIO_LED_Init())                        //LED���GPIO�ܽų�ʼ��
		error_code_handle(ERROR_CODE_GPIO_LED_Init_FAIL);
	
	if(IWDG_TIM3_Init())                        //���ڶ������Ź�ι����1S��ʱ��TIM3��ʼ��
		error_code_handle(ERROR_CODE_IWDG_TIM3_Init_FAIL);
	
	if(IWDG_Init())                             //�������Ź���ʼ��
		error_code_handle(ERROR_CODE_IWDG_Init_FAIL);
	
	if(EEPROM_24C256_Init())                        //����24C256 EEPROM ��ʼ��
		error_code_handle(ERROR_CODE_EEPROM_24C256_Init_FAIL);
	
	if(GPIO_DIP_Switch_Init())                     //���ڲ��뿪�����GPIO�ܽų�ʼ��
		error_code_handle(ERROR_CODE_GPIO_DIP_Switch_Init_FAIL);
}

/**
  * @name         communication_module_init
  * @brief        �ڶ��׶�  ��ʼ��ͨ��ģ��
  * @param        void
  * @retval       void
	* @lastModify   2018/9/25  14:14
	* @author       JackWilliam
  */
void communication_module_init(void){
	if(GPIO_DIP_Switch_Com_Mod_Read(&communication_module_physical_connected))  //��ȡͨ��ģ��ѡ����ص�GPIO��
		error_code_handle(ERROR_CODE_GPIO_DIP_Switch_Com_Mod_Read_FAIL);
	
	if(communication_module_physical_connected.lora_sx1278_connected == 1){
		
	/***************LoRa SX1278������ʼ��       START*******************/
		if(GPIO_LoRa_SX1278_Extra_Init())  //LoRa SX1278ģ���SPIͨ�ű�Ҫ�ܽ������������GPIO�ܽų�ʼ��
			error_code_handle(ERROR_CODE_GPIO_LoRa_SX1278_Extra_Init_FAIL);
		
		if(LoRa_SX1278_SPI_Init())  //LoRa SX1278ģ��SPI��ʼ��
			error_code_handle(ERROR_CODE_LoRa_SX1278_SPI_Init_FAIL);
			
		if(LoRa_SX1278_Radio_Driver_Init())  //LoRa SX1278ģ��Ƶ�ʼ�������ʼ��
			error_code_handle(ERROR_CODE_LoRa_SX1278_Radio_Driver_Init_FAIL);
		
		register_data_basic_settings.ComModuleType = ComModuleType_LoRa_SX1278_Connected;
	}
	/***************LoRa SX1278������ʼ��       END*******************/
	
	
	/***************WiFi ESP8266������ʼ��       START*******************/
	if(communication_module_physical_connected.wifi_esp8266_connected == 1){
		if(GPIO_WiFi_ESP8266_Extra_Init())  //WiFi ESP8266ģ���USART3ͨ�ű�Ҫ�ܽ������������GPIO�ܽų�ʼ��
			error_code_handle(ERROR_CODE_GPIO_WiFi_ESP8266_Extra_Init_FAIL);
		
		if(WiFi_ESP8266_USART3_Init())  //WiFi ESP8266ģ��USART3��ʼ��
			error_code_handle(ERROR_CODE_WiFi_ESP8266_USART3_Init_FAIL);
			
		if(WiFi_ESP8266_Driver_Init())  //WiFi ESP8266ģ��������ʼ��
			error_code_handle(ERROR_CODE_WiFi_ESP8266_Driver_Init_FAIL);
		
		register_data_basic_settings.ComModuleType = ComModuleType_WiFi_ESP8266_Connected;
	}
	/***************WiFi ESP8266������ʼ��       END*******************/
	
	
	/***************BlueTooth nRF52832������ʼ��       START*******************/
	if(communication_module_physical_connected.bluetooth_nrf52832_connected == 1){	
		if(BlueTooth_nRF52832_USART3_Init())  //WiFi ESP8266ģ��USART3��ʼ��
			error_code_handle(ERROR_CODE_BlueTooth_nRF52832_USART3_Init_FAIL);
			
		if(BlueTooth_nRF52832_Driver_Init())  //WiFi ESP8266ģ��������ʼ��
			error_code_handle(ERROR_CODE_BlueTooth_nRF52832_Driver_Init_FAIL);
		
		register_data_basic_settings.ComModuleType = ComModuleType_BlueTooth_nRF52832_Connected;
	}
	/***************BlueTooth nRF52832������ʼ��      END*******************/
	
	
	
	/***************NB-IoT BC26������ʼ��       START   !!!!!!!��δ��������*******************/
	if(communication_module_physical_connected.bluetooth_nrf52832_connected == 1){	
		if(NB_IoT_BC26_USART3_Init())  //NB-IoT BC26ģ��USART3��ʼ��
			error_code_handle(ERROR_CODE_NB_IoT_BC26_USART3_Init_FAIL);
			
		//!!!!!!!��δ��������
		if(NB_IoT_BC26_Driver_Init())  //NB-IoT BC26ģ��������ʼ��     !!!!!!!��δ��������
			error_code_handle(ERROR_CODE_NB_IoT_BC26_Driver_Init_FAIL);
		
		register_data_basic_settings.ComModuleType = ComModuleType_NB_IoT_BC26_Connected;
	} 
	/***************NB-IoT BC26������ʼ��       END    !!!!!!!��δ��������*******************/	
	
	if(ee_WriteBytes(&register_data_basic_settings.ComModuleType, register_address_basic_settings.ComModuleType, 1))
		error_code_handle(ERROR_CODE_EEPROM_Write_Bytes_FAIL);
}

/**
  * @name         sensor_module_init
  * @brief        �����׶�  ��ʼ��������ģ��
  * @param        void
  * @retval       void
	* @lastModify   2018/9/25  14:14
	* @author       JackWilliam
  */
void sensor_module_init(void){
	if(GPIO_DIP_Switch_Sen_Mod_Read(&sensor_module_physical_connected))  //��ȡͨ��ģ��ѡ����ص�GPIO��
		error_code_handle(ERROR_CODE_GPIO_DIP_Switch_Sen_Mod_Read_FAIL);
	
		/**************���ģ���ʼ��       START   DLT-645-2007-�๦�ܵ��ܱ�ͨ��Э��2010*******************/
	if(sensor_module_physical_connected.electric_meter_connected == 1){
		if(GPIO_Electric_Meter_Extra_Init())  //WiFi ESP8266ģ���USART3ͨ�ű�Ҫ�ܽ������������GPIO�ܽų�ʼ��
			error_code_handle(ERROR_CODE_GPIO_Electric_Meter_Extra_Init_FAIL);
		
		if(Electric_Meter_USART2_Init())  //���ģ��USART2��ʼ��
			error_code_handle(ERROR_CODE_Electric_Meter_USART2_Init_FAIL);
		
		if(Electric_Meter_Baudrate_Auto_Get(&sensor_electric_meter_general_parameters))  //���ģ�鲨�����Զ�ʶ�𣡣���������δ�������ֽ׶ι̶�����2400�Ĳ�����
			error_code_handle(ERROR_CODE_Electric_Meter_Baudrate_Auto_Get_FAIL);
	}
	 /**************���ģ���ʼ��       START   DLT-645-2007-�๦�ܵ��ܱ�ͨ��Э��2010*******************/
}

/**
  * @name         other_initialization_procedure
  * @brief        ���Ľ׶�  ������ʼ������
  * @param        void
  * @retval       void
	* @lastModify   2018/9/26  10:03
	* @author       JackWilliam
  */
void other_initialization_procedure(void){
/**	
  * @brief  �ڴ���Ԥ�õĻָ�����������Կ
  */
	uint8_t init_wipe_key[16] = {0x52, 0XDE, 0x86, 0x6C, 0x7D, 0x15, 0xE8, 0x33,
														 0xF7, 0xF0, 0xA8, 0xA3, 0x19, 0xC5, 0xE1, 0x55};
	uint8_t i;

	for(i = 0; i < 16; i++) register_data_wipe_key.WipeKey[i] = init_wipe_key[i];
														 

														 
	if(ee_WriteBytes(register_data_wipe_key.WipeKey, register_address_wipe_key.WipeKey, 16))
		error_code_handle(ERROR_CODE_EEPROM_Write_Bytes_FAIL);
	
	if(other_system_initialization_process())
		error_code_handle(ERROR_CODE_Other_System_Initialization_Process_FAIL);

}	


/**
  * @name         other_system_initialization_process
  * @brief        ����ϵͳ��ʼ������
  * @param        void
  * @retval       ������ʼ�������Ƿ�˳�����  �ɹ��򷵻�0  ʧ���򷵻�1
	* @lastModify   2018/9/26  10:20
	* @author       JackWilliam
  */
uint8_t other_system_initialization_process(void){
	  HAL_TIM_Base_Start_IT(&htim2);                                //��ʼ���ж�ʱ��
	__HAL_RCC_PWR_CLK_ENABLE();                                     //��ʼ�����Ŀ���ģ��ʱ��
	__HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_HSI);      //��֤ϵͳ���Ѻ�ʹ��HSIʱ��
	my_mem_init(SRAMIN);		                                        //��ʼ���ڲ��ڴ��  Ԥ��ռ��32K���ڴ�ռ����ڶ�̬�ڴ����
		
	memory_data_sensor_point_start_address = CreateNode();
	
	return 0;
}

/********************************************************************************************************/
/******************************AP��Ӳ����ʼ��   ����ν���***********************************************/
/********************************************************************************************************/


/********************************************************************************************************/
/*                                                                                                      */
/*                                 ·�������ӳ�ʼ��                                                     */
/*                                   ����ο�ʼ                                                         */
/*                                                                                                      */
/*                                                                                                      */
/********************************************************************************************************/

/**
  * @name         router_connection_initialization
  * @brief        AP����·�������ӳ�ʼ��
  * @param        void
  * @retval       void
	* @lastModify   2018/9/26  10:37
	* @author       JackWilliam
  */
void router_connection_initialization(void){
	if(get_STM32L4_chip_ID(unique_ID_STM32L4))
				error_code_handle(ERROR_CODE_Get_STM32L4_Chip_ID_FAIL);
	
	if(read_connection_history_byte_from_eeprom())
			error_code_handle(ERROR_CODE_Read_Connection_History_Byte_From_EEPROM_FAIL);
	
	if(register_data_basic_settings.DataExistFlag == 0x01) AP_side_registration_recovry();
	else AP_side_registration_for_first_time();
	
}

/**
  * @name         get_STM32L4_chip_ID
  * @brief        ��ȡSTM32�ڲ���96λ��ƷΨһ��ݱ�ʶ
	* @param        *STM32_ID_point:ָ�򱣴�STM32��ƷΨһʶ���������ָ��
  * @retval       ��ʶ��ȡ�Ƿ�ɹ�  �ɹ�����0  ʧ�ܷ���1
	* @lastModify   2018/9/26  10:37
	* @author       JackWilliam
  */
uint8_t get_STM32L4_chip_ID(uint8_t * STM32_ID_point){
  	uint32_t temp0,temp1,temp2;
	
//STM32L4ϵ�� ID���ַ�� 0x1FFF7590   0x1FFF7594  0x1FFF7598 ��ֻ��Ҫ��ȡ�����ַ�е����ݾͿ�����
		temp0 = *(__IO uint32_t*)(0x1FFF7590);    //��ƷΨһ��ݱ�ʶ�Ĵ�����96λ��
    temp1 = *(__IO uint32_t*)(0x1FFF7594);
		temp2 = *(__IO uint32_t*)(0x1FFF7598);                       

    STM32_ID_point[0] = (uint8_t)(temp0 & 0x000000FF);
    STM32_ID_point[1] = (uint8_t)((temp0 & 0x0000FF00)>>8);
    STM32_ID_point[2] = (uint8_t)((temp0 & 0x00FF0000)>>16);
    STM32_ID_point[3] = (uint8_t)((temp0 & 0xFF000000)>>24);
    STM32_ID_point[4] = (uint8_t)(temp1 & 0x000000FF);
    STM32_ID_point[5] = (uint8_t)((temp1 & 0x0000FF00)>>8);
    STM32_ID_point[6] = (uint8_t)((temp1 & 0x00FF0000)>>16);
    STM32_ID_point[7] = (uint8_t)((temp1 & 0xFF000000)>>24);
    STM32_ID_point[8] = (uint8_t)(temp2 & 0x000000FF);
    STM32_ID_point[9] = (uint8_t)((temp2 & 0x0000FF00)>>8);
    STM32_ID_point[10] = (uint8_t)((temp2 & 0x00FF0000)>>16);
    STM32_ID_point[11] = (uint8_t)((temp2 & 0xFF000000)>>24);   

		return 0;
}
/**
  * @name         read_connection_history_byte_from_eeprom
  * @brief        ��ȡAP���Ƿ����ɹ���·�������������ӵ�EEPROM��־λ
	* @param        void
  * @retval       ��־λ��ȡ�Ƿ�ɹ�  �ɹ�����0  ʧ�ܷ���1
	* @lastModify   2018/9/26  10:37
	* @author       JackWilliam
  */
uint8_t read_connection_history_byte_from_eeprom(void){
	if(ee_ReadBytes(&register_data_basic_settings.DataExistFlag, register_address_basic_settings.DataExistFlag, 1)) 
		return 1;
	
	return 0;
}

/**
  * @name         AP_side_registration_recovry
  * @brief        AP���豸���״�ע����̣���ע��ָ�����
	* @param        void
  * @retval       void
	* @lastModify   2018/9/26  10:37
	* @author       JackWilliam
  */
void AP_side_registration_recovry(void){
	if(EEPROM_24C256_registration_recovry_read_procedure())
		error_code_handle(ERROR_CODE_EEPROM_24C256_REGISTRATION_RECOVRY_READ_PROCEDURE_FAIL);
}

/**
  * @name         AP_side_registration_recovry
  * @brief        AP���豸�״�ע����� 
	* @param        void
  * @retval       void
	* @lastModify   2018/9/26  10:37
	* @author       JackWilliam
  */
void AP_side_registration_for_first_time(void){
/**
  * @brief  �ڴ���Ԥ�õ�ͨ����Կ
  */
uint8_t init_communication_key[16] = {0xE6, 0XD8, 0x25, 0x6A, 0x02, 0xE8, 0x46, 0x36,
																			0x9F, 0x21, 0x93, 0xA1, 0x30, 0x2D, 0xBE, 0x73}; 	

																			
}


/**
  * @name         EEPROM_24C256_registration_recovry_read_procedure
  * @brief        ���״�ע��ʱ��EEPROM����ʷע����Ϣ
	* @param        void
  * @retval       ��ʷע����Ϣ��ȡ�Ƿ�ɹ�  �ɹ�����0  ʧ�ܷ���1
	* @lastModify   2018/9/26  14:24
	* @author       JackWilliam
  */
uint8_t EEPROM_24C256_registration_recovry_read_procedure(void){
/*********��EEPROM�ж�ȡ�����������Ϣ���������ڴ�ṹ����************************/
	
	/***********************AP_REG_BASIC_SETTINGS******************************/	
	if(ee_ReadBytes(register_data_basic_settings.SlaveID, register_address_basic_settings.SlaveID, 2))
		error_code_handle(ERROR_CODE_EEPROM_Read_Bytes_FAIL);
	
	if(ee_ReadBytes(register_data_basic_settings.MasterID, register_address_basic_settings.MasterID, 2))
		error_code_handle(ERROR_CODE_EEPROM_Read_Bytes_FAIL);
	
	if(ee_ReadBytes(&register_data_basic_settings.SensorNum, register_address_basic_settings.SensorNum, 1))
		error_code_handle(ERROR_CODE_EEPROM_Read_Bytes_FAIL);
	
	if(ee_ReadBytes(&register_data_basic_settings.PowerSupply, register_address_basic_settings.PowerSupply, 1))
		error_code_handle(ERROR_CODE_EEPROM_Read_Bytes_FAIL);
	
	if(ee_ReadBytes(&register_data_basic_settings.SavePower, register_address_basic_settings.SavePower, 1))
		error_code_handle(ERROR_CODE_EEPROM_Read_Bytes_FAIL);
	/***********************AP_REG_BASIC_SETTINGS******************************/	
	
	
	/***********************AP_REG_COMMUNICATION_KEY******************************/	
	if(ee_ReadBytes(register_data_communication_key.CommunicationKey, register_address_communication_key.CommunicationKey, 16))
		error_code_handle(ERROR_CODE_EEPROM_Read_Bytes_FAIL);
	/***********************AP_REG_COMMUNICATION_KEY******************************/	
	
	/********************AP_REG_SENSOR_PARM  && AP_REG_SENSOR_ID****************************/	
	if(EEPROM_24C256_sensor_parameter_read(register_address_basic_settings.SensorNum, memory_data_sensor_point_start_address))
		error_code_handle(ERROR_CODE_EEPROM_24C256_SENSOR_PARAMETER_READ_FAIL);
	/********************AP_REG_SENSOR_PARM  && AP_REG_SENSOR_ID****************************/			
		
		
		
		
	return 0;
	
}

///**
//  * @name         EEPROM_24C256_sensor_parameter_read
//  * @brief        EEPROM��ȡ��������صļĴ���  ��ʹ�ö�̬�ڴ����  ��Ϊ������������ȷ��
//	* @param        void
//  * @retval       ��������صļĴ�����ȡ�Ƿ�ɹ�  �ɹ�����0  ʧ�ܷ���1
//	* @lastModify   2018/9/26  14:24
//	* @author       JackWilliam
//  */
//uint8_t EEPROM_24C256_sensor_parameter_read(uint8_t sensor_count){
//	uint16_t EEPROM_base_address = AP_REG_SENSOR_PARM;
//	uint16_t offset_address = 0;                                      //EEPROMƫ�Ƶ�ַ 
//	uint32_t memory_sensor_data_size = (uint32_t)sensor_count * 32;   //ÿ�鴫���������Ĵ�����ռ�ռ��СΪ32�ֽ�
//	
//	memory_data_sensor_point_start_address = mymalloc(0, memory_sensor_data_size);          //���붯̬�ڴ�
//	
//	while(sensor_count--){
//		if(ee_ReadBytes(&register_data_sensor_parm.SensorType, 
//			   offset_address + register_address_sensor_parm.SensorType , 1))
//			error_code_handle(ERROR_CODE_EEPROM_Read_Bytes_FAIL);
//		sprintf((char*)(memory_data_sensor_point_start_address + offset_address), "%c", register_data_sensor_parm.SensorType);
//		offset_address += AP_BYTE_OFFSET;
//		
//		if(ee_ReadBytes(&register_data_sensor_parm.SensorParaNum, 
//			   offset_address + register_address_sensor_parm.SensorParaNum , 1))
//			error_code_handle(ERROR_CODE_EEPROM_Read_Bytes_FAIL);
//		sprintf((char*)(memory_data_sensor_point_start_address + offset_address), "%c", register_data_sensor_parm.SensorParaNum);
//		offset_address += AP_BYTE_OFFSET;
//		
//		if(ee_ReadBytes(&register_data_sensor_parm.SensorIDLen, 
//			   offset_address + register_address_sensor_parm.SensorIDLen , 1))
//			error_code_handle(ERROR_CODE_EEPROM_Read_Bytes_FAIL);
//		sprintf((char*)(memory_data_sensor_point_start_address + offset_address), "%c", register_data_sensor_parm.SensorIDLen);
//		offset_address += AP_BYTE_OFFSET;
//		
//		if(ee_ReadBytes(register_data_sensor_parm.SensorComSpeed, 
//			   offset_address + register_address_sensor_parm.SensorComSpeed , 5))
//			error_code_handle(ERROR_CODE_EEPROM_Read_Bytes_FAIL);
//		sprintf((char*)(memory_data_sensor_point_start_address + offset_address), "%s", register_data_sensor_parm.SensorComSpeed);
//		offset_address += AP_BYTE_OFFSET * 5;
//		
//		if(ee_ReadBytes(register_data_sensor_parm.sensorParameter, 
//			   offset_address + register_address_sensor_parm.sensorParameter , 8))
//			error_code_handle(ERROR_CODE_EEPROM_Read_Bytes_FAIL);
//		sprintf((char*)(memory_data_sensor_point_start_address + offset_address), "%s", register_data_sensor_parm.sensorParameter);
//		offset_address += AP_BYTE_OFFSET * 8;
//		
//		if(ee_ReadBytes(register_data_sensor_ID.sensorID, 
//			   offset_address + register_address_sensor_ID.sensorID , 16))
//			error_code_handle(ERROR_CODE_EEPROM_Read_Bytes_FAIL);
//		sprintf((char*)(memory_data_sensor_point_start_address + offset_address), "%s", register_data_sensor_parm.sensorParameter);
//		offset_address += AP_BYTE_OFFSET * 16;
//	}
//	
//	return 0;

//}

/**
  * @name         EEPROM_24C256_sensor_parameter_read
  * @brief        EEPROM��ȡ��������صļĴ���  ��ʹ�ö�̬�ڴ����  ��Ϊ������������ȷ��  ʹ��������  ���ܻ����BUG��
	* @param        void
  * @retval       ��������صļĴ�����ȡ�Ƿ�ɹ�  �ɹ�����0  ʧ�ܷ���1
	* @lastModify   2018/9/26  14:24
	* @author       JackWilliam
  */
uint8_t EEPROM_24C256_sensor_parameter_read(uint8_t sensor_count, Linked_List *sensor_parameter_recovery_linked_list_header){
//	Linked_List *sensor_parameter_recovery_point;
	uint16_t offset_address = 0;                                      //EEPROMƫ�Ƶ�ַ 
	uint8_t i;
	
//	sensor_parameter_recovery_point = CreateNode();
//	sensor_parameter_recovery_linked_list_header = CreateNode();
	
	while(sensor_count--){
		sensor_parameter_recovery_linked_list_header->sensor_data_package_point = NULL;      //�������δ�õ������������ݶνṹ��ָ�����
		sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID = mymalloc(sizeof(sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID));  //Ϊ������õ������������ݶνṹ��ָ�����붯̬�ڴ�
		sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID->sensor_parm = mymalloc(sizeof(sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID->sensor_parm)); 
		sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID->sensor_ID = mymalloc(sizeof(sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID->sensor_ID)); 
		
		if(ee_ReadBytes(&register_data_sensor_parm.SensorType, 
			   offset_address + register_address_sensor_parm.SensorType , 1))
			error_code_handle(ERROR_CODE_EEPROM_Read_Bytes_FAIL);
		sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID->sensor_parm->SensorType = register_data_sensor_parm.SensorType;
		offset_address += AP_BYTE_OFFSET;
		
		if(ee_ReadBytes(&register_data_sensor_parm.SensorParaNum, 
			   offset_address + register_address_sensor_parm.SensorParaNum , 1))
			error_code_handle(ERROR_CODE_EEPROM_Read_Bytes_FAIL);
		sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID->sensor_parm->SensorParaNum = register_data_sensor_parm.SensorParaNum;
		offset_address += AP_BYTE_OFFSET;
		
		if(ee_ReadBytes(&register_data_sensor_parm.SensorIDLen, 
			   offset_address + register_address_sensor_parm.SensorIDLen , 1))
			error_code_handle(ERROR_CODE_EEPROM_Read_Bytes_FAIL);
		sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID->sensor_parm->SensorIDLen = register_data_sensor_parm.SensorIDLen;
		offset_address += AP_BYTE_OFFSET;
		
		if(ee_ReadBytes(register_data_sensor_parm.SensorComSpeed, 
			   offset_address + register_address_sensor_parm.SensorComSpeed , 5))
			error_code_handle(ERROR_CODE_EEPROM_Read_Bytes_FAIL);
		for(i = 0; i < 5; i++)
			sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID->sensor_parm->SensorComSpeed[i] = register_data_sensor_parm.SensorComSpeed[i];
		offset_address += AP_BYTE_OFFSET * 5;
		
		if(ee_ReadBytes(register_data_sensor_parm.sensorParameter, 
			   offset_address + register_address_sensor_parm.sensorParameter , 8))
			error_code_handle(ERROR_CODE_EEPROM_Read_Bytes_FAIL);
		for(i = 0; i < 8; i++)
			sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID->sensor_parm->sensorParameter[i] = register_data_sensor_parm.sensorParameter[i];
		offset_address += AP_BYTE_OFFSET * 8;
		
		if(ee_ReadBytes(register_data_sensor_ID.sensorID, 
			   offset_address + register_address_sensor_ID.sensorID , 16))
			error_code_handle(ERROR_CODE_EEPROM_Read_Bytes_FAIL);
		for(i = 0; i < 16; i++)
			sensor_parameter_recovery_linked_list_header->sensor_parm_and_ID->sensor_ID->sensorID[i] = register_data_sensor_ID.sensorID[i];
		offset_address += AP_BYTE_OFFSET * 16;	
		
		if(sensor_count != 0) InsertNode(sensor_parameter_recovery_linked_list_header, TAIL);    //��β������ڵ�
	}
	return 0;
}
