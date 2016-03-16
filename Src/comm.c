//�����51822 ����ͨѶЭ��
#include "main.h"
extern uint8_t buffer[];
//------------------------------------------------------
//����У���
uint8_t check_sum(uint8_t* data,uint8_t len)
{
	uint8_t temp=0;
	for(int i=0;i<len;i++)
		temp+=data[i];
	return temp;
}

//��������
//================================================================
//����ȷ������
//fe 05 01 01 ab b0 ��fe 05 01 01 ae b3
//flag=1 ���ͳɹ�ȷ�ϣ�0-����ʧ��ȷ��
void send_shakehand(uint8_t flag)
{
	buffer[0]=0xfe;
	buffer[1]=0x05;
	buffer[2]=0x01;//command
	buffer[3]=0x01;
	if(flag==1)
	{
		buffer[4]=0xab;
		buffer[5]=0xb0;
	}
	else
	{
		buffer[4]=0xae;
		buffer[5]=0xb3;
	}
	uart_send(buffer,6);
}
//================================================================
//����ȷ��
void send_confirm()
{
}
//===========================================================
//���͵����Ϣ
void send_batt_info()
{
}
//======================================================
//���Ͳ���+����
void send_sensor_data()
{
}

//===============================================================
//--------------------------------------
//������������
uint16_t get_len(uint8_t *data)
{
	if(data[2]==0xaa && (data[5]==check_sum(data,5)) )
	{
		return data[4];
	}
	else
		return 0xff;
}
//-----------------------------------------------------
//������յ�������
//return:1-�յ�������ȷ��0-�յ��������
void rece_dispatch(uint8_t *data)
{
	uint8_t len=data[0];
	if(data[1]!=0xfe || data[2]!=0x04 || (data[len]!=check_sum(&data[1],len-1)) )
	{
		//�յ���������ظ�
		send_shakehand(0);
	}
	//��������
	switch(data[3])
	{
		case 0x01://ȷ������
			break;
		case 0x02://��ʼ��flash�洢��
			flash_init();
			break;
		case 0x03://�յ�ʱ����������
			RTC_Set_datetime(&data[5]);
			RTC_AlarmConfig(data[8],data[9]+1);
			SEGGER_RTT_printf(0,"set_time=%02d-%02d-%02d;%02d-%02d-%02d;\r\n",data[5],data[6],data[7],data[8],data[9],data[10]);
			break;
		default:
			send_shakehand(0);//�յ��쳣����S
			return;
	};
	if(data[3]!=0x01)
	{
		//�ظ�ȷ���յ�
		send_shakehand(1);
	}
}