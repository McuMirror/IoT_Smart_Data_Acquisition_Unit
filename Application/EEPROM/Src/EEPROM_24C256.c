#include "EEPROM_24C256.h"
#include "EEPROM_Software_I2C_Definition.h"
#include "gpio.h"

/**
  * @brief AP�˻������üĴ��������ֽڵ�ַ����
  */
RegAddBasicSettings register_address_basic_settings = {
	.DataExistFlag    =   AP_REG_BASIC_SETTINGS,
	.SlaveID          =   AP_REG_BASIC_SETTINGS + ((uint16_t)0x0008),
	.MasterID         =   AP_REG_BASIC_SETTINGS + ((uint16_t)0x0018),
	.SensorNum        =   AP_REG_BASIC_SETTINGS + ((uint16_t)0x0028),
	.PowerSupply      =   AP_REG_BASIC_SETTINGS + ((uint16_t)0x0030),
	.SavePower        =   AP_REG_BASIC_SETTINGS + ((uint16_t)0x0038),
	.ComModuleType    =   AP_REG_BASIC_SETTINGS + ((uint16_t)0x0040),
	.SenConAuto       =   AP_REG_BASIC_SETTINGS + ((uint16_t)0x0048)
};

/**
  * @brief AP��ʱ��Ĵ��������ֽڵ�ַ����
  */
RegAddTimeSettings register_address_time_settings = {
	.GapTime        =   AP_REG_TIME_SETTINGS,
	.WindowLength   =   AP_REG_TIME_SETTINGS + ((uint16_t)0x0018),
	.Year           =   AP_REG_TIME_SETTINGS + ((uint16_t)0x0020),
	.Month          =   AP_REG_TIME_SETTINGS + ((uint16_t)0x0028),
	.Day            =   AP_REG_TIME_SETTINGS + ((uint16_t)0x0030),
	.Hour           =   AP_REG_TIME_SETTINGS + ((uint16_t)0x0038),
	.Minute         =   AP_REG_TIME_SETTINGS + ((uint16_t)0x0040),
	.Second         =   AP_REG_TIME_SETTINGS + ((uint16_t)0x0048),
	.Week           =   AP_REG_TIME_SETTINGS + ((uint16_t)0x0050)
};

/**
  * @brief AP�˴���澯�Ĵ��������ֽڵ�ַ����
  */
RegAddWarningStore register_address_warning_store = {
	.AlarmUpdata              = AP_REG_WARNING_STORE,
	.InitAlarm                = AP_REG_WARNING_STORE + ((uint16_t)0x0008),
	.DataUploadAlarm          = AP_REG_WARNING_STORE + ((uint16_t)0x0010),
	.SyncAlarm                = AP_REG_WARNING_STORE + ((uint16_t)0x0018),
	.LowPowerAlarm            = AP_REG_WARNING_STORE + ((uint16_t)0x0020),
	.AlarmUploadNoACKCount    = AP_REG_WARNING_STORE + ((uint16_t)0x0040),
	.InitResetCount           = AP_REG_WARNING_STORE + ((uint16_t)0x0048),
	.DataUploadNoACKCount     = AP_REG_WARNING_STORE + ((uint16_t)0x0050),
};

/**
  * @brief ͨ����Կ�洢�Ĵ��������ֽڵ�ַ����
  */
RegAddCommunicationKey register_address_communication_key = {
	.CommunicationKey              = AP_REG_COMMUNICATION_KEY
};

/**
  * @brief �ָ�����������Կ�洢�Ĵ��������ֽڵ�ַ����
  */
RegAddWipeKey register_address_wipe_key = {
	.WipeKey              = AP_REG_WIPE_KEY
};

/**
  * @brief �����������洢�Ĵ��������ֽڵ�ַ����
  */

