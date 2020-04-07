

#include "Motor.h"

#define Motor_Step  120		//������������������ƿ��ؽǶ�
#define speed 5			//���Ƶ��ת��

uchar tab[4] = {0xfe,0xfd,0xfb,0xf7};	//ÿ��ֻ��һ���ܽ�����͵�ƽ�����ת��

void Motor_open(void)
{
	uchar i =0;
	uchar step;
	step = Motor_Step;
	if(Motor_State > 5)//�������Ǵ򿪵�
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
	if(Motor_State < 5)//�������ǹرյ�
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




