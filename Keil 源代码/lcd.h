

//2020/03/13/20：45				测试正常运行


/*-----------------------------LCD液晶部分代码说明-------------------------------------------------------//




-------------------------------------------------------------------------------------------------*/
//

#ifndef __Lcd_H__
#define __Lcd_H__

#include "include.h"




void write_com(uchar com);
void write_date(uchar DATA);

void LCD_Init(void);
void SetPoint_xy(uchar x,uchar y);






#endif


