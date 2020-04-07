


#include "Usart.h"

//串口初始化设置
void Usart_Init(void)
{
	SCON = 0x50;	//串口工作方式1
	TMOD = 0x20;		//定时器1工作方式2，8位自动重装初值
	TH1 = 0xfd;
	TL1 = 0xfd;	//设置初值，使波特率为9600
	//PCON |= 0x80;//SMOD = 1，保留这句波特率增加一倍为19200
	
//	TH1 = 0xfe;
//	TL1 = 0xfe;	//保留这两句，覆盖原来的初值，使波特率为28800
	
	
	ES = 1;		//允许串口接收中断
	TR1 = 1;	//定时器1开始计数，产生波特率时钟
	EA = 1;//开总中断

}
//StopFlag




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
	uchar i;	//发送计数变量
	char str[17] = {0};	//整个数据帧

	char str1[5] = "smart";	//起始验证位
	char str2[2] = {0};	//缓存温度值的字符串
	char str3[2] = {0};	//缓存湿度值的字符串
	char str4[2] = {0};	//缓存烟雾浓度值的字符串
	char str5[2] = {0};	//缓存危险等级
	char str6[1] = {0};	//危险类型
	char str7[1] = {0};//消防管道开合状态
	char str8[2] = "ok";	//结束标志位

	
	//将校验标识以及数据转换成字符添加到字符串里面
	strcpy(str,str1);	//添加起始位，5位，用于验证用户		//起始0
	
	ChangeString(temperature,str2,2);
	strncat(str,str2,2);	//在后面拼接温度值						//起始5
	
	ChangeString(humidity,str3,2);
	strncat(str,str3,2);//在后面拼接湿度值							//起始7
	
	ChangeString(smoke,str4,2);
	strncat(str,str4,2);//在后面拼接烟雾浓度值					//起始9
	
	ChangeString(DangerLevel,str5,2);
	strncat(str,str5,2);//拼接危险等级									//起始11
	
	ChangeString(DangerType,str6,1);
	strncat(str,str6,1);//在后面拼接危险类型						//起始13
	
	ChangeString(Motor_State,str7,1);
	strncat(str,str7,1);//在后面拼接消防管道开合状态		//起始14
	
	strcat(str,str8);//在后面拼接消防管道开合角度				//起始15

	for(i=0;i<17;i++)
	{
		SBUF = str[i];
		while(TI == 0);	//等待一个byte发送完成后，TI由硬件置1
		TI = 0;	//清除标志位，准备下一次发送

	}


}
//End of Function




//接收一个数据帧
uchar receive_buf[20] = {0};
uchar receive_State = 0;
	//res_State的最高位为状态位，为1表示正在处理数据，暂不接收数据
	//													为0表示接收空闲，可以接收数据
void UsartReceive() interrupt 4
{
	uchar receive;
	
	if(RI)
	{
		receive = SBUF;	//接收一个字符
		if((receive_State & 0x80) != 0x80)
		{
			if(receive == 'E')
			{
				ES = 0;//暂时关串口中断，等待处理完成
				receive_State |= 0x80;//最高位置1，表示接收完一帧
			}
			else
			{
				receive_buf[receive_State & 0x7f] = receive;	//缓存一个字节到数组;receive_State的低7位位数组地址
				receive_State++;//数据统计加1
			}
			RI =0;//清中断标志
		}
	}
}
//End of Function






