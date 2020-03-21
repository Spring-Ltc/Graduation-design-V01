

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


void main()
{
	uchar state;	//定义DHT11错误类型
	int color=1;	//定义颜色变化值

	Usart_Init();	//初始化串口
	MAX7219_Init();	//初始化数码管显示驱动
	LCD_Init();	//初始化1602液晶屏

	Display_Init();
	
	
	
	FM_TurnON();
	delay_ms(300);
	FM_TurnOFF();
	
	FirePipes_Control(45);
	
	while(1)
	{
		color++;
		RGB_TurnON(color%3+1);
		delay_ms(600);
		RGB_TurnOFF(color%3+1);	//正常运行LED闪烁提示
		
		
		state = DHT11_GetValue(&temperature,&humidity);	//采集温湿度
		smoke = Get_AD_Value(0);	//采集烟雾浓度
		smoke = smoke*100/256;
		
		DangerLevel = Judge(temperature,humidity,smoke);	//返回危险等级
		
		SetPoint_xy(1,7);
		write_date('0' + temperature/10);write_date('0' + temperature%10);		//更新显示温度值
		SetPoint_xy(2,7);
		write_date('0' + humidity/10);write_date('0' + humidity%10);		//更新显示湿度值

		
		Write7219(3,number[DangerLevel/10]);
		Write7219(4,number[DangerLevel%10]);		//显示危险等级
		Write7219(7,number[smoke/10]);
		Write7219(8,number[smoke%10]);		//显示烟雾浓度值
		
		SetPoint_xy(1,15);
		write_date('0' + DangerType/10);write_date('0' + DangerType%10);	//临时显示危险类型，调试用

		SendTheData();//发送一次数据
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
}


//输入传感器采集结果，输出危险等级
uchar Judge(uchar tem,uchar hum,uchar smo)
{
	uchar  result = 0;
	uchar type1 = 0,type2 = 0,type3 = 0;
	
	result = tem*3 + hum*2 + smo*5 ;
	
	result /= 10;
	
	if(tem > 35)
		type1 = 1; //温度过高	只能采集0--50°C
	if(hum < 30)
		type2 = 3; //湿度过低	只能采集20%--90%
	if(smo > 30)
		type3 = 5; //烟雾浓度	转化后范围0--100
	
	DangerType = type1 + type2 + type3 ;
	return result;
}











