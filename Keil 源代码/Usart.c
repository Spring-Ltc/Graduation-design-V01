


#include "Usart.h"

//���ڳ�ʼ������
void Usart_Init(void)
{
	SCON = 0x50;	//���ڹ�����ʽ1
	TMOD = 0x20;		//��ʱ��1������ʽ2��8λ�Զ���װ��ֵ
	TH1 = 0xfd;
	TL1 = 0xfd;	//���ó�ֵ��ʹ������Ϊ9600
	//PCON |= 0x80;//SMOD = 1��������䲨��������һ��Ϊ19200
	
//	TH1 = 0xfe;
//	TL1 = 0xfe;	//���������䣬����ԭ���ĳ�ֵ��ʹ������Ϊ28800
	
	
	ES = 1;		//�����ڽ����ж�
	TR1 = 1;	//��ʱ��1��ʼ����������������ʱ��
	EA = 1;//�����ж�

}
//StopFlag




//������ת�����ַ�������ָ�����ȣ�����ǰ�油��,����ֻȡ����ָ��λ��
//����ת��0
//���ֻ��ת��20λ
//date:���������	��	Str_out������ַ����׵�ַ;	len:ָ���������
void ChangeString(uchar date,char *Str_out,uchar len)
{
	uchar i=0;
	char string[20] = {0};//����ת�����ַ���

	for(i=0;i<20;i++)
	{
		string[i] = '0' + date%10;	//�ȴ洢���λ
		date /=10;
	}

	for(i=0;i<len;i++)
	{
		*(Str_out+i) = string[len-i-1];
	}
}
//StopFlag



//����һ������֡��ȥ
void SendTheData(void)
{
	uchar i;	//���ͼ�������
	char str[17] = {0};	//��������֡

	char str1[5] = "smart";	//��ʼ��֤λ
	char str2[2] = {0};	//�����¶�ֵ���ַ���
	char str3[2] = {0};	//����ʪ��ֵ���ַ���
	char str4[2] = {0};	//��������Ũ��ֵ���ַ���
	char str5[2] = {0};	//����Σ�յȼ�
	char str6[1] = {0};	//Σ������
	char str7[1] = {0};//�����ܵ�����״̬
	char str8[2] = "ok";	//������־λ

	
	//��У���ʶ�Լ�����ת�����ַ���ӵ��ַ�������
	strcpy(str,str1);	//�����ʼλ��5λ��������֤�û�		//��ʼ0
	
	ChangeString(temperature,str2,2);
	strncat(str,str2,2);	//�ں���ƴ���¶�ֵ						//��ʼ5
	
	ChangeString(humidity,str3,2);
	strncat(str,str3,2);//�ں���ƴ��ʪ��ֵ							//��ʼ7
	
	ChangeString(smoke,str4,2);
	strncat(str,str4,2);//�ں���ƴ������Ũ��ֵ					//��ʼ9
	
	ChangeString(DangerLevel,str5,2);
	strncat(str,str5,2);//ƴ��Σ�յȼ�									//��ʼ11
	
	ChangeString(DangerType,str6,1);
	strncat(str,str6,1);//�ں���ƴ��Σ������						//��ʼ13
	
	ChangeString(Motor_State,str7,1);
	strncat(str,str7,1);//�ں���ƴ�������ܵ�����״̬		//��ʼ14
	
	strcat(str,str8);//�ں���ƴ�������ܵ����ϽǶ�				//��ʼ15

	for(i=0;i<17;i++)
	{
		SBUF = str[i];
		while(TI == 0);	//�ȴ�һ��byte������ɺ�TI��Ӳ����1
		TI = 0;	//�����־λ��׼����һ�η���

	}


}
//End of Function




//����һ������֡
uchar receive_buf[20] = {0};
uchar receive_State = 0;
	//res_State�����λΪ״̬λ��Ϊ1��ʾ���ڴ������ݣ��ݲ���������
	//													Ϊ0��ʾ���տ��У����Խ�������
void UsartReceive() interrupt 4
{
	uchar receive;
	
	if(RI)
	{
		receive = SBUF;	//����һ���ַ�
		if((receive_State & 0x80) != 0x80)
		{
			if(receive == 'E')
			{
				ES = 0;//��ʱ�ش����жϣ��ȴ��������
				receive_State |= 0x80;//���λ��1����ʾ������һ֡
			}
			else
			{
				receive_buf[receive_State & 0x7f] = receive;	//����һ���ֽڵ�����;receive_State�ĵ�7λλ�����ַ
				receive_State++;//����ͳ�Ƽ�1
			}
			RI =0;//���жϱ�־
		}
	}
}
//End of Function






