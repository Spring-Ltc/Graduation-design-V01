

//


/*---------------------------------------------------相关说明--------------------------------------------------------------






*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//




#ifndef __Usart_H__
#define __Usart_H__

#include "include.h"


void ChangeString(uchar date,char *Str_out,uchar len);//字符串转换函数

void SendTheData(void);

void Usart_Init(void);

extern uchar receive_buf[20];
extern uchar receive_State;


#endif




