
2020/04/06修改：
	先完成系统配置，再启动系统工作





1、串口通信数据帧格式：
	起始位（5为）：5个随机数字或字符，用于验证用户设备
	数据位：
		温度值（2位）：
		湿度值（2位）：
		烟雾浓度值（2位）：
		当前危险等级（2位）：【或由APP端计算】本地计算
		危险类型（1位）：
		消防管道阀门当前开合状态（1位）：
	结束位（2位）：APP收到结束标志位，才认为本次数据生效
	
	1		2			3		4		5		6	7		8
	起始位 + 温度值 + 湿度值 + 浓度值 + 等级 + 类型 + 状态 + 结束位  =  共
	  5		   2	   2		2		2	  1 	 1		 2			17位
	
	
		共 5+2+2+2+2+1+1+2 = 17 bit
		
2、编码方式：
	GB2312	【APP中蓝牙客户机组件的编码方式和keil的设置要一样】
		
		
3、串口接收：
	接收数据为：“	温度阈值 ； 湿度阈值 ； 浓度阈值  ； 危险等级1 ； 危险等级2  ；  E  ”
			20;70;60;10;20;E
	
	结束标志 E