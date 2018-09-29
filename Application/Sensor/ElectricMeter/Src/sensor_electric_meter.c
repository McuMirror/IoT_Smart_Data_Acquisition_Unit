#include "sensor_electric_meter.h"
#include "system_common.h"
#include "malloc.h"
#include "usart.h"


#define MAX_RS485_BUFFER_LEN               64

//extern reg_data_sensor_parm_and_ID    register_data_sensor_parm_and_ID;
uint8_t electric_meter_rs485_bus_send_buffer[MAX_RS485_BUFFER_LEN];	        //RS485���ͻ���
uint8_t electric_meter_rs485_bus_send_buffer_len = 0;                       //RS485���ͻ��泤��
uint8_t rs485_cs=0;                                                         //RS485У��λ

uint8_t electric_meter_rs485_bus_read_buffer[MAX_RS485_BUFFER_LEN];	        //RS485���ջ���
uint8_t electric_meter_rs485_bus_read_buffer_len = 0;                       //RS485���ջ��泤��

sen_electric_meter_gen_para sensor_electric_meter_general_parameters = {
	.baudrate             =     2400,             //ȱʡĬ�ϲ�����Ϊ2400
	.amount               =     1,                //ȱʡĬ�ϵ������Ϊ1
	.ID_single_length     =     6,                //ȱʡĬ�ϵ������ID����Ϊ6
	.ID_total_length      =     6,                //ȱʡĬ�ϵ��ID�ܳ���Ϊ6
};


/**
  * @name         RS485_Send_Data
	* @brief        RS485���߷���len���ֽڵ�����
	* @param        *buf:ָ���ͻ������׵�ַ��ָ��
	* @param        len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
  * @retval       RS485���߷���len���ֽڵ������Ƿ�ɹ�  �ɹ��򷵻�0  ʧ���򷵻�1
	* @lastModify   2018/9/29  9:23
	* @author       JackWilliam
  */
uint8_t RS485_Send_Data(uint8_t *buf,uint8_t len){
	uint8_t t;
	uint8_t RS485_RX_CNT = 0;
	
	electric_meter_rs485_TX_EN;			//����Ϊ����ģʽ
	memset(electric_meter_rs485_bus_read_buffer,0,MAX_RS485_BUFFER_LEN);//��ս�����
	RS485_RX_CNT=0;		//����
  for(t=0;t<len;t++)		//ѭ����������
	{
		HAL_UART_Transmit_IT(&huart2, (uint8_t *)&buf[t], 1); //���յ�����Ϣ���ͳ�ȥ		
//		USART_SendData(USART2,buf[t]);
//    while( USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET );
		HAL_Delay(5);
	}	 	
//	HAL_UART_Transmit(&huart3, (uint8_t *)&buf, len,0xFFFF);
	RS485_RX_CNT=0;	  
	electric_meter_rs485_RX_EN;				//����Ϊ����ģʽ	
	
	return 0;
}


/**
  * @name         RS485_Send_Data
	* @brief        RS485��ѯ���յ�������
	* @param        *buf:ָ����ջ������׵�ַ��ָ��
	* @param        *len:ָ����ջ��泤�ȵ�ָ��
  * @retval       RS485�������ݽ����Ƿ�ɹ�  �ɹ��򷵻�0  ʧ���򷵻�1
	* @lastModify   2018/9/29  9:23
	* @author       JackWilliam
  */
uint8_t RS485_Receive_Data(uint8_t *buf,uint8_t *len){
	uint8_t RS485_RX_CNT = 0;
	uint8_t rxlen=RS485_RX_CNT;
	uint8_t i=0;
	
	electric_meter_rs485_RX_EN;				//����Ϊ����ģʽ
	memset(buf,0,MAX_RS485_BUFFER_LEN);
	*len=0;				//Ĭ��Ϊ0
	HAL_Delay(200);		//�ȴ�200ms,��������200msû�н��յ�һ������,����Ϊ���ս���
	if(rxlen==RS485_RX_CNT&&rxlen)//���յ�������,�ҽ��������
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=electric_meter_rs485_bus_read_buffer[i];	
		}		
		*len=RS485_RX_CNT;	//��¼�������ݳ���
		memset(electric_meter_rs485_bus_read_buffer,0,MAX_RS485_BUFFER_LEN);//��ս�����
		RS485_RX_CNT=0;		//����
		return 1;
	}
	return 0;
}


/**
  * @name         Electric_Meter_Baudrate_Auto_Get����δ����!!!��
  * @brief        ���ģ�鲨�����Զ���ȡ����δ����!!!��
	* @param        *sen_para:ָ������ص�ͨ�ò������ýṹ���ָ��
  * @retval       ���ģ�鲨�����Ƿ��Զ���ȡ�ɹ�  �ɹ��򷵻�0  ʧ���򷵻�1
	* @lastModify   2018/9/26  9:58
	* @author       JackWilliam
  */
