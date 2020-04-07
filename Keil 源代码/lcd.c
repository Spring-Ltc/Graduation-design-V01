

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





























