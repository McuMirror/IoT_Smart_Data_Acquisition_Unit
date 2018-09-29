#ifndef __EEPROM_24C256_H__
#define __EEPROM_24C256_H__

#include "main.h"
#include "stm32l4xx_hal.h"


/* 
 * AT24C02 2kb = 2048bit = 2048/8 B = 256 B
 * 32 pages of 8 bytes each
 *
 * Device Address
 * 1 0 1 0 A2 A1 A0 R/W
 * 1 0 1 0 0  0  0  0 = 0XA0
 * 1 0 1 0 0  0  0  1 = 0XA1 
 */

/* AT24C01/02ÿҳ��8���ֽ� 
 * AT24C04/08A/16Aÿҳ��16���ֽ� 
 */

//#define EEPROM_DEV_ADDR			0xA0		/* 24xx02���豸��ַ */
//#define EEPROM_PAGE_SIZE		  16			  /* 24xx02��ҳ���С */
//#define EEPROM_SIZE				  400			  /* 24xx02������ */

//#define EEPROM_DEV_ADDR			0xA0		/* 24xx02���豸��ַ */
//#define EEPROM_PAGE_SIZE		  16			  /* 24xx02��ҳ���С */
//#define EEPROM_SIZE				  256			  /* 24xx02������ */

#define EEPROM_DEV_ADDR			0xA0		/* 24xx02���豸��ַ */
#define EEPROM_PAGE_SIZE		  16			  /* 24xx02��ҳ���С */
#define EEPROM_SIZE				  32768			  /* 24xx02������ */

uint8_t EEPROM_24C256_Init(void);
uint8_t ee_CheckOk(void);
uint8_t ee_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize);
uint8_t ee_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize);
uint8_t ee_Erase(uint16_t _usAddress, uint16_t _usSize);
uint8_t ee_Test(void);

/******  EEPROM������  ��Ӧ������·����AP��EEPROM�滮�ĵ� v1.0.1�� *******/
/** @defgroup EEPROM�Ĵ����滮 
  * @{
  */
#define AP_REG_BASIC_SETTINGS                        ((uint16_t)0x0000)                   //AP�˻������üĴ���       
#define AP_REG_TIME_SETTINGS                         ((uint16_t)0x0080)                   //AP��ʱ��Ĵ���
#define AP_REG_WARNING_STORE                         ((uint16_t)0x0100)                   //AP�˴���澯�Ĵ���
#define AP_REG_COMMUNICATION_KEY                     ((uint16_t)0x0180)                   //ͨ����Կ�洢�Ĵ���
#define AP_REG_WIPE_KEY                              ((uint16_t)0x0200)                   //�ָ�����������Կ�洢�Ĵ���

#define AP_REG_SENSOR_PARM                          ((uint16_t)0x2000)                   //�����������洢�Ĵ���
#define AP_REG_SENSOR_ID                            ((uint16_t)0x2080)                   //������ ID�洢�Ĵ���

#define AP_REG_OFFSET                                ((uint16_t)0x0080)                   //�Ĵ���ƫ�Ƶ�ַ
#define AP_BYTE_OFFSET                               ((uint16_t)0x0008)                   //�ֽ�ƫ�Ƶ�ַ

/**
  * @brief AP�˻������üĴ��������ֽڵ�ַ����
  */

typedef struct
{
  uint16_t DataExistFlag;          /*!< EEPROM���Ƿ񱣴��������ɹ���·���������ID����Ϣ�ı�־λ, ��ַƫ��: 0x00 */
	uint16_t SlaveID;         		   /*!< ��·�����������AP������ID,                               ��ַƫ��: 0x08 */
	uint16_t MasterID;         		   /*!< ·�����˵�����ID,                                         ��ַƫ��: 0x18 */
	uint16_t SensorNum;              /*!< AP�������ӵĴ���������,                                   ��ַƫ��: 0x28 */
	uint16_t PowerSupply;            /*!< AP�˵ĵ�Դ���緽ʽ��Ĭ��Ϊ�е繩��,                       ��ַƫ��: 0x30 */
	uint16_t SavePower;              /*!< AP���Ƿ�����Ϊ����ģʽ��Ĭ��Ϊ����ģʽ,                   ��ַƫ��: 0x38 */
	uint16_t ComModuleType;          /*!< AP�������ӵ�ͨ��ģ������,                                 ��ַƫ��: 0x40 */
	uint16_t SenConAuto;            /*!< AP�������յ��Ĵ�������������ģʽ�Ƿ�Ϊ�Զ�ģʽ,            ��ַƫ��: 0x48 */
} RegAddBasicSettings;