RegAddsensorParm register_address_sensor_parm = {
	.SensorType        =   AP_REG_SENSOR_PARM,
	.SensorParaNum     =   AP_REG_SENSOR_PARM + ((uint16_t)0x0008),
	.SensorIDLen       =   AP_REG_SENSOR_PARM + ((uint16_t)0x0010),
	.SensorComSpeed    =   AP_REG_SENSOR_PARM + ((uint16_t)0x0018),
	.sensorParameter   =   AP_REG_SENSOR_PARM + ((uint16_t)0x0040),
};

reg_data_basic_settings        register_data_basic_settings;
reg_data_time_settings         register_data_time_settings;
reg_data_warning_store         register_data_warning_store;
reg_data_communication_key     register_data_communication_key;
reg_data_wipe_key              register_data_wipe_key;
reg_data_sensor_parm           register_data_sensor_parm;
reg_data_sensor_ID             register_data_sensor_ID;
reg_data_sensor_parm_and_ID    register_data_sensor_parm_and_ID = {
	.sensor_parm = NULL,
	.sensor_ID = NULL
};

/**
  * @brief ������1 ID�洢�Ĵ��������ֽڵ�ַ����
  */
RegAddsensorID register_address_sensor_ID = {
	.sensorID              = AP_REG_SENSOR_ID
};




/**
  * @name         EEPROM_24C256_Init
  * @brief        ����24C256 EEPROM ��ʼ��
  * @param        void
  * @retval       24C256 EEPROM�Ƿ��ʼ���ɹ�  �ɹ��򷵻�0  ʧ���򷵻�1
	* @lastModify   2018/9/25  11:18
	* @author       JackWilliam
  */
uint8_t EEPROM_24C256_Init(void){
	if(GPIO_EEPROM_24C256_Init()) return 1;
	if(ee_CheckOk()) return 1;
	
	return 0;
}


/**
  * @name         ee_CheckOk
  * @brief        �ж�24C256 EERPOM�Ƿ�����
  * @param        void
  * @retval       24C256 EEPROM�Ƿ��ʼ���ɹ�  �ɹ��򷵻�0  ʧ���򷵻�1
	* @lastModify   2018/9/25  11:18
	* @author       JackWilliam
  */
uint8_t ee_CheckOk(void)
{
	if (i2c_CheckDevice(EEPROM_DEV_ADDR) == 0)
	{
		return 0;
	}
	else
	{
		/* ʧ�ܺ��мǷ���I2C����ֹͣ�ź� */
		i2c_Stop();		
		return 1;
	}
}

/**
  * @name         ee_ReadBytes
  * @brief        ��24C256 EERPOMָ����ַ����ʼ��ȡ��������
  * @param        _usAddress : ��ʼ��ַ
  * @param        _usSize : ���ݳ��ȣ���λΪ�ֽ�
  * @param        _pReadBuf : ��Ŷ��������ݵĻ�����ָ��
  * @retval       24C256 EEPROM���ݶ�ȡ�Ƿ�ɹ�  �ɹ��򷵻�0  ʧ���򷵻�1
	* @lastModify   2018/9/25  11:18
	* @author       JackWilliam
  */
uint8_t ee_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i;
	
	/* ���ô���EEPROM�漴��ȡָ�����У�������ȡ�����ֽ� */
	
	/* ��1��������I2C���������ź� */
	i2c_Start();
	
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	i2c_SendByte(EEPROM_DEV_ADDR | EEPROM_I2C_WR);	/* �˴���дָ�� */
	 
	/* ��3�����ȴ�ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}

	/* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
	i2c_SendByte(_usAddress << 8);
	
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	
	i2c_SendByte((uint8_t)_usAddress);
	/* ��5�����ȴ�ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	
	/* ��6������������I2C���ߡ�ǰ��Ĵ����Ŀ����EEPROM���͵�ַ�����濪ʼ��ȡ���� */
	i2c_Start();
	
	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	i2c_SendByte(EEPROM_DEV_ADDR | EEPROM_I2C_RD);	/* �˴��Ƕ�ָ�� */
	
	/* ��8��������ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}	
	
	/* ��9����ѭ����ȡ���� */
	for (i = 0; i < _usSize; i++)
	{
		_pReadBuf[i] = i2c_ReadByte();	/* ��1���ֽ� */
		
		/* ÿ����1���ֽں���Ҫ����Ack�� ���һ���ֽڲ���ҪAck����Nack */
		if (i != _usSize - 1)
		{
			i2c_Ack();	/* �м��ֽڶ����CPU����ACK�ź�(����SDA = 0) */
		}
		else
		{
			i2c_NAck();	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
		}
	}
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();
	return 0;	/* ִ�гɹ� */

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();
	return 1;
}


