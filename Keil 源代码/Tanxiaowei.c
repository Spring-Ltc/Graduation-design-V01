

/********************************����51�����ܹ������ּ��Ԥ��ϵͳ������˵����*******************************************/








/*-------------------------------------------------�ָ���-------------------------------------------------------------------------*/






#include "include.h"


#include "lcd.h"		//1602Һ����ʾ����ļ�
#include "Display.h"	//��ʾ��Ԫ������MAX7219��������ܣ�RGB�ƣ�������ѷ�����д�������桿
#include "Sensor.h"		//��������أ�����DHT11��ʪ�ȴ�������MQ-2����������ADת���������������жϣ�
#include "Motor.h"		//����������
#include "Usart.h"		//�����շ����

uchar code  str1[8] = "Temp: 11";
uchar code  str2[8] = "Humi: 22";
uchar code  str3[6] = "close";

void Display_Init(void);
uchar Judge(uchar tem,uchar hum,uchar smo);


void main()
{
	uchar state;	//����DHT11��������
	int color=1;	//������ɫ�仯ֵ

	Usart_Init();	//��ʼ������
	MAX7219_Init();	//��ʼ���������ʾ����
	LCD_Init();	//��ʼ��1602Һ����

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
		RGB_TurnOFF(color%3+1);	//��������LED��˸��ʾ
		
		
		state = DHT11_GetValue(&temperature,&humidity);	//�ɼ���ʪ��
		smoke = Get_AD_Value(0);	//�ɼ�����Ũ��
		smoke = smoke*100/256;
		
		DangerLevel = Judge(temperature,humidity,smoke);	//����Σ�յȼ�
		
		SetPoint_xy(1,7);
		write_date('0' + temperature/10);write_date('0' + temperature%10);		//������ʾ�¶�ֵ
		SetPoint_xy(2,7);
		write_date('0' + humidity/10);write_date('0' + humidity%10);		//������ʾʪ��ֵ

		
		Write7219(3,number[DangerLevel/10]);
		Write7219(4,number[DangerLevel%10]);		//��ʾΣ�յȼ�
		Write7219(7,number[smoke/10]);
		Write7219(8,number[smoke%10]);		//��ʾ����Ũ��ֵ
		
		SetPoint_xy(1,15);
		write_date('0' + DangerType/10);write_date('0' + DangerType%10);	//��ʱ��ʾΣ�����ͣ�������

		SendTheData();//����һ������
	}
}

//END of Function


//��ʾ��ʼ������ʾĿ¼�ͳ�ʼ״̬
void Display_Init(void)
{
	uchar i;
	SetPoint_xy(1,1);	//��ʾ�¶ȱ�ʶ
	for(i=0;i<8;i++)
	{
		write_date(str1[i]);
	}
	write_date(0xdf);write_date('C');	//��ʾ��λ
	
	
	
	SetPoint_xy(2,1);	//��ʾʪ�ȱ�ʶ
	for(i=0;i<8;i++)
	{
		write_date(str2[i]);
	}
	write_date('%');	//��ʾ��λ
	
	
	SetPoint_xy(2,12);	//��ʾ�����ܵ�����״̬
	for(i=0;i<5;i++)
	{
		write_date(str3[i]);
	}
}


//���봫�����ɼ���������Σ�յȼ�
uchar Judge(uchar tem,uchar hum,uchar smo)
{
	uchar  result = 0;
	uchar type1 = 0,type2 = 0,type3 = 0;
	
	result = tem*3 + hum*2 + smo*5 ;
	
	result /= 10;
	
	if(tem > 35)
		type1 = 1; //�¶ȹ���	ֻ�ܲɼ�0--50��C
	if(hum < 30)
		type2 = 3; //ʪ�ȹ���	ֻ�ܲɼ�20%--90%
	if(smo > 30)
		type3 = 5; //����Ũ��	ת����Χ0--100
	
	DangerType = type1 + type2 + type3 ;
	return result;
}











