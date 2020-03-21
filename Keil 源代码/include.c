

#include "include.h"


uchar temperature = 11;
uchar humidity = 11;
uchar smoke = 11;
uchar DangerType = 11;
uchar Angle = 11;
uchar DangerLevel = 0;



//软件延时函数ms
void delay_ms(uint ms)
{
   uint x,y;
   for(x=ms;x>0;x--)
      for(y=110;y>0;y--);
}


//软件延时函数us
//调用delay_us()函数：
//	调用函数延时6us，入口参数有效值为8倍；
//	若入口参数为x，则实际延时 6 + 8x  us
void delay_us(uint us)
{
	while(--us);
}