/**
  * @name         ee_WriteBytes
  * @brief        ��24C256 EERPOMָ����ַд���������ݣ�����ҳд�������д��Ч��
  * @param        _usAddress : ��ʼ��ַ
  * @param        _usSize : ���ݳ��ȣ���λΪ�ֽ�
  * @param        _pWriteBuf : ��Ŷ��������ݵĻ�����ָ��
  * @retval       24C256 EEPROM����д���Ƿ�ɹ�  �ɹ��򷵻�0  ʧ���򷵻�1
	* @lastModify   2018/9/25  11:18
	* @author       JackWilliam
  */
uint8_t ee_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i,m;
	uint16_t usAddr;
	
	/* 
		д����EEPROM�������������������ȡ�ܶ��ֽڣ�ÿ��д����ֻ����ͬһ��page��
		����24xx02��page size = 8
		�򵥵Ĵ�����Ϊ�����ֽ�д����ģʽ��ûд1���ֽڣ������͵�ַ
		Ϊ���������д��Ч��: ����������page wirte������
	*/

	usAddr = _usAddress;	
	for (i = 0; i < _usSize; i++)
	{
		/* �����͵�1���ֽڻ���ҳ���׵�ַʱ����Ҫ���·��������źź͵�ַ */
		if ((i == 0) || (usAddr & (EEPROM_PAGE_SIZE - 1)) == 0)
		{
			/*���ڣ�������ֹͣ�źţ������ڲ�д������*/
			i2c_Stop();
			
			/* ͨ���������Ӧ��ķ�ʽ���ж��ڲ�д�����Ƿ����, һ��С�� 10ms 			
				CLKƵ��Ϊ200KHzʱ����ѯ����Ϊ30������
			*/
			for (m = 0; m < 1000; m++)
			{				
				/* ��1��������I2C���������ź� */
				i2c_Start();
				
				/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
				i2c_SendByte(EEPROM_DEV_ADDR | EEPROM_I2C_WR);	/* �˴���дָ�� */
				
				/* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
				if (i2c_WaitAck() == 0)
				{
					break;
				}
			}
			if (m  == 1000)
			{
				goto cmd_fail;	/* EEPROM����д��ʱ */
			}
		
			/* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
			i2c_SendByte(usAddr << 8);
			
			if (i2c_WaitAck() != 0)
			{
				goto cmd_fail;	/* EEPROM������Ӧ�� */
			}
			
			i2c_SendByte((uint8_t)usAddr);
			
			/* ��5�����ȴ�ACK */
			if (i2c_WaitAck() != 0)
			{
				goto cmd_fail;	/* EEPROM������Ӧ�� */
			}
		}
	
		/* ��6������ʼд������ */
		i2c_SendByte(_pWriteBuf[i]);
	
		/* ��7��������ACK */
		if (i2c_WaitAck() != 0)
		{
			goto cmd_fail;	/* EEPROM������Ӧ�� */
		}

		usAddr++;	/* ��ַ��1 */		
	}
	
	/* ����ִ�гɹ�������I2C����ֹͣ�ź� */
	i2c_Stop();
	return 0;

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();
	return 1;
}



