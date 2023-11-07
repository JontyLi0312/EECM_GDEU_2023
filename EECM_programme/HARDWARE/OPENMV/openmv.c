/**
 * @file OPENMV.c
 * @author TenderZephyr (2812920762@qq.com)
 * @brief openmv 色块识别
 * @date 2023-11-05
 */
#include "OPENMV.h"
u8 target;

// u16 Serial_RxFlag;
u8 Serial_RxPacket[5] = {'0', '0', '0', '0', '0'};
/**
@return u8 flag
 *          @arg '1' 上电初始标志位状态
 *          @arg '2' 识别到目标色块
 *          @arg '3' 离开目标色块并寻找匹配色块
 *          @arg '4' 找到与目标色块匹配的色块
 */
u8 openmv()
{
	static u8 flag = '1'; // 上电初始标志位状态
	u8 colour;
	if (flag == '1' && (Serial_RxPacket[0] != '0')) // 识别到目标色块
	{
		target = Serial_RxPacket[0];
		flag = '2';
	}
	else if (flag == '2' && Serial_RxPacket[0] == '0') // 离开目标色块
	{
		flag = '3';
	}
	else if (flag == '3') // 寻找匹配色块
	{
		colour = Serial_RxPacket[0];
		if (colour == target)
		{
			flag = '4'; // 找到匹配色块
		}
	}
	return flag;
}