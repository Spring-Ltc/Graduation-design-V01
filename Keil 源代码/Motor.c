

#include "Motor.h"


uchar tab[4] = {0xfe,0xfd,0xfb,0xf7};	//ÿ��ֻ��һ���ܽ�����͵�ƽ�����ת��


//���������ܵ����ϳ̶�
//��ڲ����Ƕȣ���Χ0---90�㣬��������ת��ת��ģ��Ƕȣ�
//0���ʾ�պϣ�90���ʾȫ��
void FirePipes_Control(uchar angle)
{
	static uchar State = 0;	//��̬�ֲ����������ڱ���ܵ�����״̬
	uchar angleChange = 0;	//��Ҫ�ĽǶȱ仯��
	uint i;	//��������
	uchar speed = 5;
	
	if(angle == State || angle > 90 || angle < 0)
		return;	//�����������״̬�򳬳���Χ�����Ըôο���ָ��
	
	if(angle > State)//����,����ת��ָ��Ȧ��
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
	else	//��С������ת��ָ��Ȧ��
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
	State = angle;//����״̬
}
//END of Function









