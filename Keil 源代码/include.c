

#include "include.h"


uchar temperature = 11;
uchar humidity = 11;
uchar smoke = 11;
uchar DangerType = 11;
uchar Angle = 11;
uchar DangerLevel = 0;



//�����ʱ����ms
void delay_ms(uint ms)
{
   uint x,y;
   for(x=ms;x>0;x--)
      for(y=110;y>0;y--);
}


//�����ʱ����us
//����delay_us()������
//	���ú�����ʱ6us����ڲ�����ЧֵΪ8����
//	����ڲ���Ϊx����ʵ����ʱ 6 + 8x  us
void delay_us(uint us)
{
	while(--us);
}
