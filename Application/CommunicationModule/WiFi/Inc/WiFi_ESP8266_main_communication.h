#ifndef __WIFI_ESP8266_MAIN_COMMUNICATION_H__
#define __WIFI_ESP8266_MAIN_COMMUNICATION_H__

#include "main.h"
#include "stm32l4xx_hal.h"
#include "ESP8266_driver.h"

/********************************** �û���Ҫ���õĲ���**********************************/
//#define      macUser_ESP8266_ApSsid                       "ESP8266TEST"         //Ҫ���ӵ��ȵ������
//#define      macUser_ESP8266_ApPwd                        "12345678"           //Ҫ���ӵ��ȵ����Կ

//#define      macUser_ESP8266_UdpServer_IP                 "192.168.4.1"      //Ҫ���ӵķ������� IP
//#define      macUser_ESP8266_UdpServer_Port               "9090"               //Ҫ���ӵķ������Ķ˿�
//#define      macUser_ESP8266_UdpLocal_Port               "8080"               //�����˿�

#define      macUser_ESP8266_ApSsid                       "Sky"                //Ҫ���ӵ��ȵ������
#define      macUser_ESP8266_ApPwd                        "01452108"           //Ҫ���ӵ��ȵ����Կ

#define      macUser_ESP8266_UdpServer_IP                 "192.168.1.106"      //Ҫ���ӵķ������� IP
#define      macUser_ESP8266_UdpServer_Port               "9090"               //Ҫ���ӵķ������Ķ˿�

#define      macUser_ESP8266_UdpLocal_Port               "8080"               //�����˿�


uint8_t WiFi_ESP8266_Driver_Init(void);

#endif

