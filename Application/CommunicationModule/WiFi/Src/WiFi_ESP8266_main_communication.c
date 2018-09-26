#include "WiFi_ESP8266_main_communication.h"
#include "EEPROM_24C256.h"
#include "system_common.h"
//uint8_t WiFi_ESP8266_get_data_flag = 0;

/**
  * @name         WiFi_ESP8266_Driver_Init
  * @brief        WiFi ESP8266ģ��������ʼ��
  * @param        void
  * @retval       WiFi ESP8266ģ�������Ƿ��ʼ���ɹ�  �ɹ��򷵻�0  ʧ���򷵻�1
	* @lastModify   2018/9/25  17:40
	* @author       JackWilliam
  */
uint8_t WiFi_ESP8266_Driver_Init(void){
	macESP8266_RST_HIGH_LEVEL();
	macESP8266_CH_DISABLE();
	
	printf ( "\r\n�������� ESP8266 ......\r\n" );
	
	macESP8266_CH_ENABLE();
	
	ESP8266_AT_Test();
	
	ESP8266_Cmd ( "AT+RST", "OK", "no change", 5000 );
//	ESP8266_Net_Mode_Choose ( STA_AP );
	ESP8266_Net_Mode_Choose ( STA);
//	HAL_Delay ( 10000 );                 //��ʱ
  while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );	
	
//	while ( !	ESP8266_Link_Server ( enumUDP, macUser_ESP8266_UdpServer_IP, macUser_ESP8266_UdpServer_Port, Single_ID_0 ) );
	while ( !	ESP8266_UDP_Link_Server ( enumUDP, macUser_ESP8266_UdpServer_IP, macUser_ESP8266_UdpServer_Port, macUser_ESP8266_UdpLocal_Port,Single_ID_0 ) );
	
	while ( ! ESP8266_UnvarnishSend () );
	
	printf ( "\r\n���� ESP8266 ���\r\n" );
	
	//������һ֡�޷��������յ���BUG   2018.8.30
	memset(strEsp8266_Fram_Record.Data_RX_BUF, 0, sizeof(strEsp8266_Fram_Record.Data_RX_BUF));
	strEsp8266_Fram_Record.InfAll = 0;
	strEsp8266_Fram_Record.InfBit.FramFinishFlag = 0;
	strEsp8266_Fram_Record.InfBit.FramLength = 0;

	return 0;
}
