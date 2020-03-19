

#include "Motor.h"


uchar tab[4] = {0xfe,0xfd,0xfb,0xf7};	//每次只有一个管脚输出低电平，电机转动


//控制消防管道开合程度
//入口参数角度（范围0---90°，这里用旋转的转数模拟角度）
//0°表示闭合，90°表示全开
void FirePipes_Control(uchar angle)
{
	static uchar State = 0;	//静态局部变量，用于保存管道开合状态
	uchar angleChange = 0;	//需要的角度变化量
	uint i;	//计数变量
	uchar speed = 5;
	
	if(angle == State || angle > 90 || angle < 0)
		return;	//本来就是这个状态或超出范围，忽略该次控制指令
	
	if(angle > State)//开大,正向转动指定圈数
	{
		angleChange = angle-State;
		//angleChange /=22;
		while(--angleChange)
		{
			for(i=0;i<4;i++)
			{
				P1 &= tab[i];
				delay_ms(speed);
				P1 |= 0x0f;
			}
		}
	}
	else	//关小，反向转动指定圈速
	{
		angleChange = State-angle;
		//angleChange /=22;
		while(--angleChange)
		{
			for(i=4;i>0;i--)
			{
				P1 &= tab[i-1];
				delay_ms(speed);
				P1 |= 0x0f;
			}
		}
	}
	State = angle;//保存状态
}
//END of Function









