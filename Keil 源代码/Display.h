

//	2020/03/13/18��05			�ò��ִ���(Display��ش���)������ȫ����



/*---------------------------------���˵��----------------------------------------------


��ʾ��Ԫ������MAX7219��������ܣ�RGB�ƣ�������ѷ�����д�������桿



*/////////////////////////////////////////////////////////////////////////////////////////
//


#ifndef __Display_H__
#define __Display_H__


#include "include.h"


//MAX7219�ڲ����ƼĴ�����ַ����
#define Mode 0x09		//����ģʽ
#define Brightness 0x0a		//����
#define Size 0x0b		//����ܸ���
#define Down 0x0c		//�ض�ģʽ
#define Text 0x0f		//���Կ���

//RGB��ɫ�궨��
#define red 1
#define green 2
#define blue 3



														
/*------------------------------------------------�Զ��幦�ܺ�������-----------------------------------------------------------------------------*/

void Write7219(uint Adress,uint Date);	//��7219д�����ݣ�������ַ������		
void MAX7219_Init(void);	//��ʼ������7219оƬ
void FM_TurnON(void);	//��������
void FM_TurnOFF(void);	//�������ر�

void RGB_TurnON(uint color);
void RGB_TurnOFF(uint color);

#endif