/**
  * @brief AP�˻������üĴ�����������
  */



typedef struct
{
  uint8_t DataExistFlag;          /*!< EEPROM���Ƿ񱣴��������ɹ���·���������ID����Ϣ�ı�־λ, ��ַƫ��: 0x00 */
	uint8_t SlaveID[2];         		/*!< ��·�����������AP������ID,                               ��ַƫ��: 0x08 */
	uint8_t MasterID[2];         	  /*!< ·�����˵�����ID,                                         ��ַƫ��: 0x18 */
	uint8_t SensorNum;              /*!< AP�������ӵĴ���������,                                   ��ַƫ��: 0x28 */
	uint8_t PowerSupply;            /*!< AP�˵ĵ�Դ���緽ʽ��Ĭ��Ϊ�е繩��,                       ��ַƫ��: 0x30 */
	uint8_t SavePower;              /*!< AP���Ƿ�����Ϊ����ģʽ��Ĭ��Ϊ����ģʽ,                   ��ַƫ��: 0x38 */
	uint8_t ComModuleType;          /*!< AP�������ӵ�ͨ��ģ������,                                 ��ַƫ��: 0x40 */
	uint8_t SenConAuto;            /*!< AP�������յ��Ĵ�������������ģʽ�Ƿ�Ϊ�Զ�ģʽ,            ��ַƫ��: 0x48 */
} reg_data_basic_settings;

/**
  * @brief ComModuleType������ӵĺ궨��
  */
#define ComModuleType_LoRa_SX1278_Connected                 0x01
#define ComModuleType_WiFi_ESP8266_Connected                0x02
#define ComModuleType_BlueTooth_nRF52832_Connected          0x03
#define ComModuleType_NB_IoT_BC26_Connected                 0x04


/**
  * @brief AP��ʱ��Ĵ��������ֽڵ�ַ����
  */

typedef struct
{
  uint16_t GapTime;          /*!< ÿ�������ϱ����ڵĳ��ȣ��������ϱ��ļ��ʱ�䣬����Ϊ��λ,            ��ַƫ��: 0x00 */
	uint16_t WindowLength;     /*!< ÿ�������ϱ��Ĵ���ʱ�䣬����Ϊ��λ,                                  ��ַƫ��: 0x18 */
	uint16_t Year;         	   /*!< ���һ����·��������ʱ��ͬ����ʱ����Ϣ�е������Ϣ����BCD���ʾ,     ��ַƫ��: 0x20 */
	uint16_t Month;            /*!< ���һ����·��������ʱ��ͬ����ʱ����Ϣ�е��·���Ϣ����BCD���ʾ,     ��ַƫ��: 0x28 */
	uint16_t Day;              /*!< ���һ����·��������ʱ��ͬ����ʱ����Ϣ�е��·���Ϣ����BCD���ʾ,     ��ַƫ��: 0x30 */
	uint16_t Hour;             /*!< ���һ����·��������ʱ��ͬ����ʱ����Ϣ�е�������Ϣ����BCD���ʾ,     ��ַƫ��: 0x38 */
	uint16_t Minute;           /*!< ���һ����·��������ʱ��ͬ����ʱ����Ϣ�еķ�����Ϣ����BCD���ʾ,     ��ַƫ��: 0x40 */
	uint16_t Second;           /*!< ���һ����·��������ʱ��ͬ����ʱ����Ϣ�е�������Ϣ����BCD���ʾ,     ��ַƫ��: 0x48 */
	uint16_t Week;             /*!< ���һ����·��������ʱ��ͬ����ʱ����Ϣ�е�������Ϣ����BCD���ʾ,     ��ַƫ��: 0x50 */
} RegAddTimeSettings;



/**
  * @brief AP��ʱ��Ĵ�����������
  */

