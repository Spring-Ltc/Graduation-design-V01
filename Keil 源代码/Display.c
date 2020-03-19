

#include "Display.h"

uchar code number[10]={ 0x7e,0x30,0x6d,0x79,0x33,
												0x5b,0x5f,0x70,0x7f,0x7b};  //���ֱ���0��9


//��7219д������,������ַ������
void Write7219 (uint Adress,uint Date)
{
   uchar i;
   uint adr=Adress,dat=Date;
   CS=1;
   
   for(i=8;i>=1;i--)	//���͵�ַ
   {
      CLK=0;
      DIN=(bit)(adr&0x80);
      //DIN=adr;
      adr=adr<<1;
      CLK=1;
   }
   for(i=8;i>=1;i--)	//��������
   {
      CLK=0;
      DIN=(bit)(dat&0x80);
      //DIN=dat;
      dat=dat<<1;
      CLK=1;
   }
   CS=0;
   CS=1;
}
//END of Function


//��ʼ��7219оƬ
void MAX7219_Init(void)
{
   Write7219 (Mode,0x00);	//ѡ�� No Code 
   Write7219 (Brightness,0x07);	//���ȵ���
   Write7219 (Size,0x07);	//�����λ����8λȫѡ
   Write7219 (Down,0x01);	//��������ģʽ0x01��ʡ��ģʽΪ0x00��
   Write7219 (Text,0x00);	//��ʾ����
}
//END of Function


//��������
void FM_TurnON(void)
{
	BUZZ = 0;
}
//END of Function


//�������ر�
void FM_TurnOFF(void)
{
	BUZZ = 1;
}
//END of Function


void RGB_TurnON(uint color)
{
	switch (color)
	{
		case red:
			RGB_R = 0;break;
		case green:
			RGB_G = 0;break;
		case blue:
			RGB_B = 0;break;
	}
}
//END of Function


void RGB_TurnOFF(uint color)
{
	switch (color)
	{
		case red:
			RGB_R = 1;break;
		case green:
			RGB_G = 1;break;
		case blue:
			RGB_B = 1;break;
	}
}
//END of Function























