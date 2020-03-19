

/********************************基于51的智能工厂火灾监测预警系统软件设计说明书*******************************************/








/*-------------------------------------------------分割线-------------------------------------------------------------------------*/






#include "include.h"


#include "lcd.h"		//1602液晶显示相关文件
#include "Display.h"	//显示单元，包括MAX7219驱动数码管，RGB灯，【额外把蜂鸣器写在这里面】
#include "Sensor.h"		//传感器相关，包括DHT11温湿度传感器，MQ-2烟雾传感器（AD转换和数字量触发中断）


#include "Motor.h"		//步进电机相关
#include "Usart.h"		//串口收发相关

uchar  str[11] = "Spring_Text";

//uint tab[4] = {0xfe,0xfd,0xfb,0xf7};	//每次只有一个管脚输出低电平，电机转动




void main()
{
	uchar temperature=1,humidity=1;		//定义温湿度采集值
	uchar state;	//定义DHT11错误类型
	int color=1;	//定义颜色变化值
	uint AD_value;	//定义烟雾浓度传感器的模拟值
	
	
	uint i;
	
	MAX7219_Init();
	LCD_Init();

	//测试lcd液晶
	SetPoint_xy(1,2);
	for(i=0;i<11;i++)
	{
		write_date(str[i]);
	}
	
	
	
	FM_TurnON();
	delay_ms(300);
	FM_TurnOFF();
	
	FirePipes_Control(90);delay_ms(1500);
	FirePipes_Control(0);delay_ms(1500);
	FirePipes_Control(90);delay_ms(1500);
	FirePipes_Control(0);delay_ms(1500);
	
	while(1)
	{
		color++;
		RGB_TurnON(color%3+1);
		delay_ms(600);
		RGB_TurnOFF(color%3+1);	//正常运行LED闪烁提示
		
		FirePipes_Control(45);
		
		
		state = DHT11_GetValue(&temperature,&humidity);	//采集温湿度
		AD_value = Get_AD_Value(0);	//采集烟雾浓度
		
		Write7219(1,number[temperature/10]);
		Write7219(2,number[temperature%10]);
		Write7219(7,number[humidity/10]);
		Write7219(8,number[humidity%10]);

		SetPoint_xy(2,2);
		write_date('0' + AD_value/100);
		write_date('0' + AD_value/10%10);
		write_date('0' + AD_value%10);
		
	}
}

//END of Function





















