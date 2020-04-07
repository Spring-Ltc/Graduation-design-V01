

/********************************基于51的智能工厂火灾监测预警系统软件设计说明书*******************************************/








/*-------------------------------------------------分割线-------------------------------------------------------------------------*/






#include "include.h"


#include "lcd.h"		//1602液晶显示相关文件
#include "Display.h"	//显示单元，包括MAX7219驱动数码管，RGB灯，【额外把蜂鸣器写在这里面】
#include "Sensor.h"		//传感器相关，包括DHT11温湿度传感器，MQ-2烟雾传感器（AD转换和数字量触发中断）
#include "Motor.h"		//步进电机相关
#include "Usart.h"		//串口收发相关

uchar code  str1[8] = "Temp: 11";
uchar code  str2[8] = "Humi: 22";
uchar code  str3[6] = "close";

void Display_Init(void);
uchar Judge(uchar tem,uchar hum,uchar smo);


uchar Temp_limit = 50;
uchar Humi_limit = 50;
uchar Smok_limit = 50;
uchar level_limit1 = 30;//危险等级阈值1，超过后LED闪烁警示，蜂鸣器间断性鸣叫
uchar level_limit2 = 40;//危险等级阈值2，超过后蜂鸣器LED闪烁警示，蜂鸣器长鸣
uchar receive_len =0;	//APP端配置参数接收的字符串长度

int color=1;	//定义颜色变化值

uchar SystemConfig(void);
void Smart_CarryOut(void);


void main()
{
	uchar state;	//定义DHT11错误类型
	uchar Config_tag = 1;//标记系统未完成初始化配置
	uchar i =0;		//用到的循环标记变量

	Usart_Init();	//初始化串口
	MAX7219_Init();	//初始化数码管显示驱动
	LCD_Init();	//初始化1602液晶屏
	Display_Init();

	FM_TurnON();
	delay_ms(300);
	FM_TurnOFF();			//系统上电启动提示音

	while(Config_tag)//先进行系统配置
	{
		//接收到APP远端配置系统命令
		if((receive_State & 0x80) == 0x80)//最高位置1，接收完一帧
		{
			Config_tag = SystemConfig();
			if(Config_tag)
			{
				SBUF = '0' + Config_tag;	//配置失败提示
				while(TI == 0);	//等待一个byte发送完成后，TI由硬件置1
				TI = 0;	//清除标志位，准备下一次发送
				
				SBUF = '+' ;	//配置失败提示
				while(TI == 0);	//等待一个byte发送完成后，TI由硬件置1
				TI = 0;	//清除标志位，准备下一次发送
				
				for(i=0;i<receive_len;i++)	//返回接收到的数据
				{
					SBUF = receive_buf[i];	
					while(TI == 0);	//等待一个byte发送完成后，TI由硬件置1
					TI = 0;	//清除标志位，准备下一次发送
				}
				
			}
			receive_State = 0;	//处理完数据，清除标志位，准备下一次接收
			ES = 1;
		}
		
	}
	ES = 0;		//已经配置成功，关闭串口接收中断

	while(1)
	{

		state = DHT11_GetValue(&temperature,&humidity);	//采集温湿度
		smoke = Get_AD_Value(0);	//采集烟雾浓度
		smoke = smoke*100/256;	//归一化处理
		
		DangerLevel = Judge(temperature,humidity,smoke);	//返回当前危险等级和类型
		
		SetPoint_xy(1,7);
		write_date('0' + temperature/10);write_date('0' + temperature%10);		//更新显示温度值
		SetPoint_xy(2,7);
		write_date('0' + humidity/10);write_date('0' + humidity%10);		//更新显示湿度值
		Write7219(7,number[smoke/10]);
		Write7219(8,number[smoke%10]);		//更新显示烟雾浓度值
		Write7219(3,number[DangerLevel/10]);
		Write7219(4,number[DangerLevel%10]);		//更新显示危险等级
		
		delay_ms(100);	//连续发送数据太快，蓝牙就会卡死//尝试1：提高通信波特率19200
		SendTheData();//发送一次数据
		
		Smart_CarryOut();//本地端决策处理

		
		SetPoint_xy(1,15);
		write_date('0' + DangerType/10);write_date('0' + DangerType%10);	//临时显示危险类型，调试用

		
	}
}

//END of Function


