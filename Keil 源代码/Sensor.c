


#include "Sensor.h"

#define CLK_pulse()   _nop_();_nop_();AD_CLK=1;_nop_();_nop_();AD_CLK=0



//��ȡ��ʪ�ȴ�����DHT11��ֵ���������������ȡ�����ش��������
uchar DHT11_GetValue(uchar *temp,uchar *humi)
{
	uchar value[5] ={0,0,0,0,0};
	uchar i,j,time=50;
	uchar onebit;
	
	DHT11 = 1;
	delay_ms(2);
	DHT11 = 0;
	delay_ms(30);	//��������Ҫ��ʱ18ms��������ʱ26.803ms��
	
	DHT11 = 1;
	delay_us(3);//ʵ����ʱ30us		//��������20--40us������Ӧ�źš�25��ʱ206us��10��ʱ86us��
	time = 50;
	while(DHT11 == 1)
	{
		if(!(--time))
			return 1;		//error1���������ߺ�ʱ��δ��⵽��Ӧ�ź�
	}
	
	while(DHT11 == 0);	//�ȴ��͵�ƽ����
	delay_us(3);
	if(DHT11 == 0)	
		return 2;		//error2��δ��⵽��Ӧ�źŵĸߵ�ƽ����
	
	time = 50;
	while(DHT11 == 1)	//�ȴ���Ӧ�źŽ���
	{
		if(!(--time))
			return 3;//error3����ʱ��δ��⵽bit��ʼ�ź�

	}
	
	//���濪ʼ��������
	for(i=0;i<5;i++)
	{
		for(j=0;j<8;j++)
		{
			value[i] <<= 1;
			while(DHT11 == 0);//�ȴ���ǰbit��ʼ�źŽ���
			delay_us(3);	//ʵ����ʱ30us	//26--28us��������0Ϊ�͵�ƽ������1Ϊ�ߵ�ƽ
			onebit = DHT11;
			value[i] += onebit;
			time = 50;
			while(DHT11 == 1);	//�ȴ��ź�1�ĸߵ�ƽ�������
			{
				if(!(--time))
					return 4;	//error4������bit�����쳣
			}
		}
	}
	DHT11 = 1;//���������ݣ������������ߣ���������һ��û�б�Ҫ��
	
	if(value[0]+value[1]+value[2]+value[3] != value[4])
		return 5;//error5������У�����
	
	*temp = value[2];
	*humi = value[0];
	return 0;	
}
//END of Function






//2020/03/15/15:37	��������

		//������DHT11��غ���
/*-----------------------------------------------------------------------------------------------�ָ���------------------------------------------------------------------------------*/
		//������MQ-2��غ���

//2020/03/14/14:12  ��������






//��ȡһ��ADת���Ľ������ڲ���Ϊͨ��ѡ��0��1
uint Get_AD_Value(uchar ch)
{
	uchar i;
	uint temp = 0;

	AD_CLK = 0,AD_DI = 1;	//��ʼ��������DI�Ա��ȡ״̬
	AD_CS = 0;	//ʹ��оƬ
	
	CLK_pulse();	//��ʼ�ź�
	
	AD_DI = 1;
	CLK_pulse();//��2��ʱ���������ù���ģʽΪ��ͨ��
	
	AD_DI = ch;
	CLK_pulse();//��3��ʱ������ѡ��ͨ��ch
	
	AD_DO = 1;//�������ȡ����
	for(i=0;i<8;++i)
	{
		CLK_pulse();
		temp <<=1;
		if(AD_DO == 1)
			temp |=0x01;
	}

	AD_CLK = 0;
	AD_CS = 1;	//����һ��ת��
	
	return temp;
}
//END of Function















