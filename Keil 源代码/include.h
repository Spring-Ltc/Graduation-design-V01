
#ifndef __include_H__
#define __include_H__


#include "reg52.h"
#include "string.h"

#define uint unsigned int
#define uchar unsigned char
	

//MAX7219��IO�ӿ�����
sbit CS = P1^7;
sbit DIN = P1^5;
sbit CLK = P1^6;

//RGB�ƽӿ�����
sbit RGB_R = P2^2;
sbit RGB_G = P2^3;
sbit RGB_B = P2^1;

sbit BUZZ = P2^0;	//�������ӿ�


sbit Motor_IN1 = P1^0;
sbit Motor_IN2 = P1^1;
sbit Motor_IN3 = P1^2;
sbit Motor_IN4 = P1^3;

sbit LCD_EN = P3^4;
sbit LCD_RS = P3^5;
sbit LCD_WR = P3^6;
sbit LCD_RD = P3^7;


sbit DHT11 = P1^4;	//��ʪ�ȴ������ӿ�

sbit AD_CS = P2^4;	//ADC0832ʹ�ܶ�
sbit AD_CLK = P2^5;	
sbit AD_DO = P2^6;
sbit AD_DI = P2^7;	//ADC0832ʱ������

sbit MQ_INT0 = P3^2;
sbit MQ_INT1 = P3^3;	//����������������˿ڣ����ж�����



//�ַ����붨��
#define guan 0x00		//�ضϲ���ʾ
#define heng 0x01		//�м���ʾ��ܼ��
//����������ܱ���

extern uchar code number[10];

extern uchar temperature;
extern uchar humidity;
extern uchar smoke;
extern uchar DangerType;
extern uchar Motor_State;
extern uchar DangerLevel;
											
										
void delay_ms(uint ms);		//�����ʱ
void delay_us(uint us);		//�����ʱ	
void _nop_(void);





#endif


