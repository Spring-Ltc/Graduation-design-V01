


#include "Sensor.h"

#define CLK_pulse()   _nop_();_nop_();AD_CLK=1;_nop_();_nop_();AD_CLK=0



//获取温湿度传感器DHT11的值，如果不能正常读取，返回错误的类型
uchar DHT11_GetValue(uchar *temp,uchar *humi)
{
	uchar value[5] ={0,0,0,0,0};
	uchar i,j,time=50;
	uchar onebit;
	
	DHT11 = 1;
	delay_ms(2);
	DHT11 = 0;
	delay_ms(30);	//这里至少要延时18ms【仿真延时26.803ms】
	
	DHT11 = 1;
	delay_us(3);//实际延时30us		//主机拉高20--40us后检测响应信号【25延时206us；10延时86us】
	time = 50;
	while(DHT11 == 1)
	{
		if(!(--time))
			return 1;		//error1：主机拉高后长时间未检测到响应信号
	}
	
	while(DHT11 == 0);	//等待低电平结束
	delay_us(3);
	if(DHT11 == 0)	
		return 2;		//error2：未检测到响应信号的高电平部分
	
	time = 50;
	while(DHT11 == 1)	//等待响应信号结束
	{
		if(!(--time))
			return 3;//error3：长时间未检测到bit起始信号

	}
	
	//下面开始接受数据
	for(i=0;i<5;i++)
	{
		for(j=0;j<8;j++)
		{
			value[i] <<= 1;
			while(DHT11 == 0);//等待当前bit起始信号结束
			delay_us(3);	//实际延时30us	//26--28us过后，数据0为低电平，数据1为高电平
			onebit = DHT11;
			value[i] += onebit;
			time = 50;
			while(DHT11 == 1);	//等待信号1的高电平传输完成
			{
				if(!(--time))
					return 4;	//error4：接收bit数据异常
			}
		}
	}
	DHT11 = 1;//接收完数据，将数据线拉高（理论上这一步没有必要）
	
	if(value[0]+value[1]+value[2]+value[3] != value[4])
		return 5;//error5：数据校验错误
	
	*temp = value[2];
	*humi = value[0];
	return 0;	
}
//END of Function






//2020/03/15/15:37	测试正常

		//上面是DHT11相关函数
/*-----------------------------------------------------------------------------------------------分割线------------------------------------------------------------------------------*/
		//下面是MQ-2相关函数

//2020/03/14/14:12  测试正常






//获取一次AD转换的结果，入口参数为通道选择，0或1
uint Get_AD_Value(uchar ch)
{
	uchar i;
	uint temp = 0;

	AD_CLK = 0,AD_DI = 1;	//初始化，上拉DI以便读取状态
	AD_CS = 0;	//使能芯片
	
	CLK_pulse();	//起始信号
	
	AD_DI = 1;
	CLK_pulse();//第2个时钟脉冲设置工作模式为单通道
	
	AD_DI = ch;
	CLK_pulse();//第3个时钟脉冲选择通道ch
	
	AD_DO = 1;//上拉后读取数据
	for(i=0;i<8;++i)
	{
		CLK_pulse();
		temp <<=1;
		if(AD_DO == 1)
			temp |=0x01;
	}

	AD_CLK = 0;
	AD_CS = 1;	//结束一次转换
	
	return temp;
}
//END of Function















