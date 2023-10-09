/*** 
 * @Author: Jonty ljt20030312@Outlook.com
 * @Date: 2023-07-29 19:56
 * @LastEditTime: 2023-07-30 15:38
 * @FilePath: \jy901s\SYSTEM\jy901s.c
 * @Description: 
 */
#include <string.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "wit_c_sdk.h"
#include "delay.h"
#include "jy901s.h"
#include "UART2.h"

#define ACC_UPDATE		0x01
#define GYRO_UPDATE		0x02
#define ANGLE_UPDATE	0x04
#define MAG_UPDATE		0x08
#define READ_UPDATE		0x80
static volatile char s_cDataUpdate = 0, s_cCmd = 0xff;
const uint32_t c_uiBaud[10] = {0, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600};
static void SensorUartSend(uint8_t *p_data, uint32_t uiSize);
static void SensorDataUpdata(uint32_t uiReg, uint32_t uiRegNum);
static void Delayms(uint16_t ucMs);

void jy901s_init(void);
void jy901s_getData(jy901s_angleData *jy901s_angleData);
void jy901s_yawSet0(void);

void jy901s_init(void)
{
	Usart2_init(115200);
	WitInit(WIT_PROTOCOL_NORMAL, 0x50);
	WitSerialWriteRegister(SensorUartSend);
	WitRegisterCallBack(SensorDataUpdata);
	WitDelayMsRegister(Delayms);
}


/**
 * @brief 
 * 
 * @param jy901s_angleData 
 */
void jy901s_getData(jy901s_angleData *jy901s_angleData)
{
	jy901s_angleData->roll = sReg[Roll] / 32768.0f * 180.0f;
	jy901s_angleData->pitch = sReg[Pitch] / 32768.0f * 180.0f;
	jy901s_angleData->yaw = sReg[Yaw] / 32768.0f * 180.0f;
}

void jy901s_yawSet0(void)
{

}

static void SensorUartSend(uint8_t *p_data, uint32_t uiSize)
{
	Uart2_send(p_data, uiSize);
}

static void Delayms(uint16_t ucMs)
{
	delay_ms(ucMs);
}

static void SensorDataUpdata(uint32_t uiReg, uint32_t uiRegNum)
{
	int i;
    for(i = 0; i < uiRegNum; i++)
    {
        switch(uiReg)
        {
//            case AX:
//            case AY:
            case AZ:
				s_cDataUpdate |= ACC_UPDATE;
            break;
//            case GX:
//            case GY:
            case GZ:
				s_cDataUpdate |= GYRO_UPDATE;
            break;
//            case HX:
//            case HY:
            case HZ:
				s_cDataUpdate |= MAG_UPDATE;
            break;
//            case Roll:
//            case Pitch:
            case Yaw:
				s_cDataUpdate |= ANGLE_UPDATE;
            break;
            default:
				s_cDataUpdate |= READ_UPDATE;
			break;
        }
		uiReg++;
    }
}
