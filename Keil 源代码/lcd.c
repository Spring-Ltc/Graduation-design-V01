

#include "lcd.h"

void write_com(uchar com);
void write_date(uchar DATA);



//写指令
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


//写数据
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


//初始化1602液晶
void LCD_Init(void)
{
	LCD_EN=0;
	write_com(0x38);	//设置16x2显示，5x7点阵，8位数据接口
	write_com(0x0c);	//开显示，不显示显示光标
	write_com(0x06);	//地址指针和光标自动加1，整屏不移动
	write_com(0x01);	//显示清屏，数据指针清零
}
//END of Function


//设置液晶屏显示的起始位置（第x行第y列，及左上角坐标为 [1,1]）
void SetPoint_xy(uchar x,uchar y)
{
	uchar point = 0x80;
	if(x == 2)
		point += 0x40;
	point += y-1;
	write_com(point);
}
//END of Function





