typedef struct
{
  uint8_t GapTime[3];       /*!< ÿ�������ϱ����ڵĳ��ȣ��������ϱ��ļ��ʱ�䣬����Ϊ��λ,            ��ַƫ��: 0x00 */
	uint8_t WindowLength;     /*!< ÿ�������ϱ��Ĵ���ʱ�䣬����Ϊ��λ,                                  ��ַƫ��: 0x18 */
	uint8_t Year;         	  /*!< ���һ����·��������ʱ��ͬ����ʱ����Ϣ�е������Ϣ����BCD���ʾ,     ��ַƫ��: 0x20 */
	uint8_t Month;            /*!< ���һ����·��������ʱ��ͬ����ʱ����Ϣ�е��·���Ϣ����BCD���ʾ,     ��ַƫ��: 0x28 */
	uint8_t Day;              /*!< ���һ����·��������ʱ��ͬ����ʱ����Ϣ�е��·���Ϣ����BCD���ʾ,     ��ַƫ��: 0x30 */
	uint8_t Hour;             /*!< ���һ����·��������ʱ��ͬ����ʱ����Ϣ�е�������Ϣ����BCD���ʾ,     ��ַƫ��: 0x38 */
	uint8_t Minute;           /*!< ���һ����·��������ʱ��ͬ����ʱ����Ϣ�еķ�����Ϣ����BCD���ʾ,     ��ַƫ��: 0x40 */
	uint8_t Second;           /*!< ���һ����·��������ʱ��ͬ����ʱ����Ϣ�е�������Ϣ����BCD���ʾ,     ��ַƫ��: 0x48 */
	uint8_t Week;             /*!< ���һ����·��������ʱ��ͬ����ʱ����Ϣ�е�������Ϣ����BCD���ʾ,     ��ַƫ��: 0x50 */
} reg_data_time_settings;

/**
  * @brief AP�˴���澯�Ĵ��������ֽڵ�ַ����
  */

typedef struct
{
	/*!< AP���Ƿ��ѽ��澯��Ϣ�ϱ���·�����������յ�·�����澯��Ϣȷ�Ϻ�
				��ᱻ�����������,            
				��ַƫ��: 0x00 */
  uint16_t AlarmUpdata;
	
	/*!< AP���Ƿ���������ͨ��ģ���ʼ�������г�����Ҫ�����λ���������Ĵ���״̬��
				������״̬�������λ����ȷ��������ᱻ�����������,                                  
				��ַƫ��: 0x08 */	
	uint16_t InitAlarm;     
	
	/*!< AP���ڽ�ʵ�������ϴ���·�����Ĺ����У���������ʱ���ھ�δ�յ�����·�����������ϴ�ȷ�ϣ�
				���ڽ������������ڵĴ���ʱ�����յ�����·��������Ӧ����ᱻ�����������,     
				��ַƫ��: 0x10 */
	uint16_t DataUploadAlarm;         	
	
	/*!< AP������·�����Ĵ��������������ʱ��ͬ�������г��ִ���״̬��
				������״̬�����½������ݰ�����ȷ��������ᱻ�����������,     
				��ַƫ��: 0x18 */
	uint16_t SyncAlarm;   
	
	/*!< AP�˹�����ʹ�õĵ�ص������޷�����AP�������������е�Ҫ��
				����ر��ɹ����������λ��������,     
				��ַƫ��: 0x20 */
	uint16_t LowPowerAlarm;              
	
	/*!< AP�˸澯��Ϣ�ϴ���Ӧ��Ĵ�����
				���յ�һ������·�����ĸ澯��Ϣȷ�Ϻ���ᱻ�����������,     
				��ַƫ��: 0x40 */
	uint16_t AlarmUploadNoACKCount;             
	
	/*!< AP����������ͨ��ģ���ʼ�������г�����Ҫ�����λ���������Ĵ���״̬����������λ�Ĵ�����
				������״̬�������λ����ȷ��������ᱻ�����������,     
				��ַƫ��: 0x48 */
	uint16_t InitResetCount;           
	
	/*!< AP�������ϴ���Ӧ��Ĵ��������յ�һ������·�����������ϴ�ȷ�Ϻ�
				��ᱻ�����������,     
				��ַƫ��: 0x50 */
	uint16_t DataUploadNoACKCount; 
} RegAddWarningStore;


