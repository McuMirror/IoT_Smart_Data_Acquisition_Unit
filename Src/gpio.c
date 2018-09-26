/**
  ******************************************************************************
  * File Name          : gpio.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, sensor_electric_meter_rs485_EN_Pin|state_connected_LED_Pin|state_initialization_failed_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, communication_wifi_EN_Pin|communication_wifi_RST_Pin|communication_lora_NSS_Pin|eeprom_SCL_Pin 
                          |eeprom_SDA_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = unused_button_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(unused_button_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin PAPin */
  GPIO_InitStruct.Pin = sensor_electric_meter_rs485_EN_Pin|state_connected_LED_Pin|state_initialization_failed_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin PAPin PAPin */
  GPIO_InitStruct.Pin = module_select_button_COM_LoRa_Pin|module_select_button_COM_WiFi_Pin|module_select_button_COM_BlueTooth_Pin|module_select_button_COM_NB_IoT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin 
                           PBPin */
  GPIO_InitStruct.Pin = module_select_button_SEN_ElectricMeter_Pin|module_select_button_SEN_WaterMeter_Pin|module_select_button_SEN_GasMeter_Pin|communication_lora_DIO0_Pin 
                          |communication_lora_DIO2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin */
  GPIO_InitStruct.Pin = communication_wifi_EN_Pin|communication_wifi_RST_Pin|communication_lora_NSS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin */
  GPIO_InitStruct.Pin = eeprom_SCL_Pin|eeprom_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */
/**
  * @brief   ����Ƿ��а�������
  * @param   ����Ķ˿ںͶ˿�λ
  *		@arg GPIOx: x�����ǣ�A...G�� 
  *		@arg GPIO_PIN ������GPIO_PIN_x��x������1...16��
  * @retval  ������״̬
  *		@arg KEY_ON:��������
  *		@arg KEY_OFF:����û����
  */
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin){			
	/*����Ƿ��а������� */
	if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) == KEY_ON )  
	{	 
		/*�ȴ������ͷ� */
		while(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) == KEY_ON);   
		return 	KEY_ON;	 
	}
	else
		return KEY_OFF;
}


/**
  * @name         GPIO_LED_Init
  * @brief        LED���GPIO�ܽų�ʼ��
  * @param        void
  * @retval       LED���GPIO�ܽ��Ƿ��ʼ���ɹ�  �ɹ��򷵻�0  ʧ���򷵻�1
	* @lastModify   2018/9/25  10:09
	* @author       JackWilliam
  */
uint8_t GPIO_LED_Init(void){
  GPIO_InitTypeDef GPIO_InitStruct;
	
	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	/*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, state_connected_LED_Pin|state_initialization_failed_LED_Pin, GPIO_PIN_RESET);
	
	/*Configure GPIO pins : PAPin PAPin */
  GPIO_InitStruct.Pin = state_connected_LED_Pin | state_initialization_failed_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	return 0;
}

/**
  * @name         GPIO_EEPROM_24C256_Init
  * @brief        EEPROM 24C256���GPIO�ܽų�ʼ��
  * @param        void
  * @retval       EEPROM 24C256���GPIO�ܽ��Ƿ��ʼ���ɹ�  �ɹ��򷵻�0  ʧ���򷵻�1
	* @lastModify   2018/9/25  11:19
	* @author       JackWilliam
  */
uint8_t GPIO_EEPROM_24C256_Init(void){
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
	
	  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, eeprom_SCL_Pin | eeprom_SDA_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PBPin PBPin */
  GPIO_InitStruct.Pin = eeprom_SCL_Pin|eeprom_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	return 0;
}


/**
  * @name         GPIO_DIP_Switch_Init
  * @brief        ���뿪�����GPIO�ܽų�ʼ��
  * @param        void
  * @retval       ���뿪�����GPIO�ܽ��Ƿ��ʼ���ɹ�  �ɹ��򷵻�0  ʧ���򷵻�1
	* @lastModify   2018/9/25  12:58
	* @author       JackWilliam
  */
uint8_t GPIO_DIP_Switch_Init(void){
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pins : PAPin PAPin PAPin PAPin */
  GPIO_InitStruct.Pin = module_select_button_COM_LoRa_Pin|module_select_button_COM_WiFi_Pin|module_select_button_COM_BlueTooth_Pin|module_select_button_COM_NB_IoT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	/*Configure GPIO pins : PBPin PBPin PBPin PBPin 
                           PBPin */
  GPIO_InitStruct.Pin = module_select_button_SEN_ElectricMeter_Pin|module_select_button_SEN_WaterMeter_Pin|module_select_button_SEN_GasMeter_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	return 0;
}

