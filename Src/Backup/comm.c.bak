//处理和51822 数据通讯协议
#include "main.h"
extern uint8_t buffer[];
//------------------------------------------------------
//计算校验和
uint8_t check_sum(uint8_t* data,uint8_t len)
{
	uint8_t temp=0;
	for(int i=0;i<len;i++)
		temp+=data[i];
	return temp;
}

//发送命令
//================================================================
//发送确认命令
//fe 05 01 01 ab b0 ；fe 05 01 01 ae b3
//flag=1 发送成功确认，0-发送失败确认
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
//命令确认
void send_confirm()
{
}
//===========================================================
//发送电池信息
void send_batt_info()
{
}
//======================================================
//发送步数+心率
void send_sensor_data()
{
}

//===============================================================
//--------------------------------------
//解析长度命令
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
//处理接收到的数据
//return:1-收到命令正确；0-收到命令错误
void rece_dispatch(uint8_t *data)
{
	uint8_t len=data[0];
	if(data[1]!=0xfe || data[2]!=0x04 || (data[len]!=check_sum(&data[1],len-1)) )
	{
		//收到错误命令回复
		send_shakehand(0);
	}
	//解析命令
	switch(data[3])
	{
		case 0x01://确认命令
			break;
		case 0x02://初始化flash存储区
			flash_init();
			break;
		case 0x03://收到时间设置命令
			RTC_Set_datetime(&data[5]);
			RTC_AlarmConfig(data[8],data[9]+1);
			SEGGER_RTT_printf(0,"set_time=%02d-%02d-%02d;%02d-%02d-%02d;\r\n",data[5],data[6],data[7],data[8],data[9],data[10]);
			break;
		default:
			send_shakehand(0);//收到异常命令S
			return;
	};
	if(data[3]!=0x01)
	{
		//回复确认收到
		send_shakehand(1);
	}
}