/**
  * @brief AP�˴���澯�Ĵ�����������
  */

typedef struct
{
	/*!< AP���Ƿ��ѽ��澯��Ϣ�ϱ���·�����������յ�·�����澯��Ϣȷ�Ϻ�
				��ᱻ�����������,            
				��ַƫ��: 0x00 */
  uint8_t AlarmUpdata;
	
	/*!< AP���Ƿ���������ͨ��ģ���ʼ�������г�����Ҫ�����λ���������Ĵ���״̬��
				������״̬�������λ����ȷ��������ᱻ�����������,                                  
				��ַƫ��: 0x08 */	
	uint8_t InitAlarm;     
	
	/*!< AP���ڽ�ʵ�������ϴ���·�����Ĺ����У���������ʱ���ھ�δ�յ�����·�����������ϴ�ȷ�ϣ�
				���ڽ������������ڵĴ���ʱ�����յ�����·��������Ӧ����ᱻ�����������,     
				��ַƫ��: 0x10 */
	uint8_t DataUploadAlarm;         	
	
	/*!< AP������·�����Ĵ��������������ʱ��ͬ�������г��ִ���״̬��
				������״̬�����½������ݰ�����ȷ��������ᱻ�����������,     
				��ַƫ��: 0x18 */
	uint8_t SyncAlarm;   
	
	/*!< AP�˹�����ʹ�õĵ�ص������޷�����AP�������������е�Ҫ��
				����ر��ɹ����������λ��������,     
				��ַƫ��: 0x20 */
	uint8_t LowPowerAlarm;              
	
	/*!< AP�˸澯��Ϣ�ϴ���Ӧ��Ĵ�����
				���յ�һ������·�����ĸ澯��Ϣȷ�Ϻ���ᱻ�����������,     
				��ַƫ��: 0x40 */
	uint8_t AlarmUploadNoACKCount;             
	
	/*!< AP����������ͨ��ģ���ʼ�������г�����Ҫ�����λ���������Ĵ���״̬����������λ�Ĵ�����
				������״̬�������λ����ȷ��������ᱻ�����������,     
				��ַƫ��: 0x48 */
	uint8_t InitResetCount;           
	
	/*!< AP�������ϴ���Ӧ��Ĵ��������յ�һ������·�����������ϴ�ȷ�Ϻ�
				��ᱻ�����������,     
				��ַƫ��: 0x50 */
	uint8_t DataUploadNoACKCount; 
} reg_data_warning_store;

/**
  * @brief ͨ����Կ�洢�Ĵ��������ֽڵ�ַ����
  */
typedef struct
{
	  uint16_t CommunicationKey;          /*!< AP����·����ͨ��ʱʹ�õ�AES-128bit��Ϣ���ܵ���Կ������Կ����·�������и���,            ��ַƫ��: 0x00 */
} RegAddCommunicationKey;


/**
  * @brief ͨ����Կ�洢�Ĵ�����������
  */
typedef struct
{
	  uint8_t CommunicationKey[16];          /*!< AP����·����ͨ��ʱʹ�õ�AES-128bit��Ϣ���ܵ���Կ������Կ����·�������и���,            ��ַƫ��: 0x00 */
} reg_data_communication_key;

/**
  * @brief �ָ�����������Կ�洢�Ĵ��������ֽڵ�ַ����
  */
typedef struct
{
	  uint16_t WipeKey;          /*!< AP�˻ָ���������ʱ����Ҫ��֤��16λ��Կ������Կ������·�������и���,            ��ַƫ��: 0x00 */
} RegAddWipeKey;



/**
  * @brief �ָ�����������Կ�洢�Ĵ�����������
  */
typedef struct
{
	  uint8_t WipeKey[16];          /*!< AP�˻ָ���������ʱ����Ҫ��֤��16λ��Կ������Կ������·�������и���,            ��ַƫ��: 0x00 */
} reg_data_wipe_key;


/**
  * @brief �����������洢�Ĵ��������ֽڵ�ַ����
  */