/**
  * @name         GPIO_LoRa_SX1278_Extra_Init
  * @brief        LoRa SX1278ģ���SPIͨ�ű�Ҫ�ܽ������������GPIO�ܽų�ʼ��
  * @param        void
  * @retval       LoRa SX1278ģ���SPIͨ�ű�Ҫ�ܽ������������GPIO�ܽ��Ƿ��ʼ���ɹ�  �ɹ��򷵻�0  ʧ���򷵻�1
	* @lastModify   2018/9/25  14:23
	* @author       JackWilliam
  */
uint8_t GPIO_LoRa_SX1278_Extra_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	
	  __HAL_RCC_GPIOB_CLK_ENABLE();
	
	 /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, communication_lora_NSS_Pin, GPIO_PIN_RESET);
	
  /*Configure GPIO pins : PBPin PBPin*/
  GPIO_InitStruct.Pin = communication_lora_DIO0_Pin|communication_lora_DIO2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	/*Configure GPIO pins : PBPin*/
  GPIO_InitStruct.Pin = communication_lora_NSS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	return 0;
}


/**
  * @name         GPIO_WiFi_ESP8266_Extra_Init
  * @brief        WiFi ESP8266ģ���USART3ͨ�ű�Ҫ�ܽ������������GPIO�ܽų�ʼ��
  * @param        void
  * @retval       WiFi ESP8266ģ���USART3ͨ�ű�Ҫ�ܽ������������GPIO�ܽ��Ƿ��ʼ���ɹ�  �ɹ��򷵻�0  ʧ���򷵻�1
	* @lastModify   2018/9/25  15:47
	* @author       JackWilliam
  */
uint8_t GPIO_WiFi_ESP8266_Extra_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, communication_wifi_EN_Pin | communication_wifi_RST_Pin, GPIO_PIN_RESET);
	
  /*Configure GPIO pins : PBPin PBPin PBPin */
  GPIO_InitStruct.Pin = communication_wifi_EN_Pin|communication_wifi_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	return 0;
}


/**
  * @name         GPIO_DIP_Switch_Com_Mod_Read
  * @brief        ��ȡͨ��ģ��ѡ����ص�GPIO��
  * @param        *com_mod_phy_connected: ָ�򱣳�ͨ��ģ��ѡ���뿪��������GPIO��״̬��com_mod_phy_connected�ṹ���ָ��
  * @retval       ͨ��ģ��ѡ����ص�GPIO���Ƿ��ȡ�ɹ�  �ɹ��򷵻�0  ʧ���򷵻�1
	* @lastModify   2018/9/25  13:51
	* @author       JackWilliam
  */
uint8_t GPIO_DIP_Switch_Com_Mod_Read(com_mod_phy_connected * physical_connected_status){
	physical_connected_status->lora_sx1278_connected = Key_Scan(module_select_button_COM_LoRa_GPIO_Port, module_select_button_COM_LoRa_Pin);
	physical_connected_status->wifi_esp8266_connected = Key_Scan(module_select_button_COM_WiFi_GPIO_Port, module_select_button_COM_WiFi_Pin);
	physical_connected_status->bluetooth_nrf52832_connected = Key_Scan(module_select_button_COM_BlueTooth_GPIO_Port, module_select_button_COM_BlueTooth_Pin);
	physical_connected_status->nb_iot_bc26_connected = Key_Scan(module_select_button_COM_NB_IoT_GPIO_Port, module_select_button_COM_NB_IoT_Pin);
	
	return 0;
}

/**
  * @name         GPIO_DIP_Switch_Cen_Mod_Read
  * @brief        ��ȡ������ģ��ѡ����ص�GPIO��
  * @param        *com_mod_phy_connected: ָ�򱣳�ͨ��ģ��ѡ���뿪��������GPIO��״̬��sen_mod_phy_connected�ṹ���ָ��
  * @retval       ������ģ��ѡ����ص�GPIO���Ƿ��ȡ�ɹ�  �ɹ��򷵻�0  ʧ���򷵻�1
	* @lastModify   2018/9/25  22:06
	* @author       JackWilliam
  */
uint8_t GPIO_DIP_Switch_Sen_Mod_Read(sen_mod_phy_connected * physical_connected_status){
	physical_connected_status->electric_meter_connected = Key_Scan(module_select_button_SEN_ElectricMeter_GPIO_Port, module_select_button_SEN_ElectricMeter_Pin);
	physical_connected_status->water_meter_connected = Key_Scan(module_select_button_SEN_WaterMeter_GPIO_Port, module_select_button_SEN_WaterMeter_Pin);
	physical_connected_status->gas_meter_connected = Key_Scan(module_select_button_SEN_GasMeter_GPIO_Port, module_select_button_SEN_GasMeter_Pin);
	
	return 0;
}

/* USER CODE END 2 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
