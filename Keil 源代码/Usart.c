


#include "Usart.h"

//串口初始化设置
void Usart_Init(void)
{
	SCON = 0x50;	//串口工作方式1
	TMOD = 0x20;		//定时器1工作方式2，8位自动重装初值
	TH1 = 0xfd;
	TL1 = 0xfd;
	ES = 1;		//允许串口接收中断
	TR1 = 1;	//定时器1开始计数，产生波特率时钟
	EA = 1;//开总中断

}





//把数据转换成字符串，并指定长度，不足前面补零,超出只取后面指定位数
//不能转换0
//最大只能转换20位
//date:输入的数据	；	Str_out：输出字符串首地址;	len:指定输出长度
void ChangeString(uchar date,char *Str_out,uchar len)
{
	uchar i=0;
	char string[20] = {0};//缓存转化的字符串

	for(i=0;i<20;i++)
	{
		string[i] = '0' + date%10;	//先存储最低位
		date /=10;
	}

	for(i=0;i<len;i++)
	{
		*(Str_out+i) = string[len-i-1];
	}
}
//StopFlag



//发送一个数据帧出去
void SendTheData(void)
{
	char str[16] = {0};	//整个数据帧
	
	char str1[5] = "smart";	//起始验证位
	char str2[2] = {0};	//缓存温度值的字符串
	char str3[2] = {0};	//缓存湿度值的字符串
	char str4[2] = {0};	//缓存烟雾浓度值的字符串
	char str5[1] = {0};	//危险类型
	char str6[2] = {0};//消防管道开合角度
	
	char str7[2] = "ok";	//结束标志位
	
	uchar i;	//发送计数变量

	
	//将校验标识以及数据转换成字符添加到字符串里面
	strcpy(str,str1);	//添加起始位，5位，用于验证用户0+5=5
	
	ChangeString(temperature,str2,2);
	strncat(str,str2,2);	//在后面拼接温度值5+2=7
	
	ChangeString(humidity,str3,2);
	strncat(str,str3,2);//在后面拼接湿度值7+2=9
	
	ChangeString(smoke,str4,2);
	strncat(str,str4,2);//在后面拼接烟雾浓度值9+2=11
	
	ChangeString(DangerType,str5,1);
	strncat(str,str5,1);//在后面拼接危险类型11+1=12
	
	ChangeString(Angle,str6,2);
	strncat(str,str6,2);//在后面拼接消防管道开合角度12+2=14
	
	strcat(str,str7);//在后面拼接消防管道开合角度14+2=16

	for(i=0;i<16;i++)
	{
		SBUF = str[i];
		while(TI == 0);	//等待一个byte发送完成后，TI由硬件置1
		TI = 0;	//清除标志位，准备下一次发送

	}


}
//End of Function