typedef struct
{ 
  uint16_t SensorType;            /*!< AP�������ӵĴ���������,            ��ַƫ��: 0x00 */
	uint16_t SensorParaNum;         /*!< AP�������ӵĴ�������Ҫ�ɼ�������������Ŀ,         ��ַƫ��: 0x08 */
	uint16_t SensorIDLen;         	/*!< AP�������ӵĴ�������ID����,     ��ַƫ��: 0x10 */
	
	/*!< AP�������ӵĲ�ͬ����������ͨ������Ҫ��ͨ�����ʣ�������������Ϊ���
				�������������ǵ���Ӧ��RS485���ڲ����ʣ�ʮ����������,     
				��ַƫ��: 0x18 */
	uint16_t SensorComSpeed; 

	/*!< ·������Ҫ��AP�����ӵĵ�һ������������Ҫ�ϱ����������ͣ�ÿһλ����һ�ִ�����������
				1����ò�����Ҫ�ɼ��ϴ���0����ò�������ɼ��ϴ�,     
				��ַƫ��: 0x40 */	
	uint16_t sensorParameter;     
} RegAddsensorParm;


/**
  * @brief �����������洢�Ĵ�����������
  */

typedef struct
{ 
  uint8_t SensorType;            /*!< AP�������ӵĴ���������,            ��ַƫ��: 0x00 */
	uint8_t SensorParaNum;         /*!< AP�������ӵĴ�������Ҫ�ɼ�������������Ŀ,         ��ַƫ��: 0x08 */
	uint8_t SensorIDLen;         	/*!< AP�������ӵĴ�������ID����,     ��ַƫ��: 0x10 */
	
	/*!< AP�������ӵĲ�ͬ����������ͨ������Ҫ��ͨ�����ʣ�������������Ϊ���
				�������������ǵ���Ӧ��RS485���ڲ����ʣ�ʮ����������,     
				��ַƫ��: 0x18 */
	uint8_t SensorComSpeed[5]; 

	/*!< ·������Ҫ��AP�����ӵĵ�һ������������Ҫ�ϱ����������ͣ�ÿһλ����һ�ִ�����������
				1����ò�����Ҫ�ɼ��ϴ���0����ò�������ɼ��ϴ�,     
				��ַƫ��: 0x40 */	
	uint8_t sensorParameter[8];     
} reg_data_sensor_parm;


/**
  * @brief ������ ID�洢�Ĵ��������ֽڵ�ַ����
  */
typedef struct
{
	  uint16_t sensorID;          /*!< ������ID��ַ����,            ��ַƫ��: 0x00 */
} RegAddsensorID;


/**
  * @brief SensorType��صĺ궨��
  */
#define SensorType_Electric_Meter_Connected                 0x01
#define SensorType_Water_Meter_Connected                    0x02
#define SensorType_Gas_Meter_Connected                      0x03
#define SensorType_Image_Source_Connected                   0x04
#define SensorType_Other_Senser_Connected                   0x05

/**
  * @brief ������ ID�洢�Ĵ�����������
  */
typedef struct
{
	  uint8_t sensorID[16];          /*!< ������IDʵ�����ݶ���,            ��ַƫ��: 0x00 */
} reg_data_sensor_ID;


/**
  * @brief ������������ID��ͬ����洢�Ĵ�����������
  */

typedef struct
{ 
	reg_data_sensor_parm   * sensor_parm;
	reg_data_sensor_ID     * sensor_ID;
} reg_data_sensor_parm_and_ID;

extern reg_data_basic_settings        register_data_basic_settings;
extern reg_data_time_settings         register_data_time_settings;
extern reg_data_warning_store         register_data_warning_store;
extern reg_data_communication_key     register_data_communication_key;
extern reg_data_wipe_key              register_data_wipe_key;
extern reg_data_sensor_parm          register_data_sensor_parm;
extern reg_data_sensor_ID            register_data_sensor_ID;



extern RegAddBasicSettings register_address_basic_settings;
extern RegAddTimeSettings register_address_time_settings;
extern RegAddWarningStore register_address_warning_store;
extern RegAddCommunicationKey register_address_communication_key;
extern RegAddWipeKey register_address_wipe_key;
extern RegAddsensorParm register_address_sensor_parm;
extern RegAddsensorID register_address_sensor_ID;
extern RegAddsensorParm register_address_sensor_parm;

/******  EEPROM������  ��Ӧ������·����AP��EEPROM�滮�ĵ� v1.0.1�� *******/

#endif
