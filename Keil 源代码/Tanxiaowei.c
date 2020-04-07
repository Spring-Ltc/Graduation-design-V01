

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


uchar Temp_limit = 50;
uchar Humi_limit = 50;
uchar Smok_limit = 50;
uchar level_limit1 = 30;//Σ�յȼ���ֵ1��������LED��˸��ʾ�����������������
uchar level_limit2 = 40;//Σ�յȼ���ֵ2�������������LED��˸��ʾ������������
uchar receive_len =0;	//APP�����ò������յ��ַ�������

int color=1;	//������ɫ�仯ֵ

uchar SystemConfig(void);
void Smart_CarryOut(void);


void main()
{
	uchar state;	//����DHT11��������
	uchar Config_tag = 1;//���ϵͳδ��ɳ�ʼ������
	uchar i =0;		//�õ���ѭ����Ǳ���

	Usart_Init();	//��ʼ������
	MAX7219_Init();	//��ʼ���������ʾ����
	LCD_Init();	//��ʼ��1602Һ����
	Display_Init();

	FM_TurnON();
	delay_ms(300);
	FM_TurnOFF();			//ϵͳ�ϵ�������ʾ��

	while(Config_tag)//�Ƚ���ϵͳ����
	{
		//���յ�APPԶ������ϵͳ����
		if((receive_State & 0x80) == 0x80)//���λ��1��������һ֡
		{
			Config_tag = SystemConfig();
			if(Config_tag)
			{
				SBUF = '0' + Config_tag;	//����ʧ����ʾ
				while(TI == 0);	//�ȴ�һ��byte������ɺ�TI��Ӳ����1
				TI = 0;	//�����־λ��׼����һ�η���
				
				SBUF = '+' ;	//����ʧ����ʾ
				while(TI == 0);	//�ȴ�һ��byte������ɺ�TI��Ӳ����1
				TI = 0;	//�����־λ��׼����һ�η���
				
				for(i=0;i<receive_len;i++)	//���ؽ��յ�������
				{
					SBUF = receive_buf[i];	
					while(TI == 0);	//�ȴ�һ��byte������ɺ�TI��Ӳ����1
					TI = 0;	//�����־λ��׼����һ�η���
				}
				
			}
			receive_State = 0;	//���������ݣ������־λ��׼����һ�ν���
			ES = 1;
		}
		
	}
	ES = 0;		//�Ѿ����óɹ����رմ��ڽ����ж�

	while(1)
	{

		state = DHT11_GetValue(&temperature,&humidity);	//�ɼ���ʪ��
		smoke = Get_AD_Value(0);	//�ɼ�����Ũ��
		smoke = smoke*100/256;	//��һ������
		
		DangerLevel = Judge(temperature,humidity,smoke);	//���ص�ǰΣ�յȼ�������
		
		SetPoint_xy(1,7);
		write_date('0' + temperature/10);write_date('0' + temperature%10);		//������ʾ�¶�ֵ
		SetPoint_xy(2,7);
		write_date('0' + humidity/10);write_date('0' + humidity%10);		//������ʾʪ��ֵ
		Write7219(7,number[smoke/10]);
		Write7219(8,number[smoke%10]);		//������ʾ����Ũ��ֵ
		Write7219(3,number[DangerLevel/10]);
		Write7219(4,number[DangerLevel%10]);		//������ʾΣ�յȼ�
		
		delay_ms(100);	//������������̫�죬�����ͻῨ��//����1�����ͨ�Ų�����19200
		SendTheData();//����һ������
		
		Smart_CarryOut();//���ض˾��ߴ���

		
		SetPoint_xy(1,15);
		write_date('0' + DangerType/10);write_date('0' + DangerType%10);	//��ʱ��ʾΣ�����ͣ�������

		
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
	
	Write7219(1,heng);Write7219(2,heng);Write7219(3,heng);Write7219(4,heng);
	Write7219(5,heng);Write7219(6,heng);Write7219(7,heng);Write7219(8,heng);
	
	
}
//StopFlag


//���봫�����ɼ���������Σ�յȼ�
uchar Judge(uchar tem,uchar hum,uchar smo)
{
	uchar  result = 0;
	uchar type1 = 0,type2 = 0,type3 = 0;

	if(tem > Temp_limit)
	{
		type1 = 1; //�¶ȹ���	ֻ�ܲɼ�0--50��C
		result += (tem - Temp_limit);
	}
	if(hum < Humi_limit)
	{
		type2 = 3; //ʪ�ȹ���	ֻ�ܲɼ�20%--90%
		result += (Humi_limit - hum);
	}
	if(smo > Smok_limit)
	{
		type3 = 5; //����Ũ��	ת����Χ0--100
		result += (smo - Smok_limit);
	}
	
	DangerType = type1 + type2 + type3 ;
	return result;
}

//StopFlag


//APPԶ������ϵͳ����
//�������¶���ֵ��ʪ����ֵ��Ũ����ֵ��Σ�յȼ�1��Σ�յȼ�2
//���óɹ��򷵻�0������ʧ�ܷ���1
uchar SystemConfig(void)
{
	uchar i;
	receive_len = receive_State & 0x7f;//��ȡ�����ַ�������
	
	i=0;
	Temp_limit =0;
	while((receive_buf[i] != ';') && (receive_len --))//��ȡ��1���ֺ�ǰ�����ݣ��¶���ֵ
	{
		Temp_limit *= 10;
		Temp_limit += receive_buf[i] - '0';
		i++;
	}
	if(Temp_limit>99)
		return 1;
	
	i++;	//�����ֺ�
	Humi_limit =0;
	while((receive_buf[i] != ';') && (receive_len --))//��ȡ��2���ֺ�ǰ�����ݣ�ʪ����ֵ
	{
		Humi_limit *=10;
		Humi_limit += receive_buf[i] - '0';
		i++;
	}
	if(Humi_limit>99)
		return 2;
	
	i++;//�����ֺ�
	Smok_limit =0;
	while((receive_buf[i] != ';') && (receive_len --))//��ȡ��3���ֺ�ǰ�����ݣ�Ũ����ֵ
	{
		Smok_limit *=10;
		Smok_limit += receive_buf[i] - '0';
		i++;
	}
	if(Smok_limit>99)
		return 3;
	
	i++;//�����ֺ�
	level_limit1 =0;
	while((receive_buf[i] != ';') && (receive_len --))//��ȡ��4���ֺ�ǰ�����ݣ�Σ�յȼ�1��ֵ
	{
		level_limit1 *=10;
		level_limit1 += receive_buf[i] - '0';
		i++;
	}
	if(level_limit1>99)
		return 4;
	
	
	i++;//�����ֺ�
	level_limit2 =0;
	while((receive_buf[i] != ';') && (receive_len --))//��ȡ��5���ֺ�ǰ�����ݣ�Σ�յȼ�2��ֵ
	{
		level_limit2 *=10;
		level_limit2 += receive_buf[i] - '0';
		i++;
	}
	if(level_limit2>99)
		return 5;
	

		FM_TurnON();
		delay_ms(300);
		FM_TurnOFF();		//�������óɹ���ʾ��

	
	
	return 0;//�������óɹ����
}
//StopFlag



//���ܴ���
void Smart_CarryOut(void)
{
	if(DangerLevel > level_limit2)//����Σ�յȼ�2
	{
		Motor_open();//��ˮ�����
		FM_TurnON();//����������
		
		color++;
		RGB_TurnON(color%3+1);
		delay_ms(100);
		RGB_TurnOFF(color%3+1);	//LED��˸��ʾ
		delay_ms(50);
	}
	else if(DangerLevel > level_limit1)//����Σ�յȼ�1
	{
		Motor_close();//�ݲ���𣬽�����
		
		color++;
		RGB_TurnON(color%3+1);
		FM_TurnON();
		delay_ms(100);
		RGB_TurnOFF(color%3+1);	//LED��˸��ʾ
		FM_TurnOFF();
		delay_ms(50);
	}
	else//��ȫ
	{
		Motor_close();//�ر�ˮ��
		FM_TurnOFF();//�رշ�����
		RGB_TurnON(green);//��ɫ������ʾ��ȫ
		RGB_TurnOFF(red);
		RGB_TurnOFF(blue);	//�ر�����������ɫ�ƹ�
	}
}
//StopFlag



