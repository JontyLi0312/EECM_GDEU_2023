/***
 * @Author: Jonty ljt20030312@Outlook.com
 * @Date: 2023-07-29 16:17
 * @LastEditTime: 2023-07-30 10:34
 * @FilePath: \jy901s\SYSTEM\UART2.h
 * @Description:
 */
#ifndef __UART5_H
#define __UART5_H
#include <sys.h>

void Uart5_init(void);
void Uart5_send(unsigned char *p_data, unsigned int uiSize);

#endif

//------------------End of File----------------------------