//显示初始化，显示目录和初始状态
void Display_Init(void)
{
	uchar i;
	SetPoint_xy(1,1);	//显示温度标识
	for(i=0;i<8;i++)
	{
		write_date(str1[i]);
	}
	write_date(0xdf);write_date('C');	//显示单位
	
	
	
	SetPoint_xy(2,1);	//显示湿度标识
	for(i=0;i<8;i++)
	{
		write_date(str2[i]);
	}
	write_date('%');	//显示单位
	
	
	SetPoint_xy(2,12);	//显示消防管道开合状态
	for(i=0;i<5;i++)
	{
		write_date(str3[i]);
	}
	
	Write7219(1,heng);Write7219(2,heng);Write7219(3,heng);Write7219(4,heng);
	Write7219(5,heng);Write7219(6,heng);Write7219(7,heng);Write7219(8,heng);
	
	
}
//StopFlag


//输入传感器采集结果，输出危险等级
uchar Judge(uchar tem,uchar hum,uchar smo)
{
	uchar  result = 0;
	uchar type1 = 0,type2 = 0,type3 = 0;

	if(tem > Temp_limit)
	{
		type1 = 1; //温度过高	只能采集0--50°C
		result += (tem - Temp_limit);
	}
	if(hum < Humi_limit)
	{
		type2 = 3; //湿度过低	只能采集20%--90%
		result += (Humi_limit - hum);
	}
	if(smo > Smok_limit)
	{
		type3 = 5; //烟雾浓度	转化后范围0--100
		result += (smo - Smok_limit);
	}
	
	DangerType = type1 + type2 + type3 ;
	return result;
}

//StopFlag


//APP远端配置系统参数
//参数：温度阈值、湿度阈值、浓度阈值、危险等级1、危险等级2
//配置成功则返回0，配置失败返回1
uchar SystemConfig(void)
{
	uchar i;
	receive_len = receive_State & 0x7f;//提取接收字符串长度
	
	i=0;
	Temp_limit =0;
	while((receive_buf[i] != ';') && (receive_len --))//提取第1个分号前的内容：温度阈值
	{
		Temp_limit *= 10;
		Temp_limit += receive_buf[i] - '0';
		i++;
	}
	if(Temp_limit>99)
		return 1;
	
	i++;	//跳过分号
	Humi_limit =0;
	while((receive_buf[i] != ';') && (receive_len --))//提取第2个分号前的内容：湿度阈值
	{
		Humi_limit *=10;
		Humi_limit += receive_buf[i] - '0';
		i++;
	}
	if(Humi_limit>99)
		return 2;
	
	i++;//跳过分号
	Smok_limit =0;
	while((receive_buf[i] != ';') && (receive_len --))//提取第3个分号前的内容：浓度阈值
	{
		Smok_limit *=10;
		Smok_limit += receive_buf[i] - '0';
		i++;
	}
	if(Smok_limit>99)
		return 3;
	
	i++;//跳过分号
	level_limit1 =0;
	while((receive_buf[i] != ';') && (receive_len --))//提取第4个分号前的内容：危险等级1阈值
	{
		level_limit1 *=10;
		level_limit1 += receive_buf[i] - '0';
		i++;
	}
	if(level_limit1>99)
		return 4;
	
	
	i++;//跳过分号
	level_limit2 =0;
	while((receive_buf[i] != ';') && (receive_len --))//提取第5个分号前的内容：危险等级2阈值
	{
		level_limit2 *=10;
		level_limit2 += receive_buf[i] - '0';
		i++;
	}
	if(level_limit2>99)
		return 5;
	

		FM_TurnON();
		delay_ms(300);
		FM_TurnOFF();		//数据设置成功提示音

	
	
	return 0;//返回配置成功结果
}
//StopFlag



//智能处理
void Smart_CarryOut(void)
{
	if(DangerLevel > level_limit2)//超过危险等级2
	{
		Motor_open();//打开水管灭火
		FM_TurnON();//蜂鸣器长鸣
		
		color++;
		RGB_TurnON(color%3+1);
		delay_ms(100);
		RGB_TurnOFF(color%3+1);	//LED闪烁提示
		delay_ms(50);
	}
	else if(DangerLevel > level_limit1)//超过危险等级1
	{
		Motor_close();//暂不灭火，仅警报
		
		color++;
		RGB_TurnON(color%3+1);
		FM_TurnON();
		delay_ms(100);
		RGB_TurnOFF(color%3+1);	//LED闪烁提示
		FM_TurnOFF();
		delay_ms(50);
	}
	else//安全
	{
		Motor_close();//关闭水管
		FM_TurnOFF();//关闭蜂鸣器
		RGB_TurnON(green);//绿色常量表示安全
		RGB_TurnOFF(red);
		RGB_TurnOFF(blue);	//关闭另外两种颜色灯光
	}
}
//StopFlag