uint8_t Electric_Meter_Baudrate_Auto_Get(sen_electric_meter_gen_para * sen_para){
	sen_para->baudrate = 2400;
	
	
	sprintf((char *)register_data_sensor_parm.SensorComSpeed, "%d", sen_para->baudrate);
	
	if(ee_WriteBytes(register_data_sensor_parm.SensorComSpeed, register_address_sensor_parm.SensorComSpeed, 5))
		error_code_handle(ERROR_CODE_EEPROM_Write_Bytes_FAIL);
	return 0;
}


/**
  * @name         electric_meter_address_auto_get
  * @brief        �Զ�ģʽ�µ���ַ��ȡ
	* @param        *sensor_parm_and_ID:ָ������ص�ͨ�ò���������IDһ��Ľṹ���ָ��
	* @param        retry_times:��ַ��ȡ���������Դ���
	* @retval       ���ģ�鲨�����Ƿ��Զ���ȡ�ɹ�  �ɹ��򷵻�0  ʧ���򷵻�1
	* @lastModify   2018/9/26  9:58
	* @author       JackWilliam
  */
uint8_t electric_meter_address_auto_get(uint8_t *sensor_ID, uint8_t retry_times)
{
	uint8_t i=0;
	uint8_t rx_done=0;//������ɱ�־
	char* loc=0;
	uint8_t ID_LEN = 0;        //���ID����
	
	memset(electric_meter_rs485_bus_send_buffer,0,MAX_RS485_BUFFER_LEN);
	electric_meter_rs485_bus_send_buffer_len=0;
	
	for(i=0;i<4;i++)
	{
		electric_meter_rs485_bus_send_buffer[i]=0xFE;
		electric_meter_rs485_bus_send_buffer_len++;
	}			
	
	RS485_Send_Data(electric_meter_rs485_bus_send_buffer,4);//����16���ֽ�
	
	electric_meter_rs485_bus_send_buffer[4]=0x68;
	electric_meter_rs485_bus_send_buffer_len++;
	
	for(i=5;i<11;i++)
	{
		electric_meter_rs485_bus_send_buffer[i]=0xAA;
		electric_meter_rs485_bus_send_buffer_len++;
	}
	
	electric_meter_rs485_bus_send_buffer[11]=0x68;
	electric_meter_rs485_bus_send_buffer_len++;
	
	electric_meter_rs485_bus_send_buffer[12]=0x13;
	electric_meter_rs485_bus_send_buffer_len++;
	
	electric_meter_rs485_bus_send_buffer[13]=0x00;
	electric_meter_rs485_bus_send_buffer_len++;
	
	for(i=4;i<14;i++)
		rs485_cs=rs485_cs+electric_meter_rs485_bus_send_buffer[i];			
	electric_meter_rs485_bus_send_buffer[14]=rs485_cs;
	electric_meter_rs485_bus_send_buffer_len++;
	
	electric_meter_rs485_bus_send_buffer[15]=0x16;
	electric_meter_rs485_bus_send_buffer_len++;
	
	RS485_Send_Data(electric_meter_rs485_bus_send_buffer,electric_meter_rs485_bus_send_buffer_len);//����16���ֽ�
	
	HAL_Delay(200);
//	sensor_and_ID = mymalloc(sizeof(sensor_parm_and_ID->sensor_ID));
//	memset(sensor_ID,0,sizeof(sensor_ID));
	memset(sensor_ID,0,MAX_RS485_BUFFER_LEN);
	ID_LEN=0;
	while(retry_times)//��������
	{
		retry_times--;
		rx_done = RS485_Receive_Data(electric_meter_rs485_bus_read_buffer,&electric_meter_rs485_bus_read_buffer_len);
		if(rx_done)
		{
			loc=strchr( (const char *)electric_meter_rs485_bus_read_buffer,0x68);//�����״γ���0x68��λ��
			if(loc!=NULL)
			{
				while(1)
				{
					loc++;
					if(*loc==0x68)
						break;
					else
						sensor_ID[ID_LEN++]=*loc; //ȡ����ַ
				}
				memset(electric_meter_rs485_bus_read_buffer,0,MAX_RS485_BUFFER_LEN);
				electric_meter_rs485_bus_read_buffer_len=0;
				
				while(ID_LEN)
					electric_meter_rs485_bus_read_buffer[electric_meter_rs485_bus_read_buffer_len++]=sensor_ID[--ID_LEN];				
				memset(sensor_ID,0,MAX_RS485_BUFFER_LEN);
				
				for(ID_LEN=0;ID_LEN<electric_meter_rs485_bus_read_buffer_len;ID_LEN++)
					sensor_ID[ID_LEN]=electric_meter_rs485_bus_read_buffer[ID_LEN];
				memset(electric_meter_rs485_bus_read_buffer,0,MAX_RS485_BUFFER_LEN);
				electric_meter_rs485_bus_read_buffer_len=0;
				return 1;
			}
			return 0;
		}
	}
	return 0;
}





