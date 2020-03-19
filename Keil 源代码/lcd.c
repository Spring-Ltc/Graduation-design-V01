

#include "lcd.h"

void write_com(uchar com);
void write_date(uchar DATA);



//дָ��
void write_com(uchar com)
{
	LCD_RS=0;
	P0=com;
	delay_ms(5);
	LCD_EN=1;
	delay_ms(5);
	LCD_EN=0;
}
//END of Function


//д����
void write_date(uchar DATA)   
{ 
	LCD_RS=1;
	P0=DATA;
	delay_ms(5);
	LCD_EN=1;
	delay_ms(5);
	LCD_EN=0;
}
//END of Function

//��ʼ��1602Һ��
void LCD_Init(void)
{
	LCD_EN=0;
	write_com(0x38);	//����16x2��ʾ��5x7����8λ���ݽӿ�
	write_com(0x0c);	//����ʾ������ʾ��ʾ���
	write_com(0x06);	//��ַָ��͹���Զ���1���������ƶ�
	write_com(0x01);	//��ʾ����������ָ������
}
//END of Function


//����Һ������ʾ����ʼλ�ã���x�е�y�У������Ͻ�����Ϊ [1,1]��
void SetPoint_xy(uchar x,uchar y)
{
	uchar point = 0x80;
	if(x == 2)
		point += 0x40;
	point += y-1;
	write_com(point);
}
//END of Function

/*
void desplay_char(unsigned char x,bit y,unsigned char p) //��ָ��λ����ʾһ���ַ���
{ 
	addr_x_y(x,y);
	write_date(p);
}
//END of Function




void xs_int(unsigned int shuju,bit t)   //��ʾһ������
{
	unsigned char huancun[6]={0};
	unsigned char biaozhi=0,i;
	if(shuju < 10)
		biaozhi = 1;
	else if(shuju < 100) 
		biaozhi = 2;
	else if(shuju < 1000) 
		biaozhi = 3;
	else if(shuju < 10000)
		biaozhi = 4;
	else if(shuju < 65535) 
		biaozhi = 5;
	switch(biaozhi)
	{
		case 5:
			huancun[5] = shuju/10000; 
		case 4:
			huancun[3] = shuju%10000/1000;
		case 3:
			huancun[2] = shuju%1000/100;
		case 2:
			huancun[1] = shuju%100/10;
		case 1:
			huancun[0] = shuju%10; break;
		default:break;
	}
	for(i=6;i>1;i--)
	{
		if(i==5)
			desplay_char(10,1,'.');
		else 
			desplay_char(15-i,t,0x30+huancun[i-1]); 
	} 
	desplay_char(15,t,'V'); 
}
//END of Function




*/































