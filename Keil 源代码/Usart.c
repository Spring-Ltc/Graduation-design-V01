


#include "Usart.h"

//���ڳ�ʼ������
void Usart_Init(void)
{
	SCON = 0x50;	//���ڹ�����ʽ1
	TMOD = 0x20;		//��ʱ��1������ʽ2��8λ�Զ���װ��ֵ
	TH1 = 0xfd;
	TL1 = 0xfd;
	ES = 1;		//�����ڽ����ж�
	TR1 = 1;	//��ʱ��1��ʼ����������������ʱ��
	EA = 1;//�����ж�

}





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
	char str[16] = {0};	//��������֡
	
	char str1[5] = "smart";	//��ʼ��֤λ
	char str2[2] = {0};	//�����¶�ֵ���ַ���
	char str3[2] = {0};	//����ʪ��ֵ���ַ���
	char str4[2] = {0};	//��������Ũ��ֵ���ַ���
	char str5[1] = {0};	//Σ������
	char str6[2] = {0};//�����ܵ����ϽǶ�
	
	char str7[2] = "ok";	//������־λ
	
	uchar i;	//���ͼ�������

	
	//��У���ʶ�Լ�����ת�����ַ���ӵ��ַ�������
	strcpy(str,str1);	//�����ʼλ��5λ��������֤�û�0+5=5
	
	ChangeString(temperature,str2,2);
	strncat(str,str2,2);	//�ں���ƴ���¶�ֵ5+2=7
	
	ChangeString(humidity,str3,2);
	strncat(str,str3,2);//�ں���ƴ��ʪ��ֵ7+2=9
	
	ChangeString(smoke,str4,2);
	strncat(str,str4,2);//�ں���ƴ������Ũ��ֵ9+2=11
	
	ChangeString(DangerType,str5,1);
	strncat(str,str5,1);//�ں���ƴ��Σ������11+1=12
	
	ChangeString(Angle,str6,2);
	strncat(str,str6,2);//�ں���ƴ�������ܵ����ϽǶ�12+2=14
	
	strcat(str,str7);//�ں���ƴ�������ܵ����ϽǶ�14+2=16

	for(i=0;i<16;i++)
	{
		SBUF = str[i];
		while(TI == 0);	//�ȴ�һ��byte������ɺ�TI��Ӳ����1
		TI = 0;	//�����־λ��׼����һ�η���

	}


}
//End of Function