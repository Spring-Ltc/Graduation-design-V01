

#include "Display.h"

uchar code number[10]={ 0x7e,0x30,0x6d,0x79,0x33,
												0x5b,0x5f,0x70,0x7f,0x7b};  //数字编码0到9


//向7219写入内容,包含地址和数据
void Write7219 (uint Adress,uint Date)
{
   uchar i;
   uint adr=Adress,dat=Date;
   CS=1;
   
   for(i=8;i>=1;i--)	//发送地址
   {
      CLK=0;
      DIN=(bit)(adr&0x80);
      //DIN=adr;
      adr=adr<<1;
      CLK=1;
   }
   for(i=8;i>=1;i--)	//发送数据
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


//初始化7219芯片
void MAX7219_Init(void)
{
   Write7219 (Mode,0x00);	//选择 No Code 
   Write7219 (Brightness,0x07);	//亮度调节
   Write7219 (Size,0x07);	//数码管位数，8位全选
   Write7219 (Down,0x01);	//正常工作模式0x01（省电模式为0x00）
   Write7219 (Text,0x00);	//显示测试
}
//END of Function


//蜂鸣器打开
void FM_TurnON(void)
{
	BUZZ = 0;
}
//END of Function


//蜂鸣器关闭
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