/**
  * @name         ee_Erase
  * @brief        ��24C256 EERPOMָ����ַ��������
	* @param        _usAddress : ��ʼ��ַ
  * @param        _usSize : ���ݳ��ȣ���λΪ�ֽ�
  * @retval       24C256 EEPROM���ݲ����Ƿ�ɹ�  �ɹ��򷵻�0  ʧ���򷵻�1
	* @lastModify   2018/9/25  11:18
	* @author       JackWilliam
  */
uint8_t ee_Erase(uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i;
	uint8_t buf[_usSize];
	
	/* ��仺���� */
	for (i = 0; i < _usSize; i++)
	{
		buf[i] = 0xFF;
	}
	
	/* дEEPROM, ��ʼ��ַ = 0�����ݳ���Ϊ 256 */
	if (ee_WriteBytes(buf, _usAddress, _usSize) == 0)
	{
		printf("����eeprom����\r\n");
		return 1;
	}
	else
	{
		printf("����eeprom�ɹ���\r\n");
		return 0;
	}
}


/*--------------------------------------------------------------------------------------------------*/
static void ee_Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}


/*
 * eeprom AT24C02 ��д����
 * ��������0���쳣����1
 */
uint8_t ee_Test(void) 
{
  uint16_t i;
	uint8_t write_buf[256];
  uint8_t read_buf[256];
  
/*-----------------------------------------------------------------------------------*/  
  if (ee_CheckOk() == 1)
	{
		/* û�м�⵽EEPROM */
		printf("û�м�⵽����EEPROM!\r\n");
				
		return 1;
	}
/*------------------------------------------------------------------------------------*/  
  /* �����Ի����� */
//	for (i = 0; i < EEPROM_SIZE; i++)
//	{		
//		write_buf[i] = i;
//	}
	for (i = 0; i < 256; i++)
	{		
		write_buf[i] = i;
	}
/*------------------------------------------------------------------------------------*/  
//  if (ee_WriteBytes(write_buf, 0, EEPROM_SIZE) == 0)
//	{
//		printf("дeeprom����\r\n");
//		return 0;
//	}
//	else
//	{		
//		printf("дeeprom�ɹ���\r\n");
//	}
	if (ee_WriteBytes(write_buf, 0, 256) == 0)
	{
		printf("дeeprom����\r\n");
		return 1;
	}
	else
	{		
		printf("дeeprom�ɹ���\r\n");
	}
  
  /*д��֮����Ҫ�ʵ�����ʱ��ȥ������Ȼ�����*/
  ee_Delay(0x0FFFFF);
/*-----------------------------------------------------------------------------------*/
//  if (ee_ReadBytes(read_buf, 0, EEPROM_SIZE) == 0)
//	{
//		printf("��eeprom����\r\n");
//		return 0;
//	}
//	else
//	{		
//		printf("��eeprom�ɹ����������£�\r\n");
//	}
	  if (ee_ReadBytes(read_buf, 0, 256) == 0)
	{
		printf("��eeprom����\r\n");
		return 1;
	}
	else
	{		
		printf("��eeprom�ɹ����������£�\r\n");
	}
/*-----------------------------------------------------------------------------------*/  
//  for (i = 0; i < EEPROM_SIZE; i++)
//	{
//		if(read_buf[i] != write_buf[i])
//		{
//			printf("0x%02X ", read_buf[i]);
//			printf("����:EEPROM������д������ݲ�һ��");
//			return 0;
//		}
//    printf(" %02X", read_buf[i]);
//		
//		if ((i & 15) == 15)
//		{
//			printf("\r\n");	
//		}		
//	}
	
	for (i = 0; i < 256; i++)
	{
		if(read_buf[i] != write_buf[i])
		{
			printf("0x%02X ", read_buf[i]);
			printf("����:EEPROM������д������ݲ�һ��");
			return 1;
		}
    printf(" %02X", read_buf[i]);
		
		if ((i & 15) == 15)
		{
			printf("\r\n");	
		}		
	}
  printf("eeprom��д���Գɹ�\r\n");
  return 0;
}
/*********************************************END OF FILE**********************/
