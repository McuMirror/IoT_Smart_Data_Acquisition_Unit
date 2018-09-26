#include "sensor_electric_meter.h"

sen_electric_meter_gen_para sensor_electric_meter_general_parameters = {
	.baudrate             =     2400,             //ȱʡĬ�ϲ�����Ϊ2400
	.amount               =     1,                //ȱʡĬ�ϵ������Ϊ1
	.ID_single_length     =     6,                //ȱʡĬ�ϵ������ID����Ϊ6
	.ID_total_length      =     6,                //ȱʡĬ�ϵ��ID�ܳ���Ϊ6
};


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
	
	return 0;
}
