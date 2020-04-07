

#include "Motor.h"

#define Motor_Step  120		//步进电机脉冲数，控制开关角度
#define speed 5			//控制电机转速

uchar tab[4] = {0xfe,0xfd,0xfb,0xf7};	//每次只有一个管脚输出低电平，电机转动

void Motor_open(void)
{
	uchar i =0;
	uchar step;
	step = Motor_Step;
	if(Motor_State > 5)//本来就是打开的
		return;
	else
	{
		while(--step)
		{
			for(i=0;i<4;i++)
			{
				P1 &= tab[i];
				delay_ms(speed);
				P1 |= 0x0f;
			}
		}
		Motor_State = 7;	
	}
}
//END of Function


void Motor_close(void)
{
	uchar i =0;
	uchar step;
	step = Motor_Step;
	if(Motor_State < 5)//本来就是关闭的
		return;
	else
	{
		while(--step)
		{
			for(i=4;i>0;i--)
			{
				P1 &= tab[i-1];
				delay_ms(speed);
				P1 |= 0x0f;
			}
		}
		Motor_State = 3;
	}
}
//END of Function




