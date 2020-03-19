

/********************************����51�����ܹ������ּ��Ԥ��ϵͳ������˵����*******************************************/








/*-------------------------------------------------�ָ���-------------------------------------------------------------------------*/






#include "include.h"


#include "lcd.h"		//1602Һ����ʾ����ļ�
#include "Display.h"	//��ʾ��Ԫ������MAX7219��������ܣ�RGB�ƣ�������ѷ�����д�������桿
#include "Sensor.h"		//��������أ�����DHT11��ʪ�ȴ�������MQ-2����������ADת���������������жϣ�


#include "Motor.h"		//����������
#include "Usart.h"		//�����շ����

uchar  str[11] = "Spring_Text";

//uint tab[4] = {0xfe,0xfd,0xfb,0xf7};	//ÿ��ֻ��һ���ܽ�����͵�ƽ�����ת��




void main()
{
	uchar temperature=1,humidity=1;		//������ʪ�Ȳɼ�ֵ
	uchar state;	//����DHT11��������
	int color=1;	//������ɫ�仯ֵ
	uint AD_value;	//��������Ũ�ȴ�������ģ��ֵ
	
	
	uint i;
	
	MAX7219_Init();
	LCD_Init();

	//����lcdҺ��
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
		RGB_TurnOFF(color%3+1);	//��������LED��˸��ʾ
		
		FirePipes_Control(45);
		
		
		state = DHT11_GetValue(&temperature,&humidity);	//�ɼ���ʪ��
		AD_value = Get_AD_Value(0);	//�ɼ�����Ũ��
		
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





















