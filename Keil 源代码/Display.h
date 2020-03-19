

//	2020/03/13/18：05			该部分代码(Display相关代码)测试完全正常



/*---------------------------------相关说明----------------------------------------------


显示单元，包括MAX7219驱动数码管，RGB灯，【额外把蜂鸣器写在这里面】



*/////////////////////////////////////////////////////////////////////////////////////////
//


#ifndef __Display_H__
#define __Display_H__


#include "include.h"


//MAX7219内部控制寄存器地址定义
#define Mode 0x09		//解码模式
#define Brightness 0x0a		//亮度
#define Size 0x0b		//数码管个数
#define Down 0x0c		//关断模式
#define Text 0x0f		//测试控制

//RGB颜色宏定义
#define red 1
#define green 2
#define blue 3



														
/*------------------------------------------------自定义功能函数声明-----------------------------------------------------------------------------*/

void Write7219(uint Adress,uint Date);	//向7219写入内容，包含地址和数据		
void MAX7219_Init(void);	//初始化两块7219芯片
void FM_TurnON(void);	//蜂鸣器打开
void FM_TurnOFF(void);	//蜂鸣器关闭

void RGB_TurnON(uint color);
void RGB_TurnOFF(uint color);

#endif


