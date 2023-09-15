/*** 
 * @Author: Jonty ljt20030312@Outlook.com
 * @Date: 2023-07-21 21:13
 * @LastEditTime: 2023-09-14 16:45
 * @FilePath: \Template\HARDWARE\oled.c
 * @Description: 
 */
#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"


void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_WaitAck(void);
void Send_Byte(u8 dat);
void OLED_WR_Byte(u8 dat,u8 mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode);
void OLED_DrawCircle(u8 x,u8 y,u8 r);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode);
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode);
void OLED_ShowNum(u8 x,u8 y,int32_t num,u8 len,u8 size1,u8 mode);
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode);
void OLED_ScrollDisplay(u8 num,u8 space,u8 mode);
void OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode);
void OLED_Init(void);


u8 OLED_GRAM[144][8];


/**
 * @brief 反显函数
 * 
 * @param i 
 * 			@arg 0 正常显示
 * 			@arg 1 反色显示
 */
void OLED_ColorTurn(u8 i)
{
	if(i == 0){
		OLED_WR_Byte(0xA6, OLED_CMD);//正常显示
	}
	if(i == 1){
		OLED_WR_Byte(0xA7, OLED_CMD);//反色显示
	}
}


/**
 * @brief 屏幕旋转180度
 * 
 * @param i 
 * 			@arg 0 正常显示
 * 			@arg 1 反转显示
 */
void OLED_DisplayTurn(u8 i)
{
	if(i == 0){
		OLED_WR_Byte(0xC8,OLED_CMD);//正常显示
		OLED_WR_Byte(0xA1,OLED_CMD);
	}
	if(i = 1){
		OLED_WR_Byte(0xC0,OLED_CMD);//反转显示
		OLED_WR_Byte(0xA0,OLED_CMD);
	}
}


/**
 * @brief 延时
 * 
 */
void IIC_delay(void)
{
	u8 t = 10;
	while(t--);
}


/**
 * @brief I2C起始信号
 * 
 */
void I2C_Start(void)
{
	OLED_SDA_Set();
	OLED_SCL_Set();
	IIC_delay();
	OLED_SDA_Clr();
	IIC_delay();
	OLED_SCL_Clr();
	IIC_delay();
}


/**
 * @brief I2C结束信号
 * 
 */
void I2C_Stop(void)
{
	OLED_SDA_Clr();
	OLED_SCL_Set();
	IIC_delay();
	OLED_SDA_Set();
}


/**
 * @brief 等待信号应答
 * 
 */
void I2C_WaitAck(void) //测数据信号的电平
{
	OLED_SDA_Set();
	IIC_delay();
	OLED_SCL_Set();
	IIC_delay();
	OLED_SCL_Clr();
	IIC_delay();
}


/**
 * @brief 写入一个字节
 * 
 * @param dat 需要写入的字节
 */
void Send_Byte(u8 dat)
{
	u8 i;
	for(i = 0; i < 8; i++){
		//将dat的8位从最高位依次写入
		if(dat & 0x80){
			OLED_SDA_Set();
    	}
		else{
			OLED_SDA_Clr();
    	}

		IIC_delay();
		OLED_SCL_Set();
		IIC_delay();
		OLED_SCL_Clr();//将时钟信号设置为低电平
		dat <<= 1;
  	}
}


/**
 * @brief 发送一个字节
 * 
 * @param dat 发送的字节
 * @param mode 数据/命令标志
 * 				@arg 0 表示命令
 * 				@arg 1 表示数据
 */
void OLED_WR_Byte(u8 dat, u8 mode)
{
	I2C_Start();
	Send_Byte(0x78);
	I2C_WaitAck();
	if(mode){
		Send_Byte(0x40);
	}
  	else{
		Send_Byte(0x00);
	}
	I2C_WaitAck();
	Send_Byte(dat);
	I2C_WaitAck();
	I2C_Stop();
}


/**
 * @brief 开启OLED显示
 * 
 */
void OLED_DisPlay_On(void)
{
	OLED_WR_Byte(0x8D, OLED_CMD);//电荷泵使能
	OLED_WR_Byte(0x14, OLED_CMD);//开启电荷泵
	OLED_WR_Byte(0xAF, OLED_CMD);//点亮屏幕
}


/**
 * @brief 关闭OLED显示
 * 
 */
void OLED_DisPlay_Off(void)
{
	OLED_WR_Byte(0x8D, OLED_CMD);//电荷泵使能
	OLED_WR_Byte(0x10, OLED_CMD);//关闭电荷泵
	OLED_WR_Byte(0xAE, OLED_CMD);//关闭屏幕
}


/**
 * @brief 更新显存到OLED
 * 
 */
void OLED_Refresh(void)
{
	u8 i, n;
	for(i = 0; i < 8; i++){
		OLED_WR_Byte(0xb0 + i, OLED_CMD); //设置行起始地址
		OLED_WR_Byte(0x00, OLED_CMD);   //设置低列起始地址
		OLED_WR_Byte(0x10, OLED_CMD);   //设置高列起始地址

		I2C_Start();
		Send_Byte(0x78);
		I2C_WaitAck();
		Send_Byte(0x40);
		I2C_WaitAck();

		for(n = 0; n < 128; n++){
			Send_Byte(OLED_GRAM[n][i]);
			I2C_WaitAck();
		}

		I2C_Stop();
  	}
}


/**
 * @brief 清屏
 * 
 */
void OLED_Clear(void)
{
	u8 i, n;
	for(i = 0; i < 8; i++){
	   	for(n = 0; n < 128; n++){
			OLED_GRAM[n][i] = 0;//清除所有数据
		}
  	}
	OLED_Refresh();//更新显示
}


/**
 * @brief 画点
 * 
 * @param x 点的x坐标
 * 			@arg 0 ~ 127
 * @param y 点的y坐标
 * 			@arg 0 ~ 63
 * @param t 
 * 			@arg 1 填充
 * 			@arg 0 清空
 */
void OLED_DrawPoint(u8 x, u8 y, u8 t)
{
	u8 i, m, n;
	i = y / 8;
	m = y % 8;
	n = 1 << m;
	if(t){
		OLED_GRAM[x][i] |= n;
	}
	else{
		OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
		OLED_GRAM[x][i] |= n;
		OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
	}
}


/**
 * @brief 画线
 * 
 * @param x1 起点x坐标
 * @param y1 起点y坐标
 * @param x2 终点x坐标
 * @param y2 终点y坐标
 * @param mode 
 * 				@arg 1 填充
 * 				@arg 0 清空
 */
void OLED_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2, u8 mode)
{
	u16 t; 
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;
	delta_x = x2 - x1; //计算坐标增量 
	delta_y = y2 - y1;
	uRow = x1;//画线起点坐标
	uCol = y1;

	//设置单步方向 
	if(delta_x > 0){
		incx=1;
	}
	//垂直线 
	else if (delta_x==0){
		incx=0;
	}
	else {
		incx=-1;
		delta_x = -delta_x;
	}

	if(delta_y>0){
		incy=1;
	}
	//水平线 
	else if (delta_y==0){
		incy=0;
	}
	else {
		incy = -1;
		delta_y = -delta_x;
	}

	//选取基本增量坐标轴
	if(delta_x > delta_y){
		distance = delta_x;
	}  
	else {
		distance = delta_y;
	}

	for(t = 0; t < distance + 1; t++){
		OLED_DrawPoint(uRow,uCol,mode);//画点

		xerr += delta_x;
		yerr+=delta_y;

		if(xerr > distance){
			xerr -= distance;
			uRow += incx;
		}

		if(yerr > distance){
			yerr -= distance;
			uCol += incy;
		}
	}
}


/**
 * @brief 画圆
 * 
 * @param x 圆心x坐标
 * @param y 圆心y坐标
 * @param r 园的半径
 */
void OLED_DrawCircle(u8 x, u8 y, u8 r)
{
	int a, b, num;
    a = 0;
    b = r;

    while(2 * b * b >= r * r){
        OLED_DrawPoint(x + a, y - b,1);
        OLED_DrawPoint(x - a, y - b,1);
        OLED_DrawPoint(x - a, y + b,1);
        OLED_DrawPoint(x + a, y + b,1);
 
        OLED_DrawPoint(x + b, y + a,1);
        OLED_DrawPoint(x + b, y - a,1);
        OLED_DrawPoint(x - b, y - a,1);
        OLED_DrawPoint(x - b, y + a,1);
        
        a++;
        num = (a * a + b * b) - r*r;//计算画的点离圆心的距离

        if(num > 0){
            b--;
            a--;
        }
    }
}


/**
 * @brief 指定位置显示一个字符
 * 
 * @param x 字符的x坐标
 * 			@arg 0 ~ 127
 * @param y 字符的y坐标
 * 			@arg 0 ~ 63
 * @param chr 需要显示的字符
 * @param size1 选择字体
 * 				@arg 8  6 * 8
 * 				@arg 12 6 * 12
 * 				@arg 16 8 * 16
 * 				@arg 24 12 * 24
 * @param mode 显示模式
 * 				@arg 0 反色显示
 * 				@arg 1 正常显示
 */
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 size1, u8 mode)
{
	u8 i, m, temp, size2, chr1;
	u8 x0 = x, y0 = y;
	if(size1 == 8){
		size2 = 6;
	}
	else{
		size2 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * (size1 / 2);  //得到字体一个字符对应点阵集所占的字节数
	}  

	chr1 = chr-' ';  //计算偏移后的值
	for(i = 0; i < size2; i++){
		if(size1 == 8){
			temp = asc2_0806[chr1][i];
		} //调用0806字体
		else if(size1 == 12){
			temp = asc2_1206[chr1][i];
		} //调用1206字体
		else if(size1==16){
			temp = asc2_1608[chr1][i];
		} //调用1608字体
		else if(size1 == 24){
			temp=asc2_2412[chr1][i];
		} //调用2412字体
		else return;
		for(m=0;m<8;m++){
			if(temp & 0x01){
				OLED_DrawPoint(x,y,mode);
			}
			else{
				OLED_DrawPoint(x,y,!mode);
			}

			temp >>= 1;
			y++;
		}

		x++;
		if((size1 != 8) && ((x - x0) == size1 / 2)){
			x = x0;
			y0 = y0 + 8;
		}
		y = y0;
  	}
}


/**
 * @brief 显示字符串
 * 
 * @param x 字符串起点x坐标
 * @param y 字符串起点y坐标
 * @param chr 要显示的字符串
 * @param size1 字体大小
 * @param mode 显示模式
 */
void OLED_ShowString(u8 x, u8 y, u8 *chr, u8 size1, u8 mode)
{
	u8 judge = (*chr >= ' ') && (*chr <= '~');  //判断是不是非法字符!
	while(judge){
		OLED_ShowChar(x,y,*chr,size1,mode);
		if(size1==8)x+=6;
		else x+=size1/2;
		chr++;
    }
}

//m^n
/**
 * @brief 幂
 * 
 * @param m 底数
 * @param n 幂
 * @return u32 返回结果
 */
u32 OLED_Pow(u8 m, u8 n)
{
	u32 result = 1;

	while(n--){
	  result *= m;
	}

	return result;
}


/**
 * @brief 显示数字
 * 
 * @param x 数字起点x坐标
 * @param y 数字起点y坐标
 * @param num 要显示的数字
 * @param len 要显示数字的位数
 * @param size1 字体大小
 * @param mode 显示模式
 */
void OLED_ShowNum(u8 x, u8 y, int32_t num, u8 len, u8 size1, u8 mode)
{
	u8 t,temp,m = 0;
	uint32_t actualNum;

	if(size1 == 8){
		m = 2;
	};

  	if (num >= 0){
		OLED_ShowChar(x, y, '+', 8, 1);
		actualNum = num;
  	}
	else{
		OLED_ShowChar(x, y, '-', 8, 1);
		actualNum = -num;
	}

	for (t = 0; t < len; t++){
		temp = (actualNum / OLED_Pow(10, len - t - 1)) % 10;

		if(temp == 0){
			OLED_ShowChar(x + (size1 / 2 + m) * t + 5, y, '0', size1, mode);
		}
		else {
			OLED_ShowChar(x + (size1 / 2 + m) * t + 5, y, temp + '0', size1, mode);
			
		}
	}
}


/**
 * @brief 显示汉字
 * 
 * @param x 汉字起点x坐标
 * @param y 汉字起点y坐标
 * @param num 汉字对应的序号
 * @param size1 字体大小
 * @param mode 显示模式
 */
void OLED_ShowChinese(u8 x, u8 y, u8 num, u8 size1, u8 mode)
{
	u8 m, temp;
	u8 x0 = x,y0 = y;
	u16 i,size3 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * size1;  //得到字体一个字符对应点阵集所占的字节数

	for(i = 0; i < size3; i++){
		//调用16*16字体
		if(size1 == 16){
			temp = Hzk1[num][i];
		}
		//调用24*24字体
		else if(size1 == 24){
			temp = Hzk2[num][i];
		}
		//调用32*32字体
		else if(size1 == 32){
			temp = Hzk3[num][i];
		}
		//调用64*64字体
		else if(size1 == 64){
			temp = Hzk4[num][i];
		}
		else return;

		for(m = 0; m < 8; m++){
			if(temp & 0x01){
				OLED_DrawPoint(x, y, mode);
			}
			else{ 
				OLED_DrawPoint(x, y, !mode);
			}
			temp >>= 1;
			y++;
		}

		x++;

		if((x - x0) == size1){
			x = x0;y0 = y0 + 8;
		}

		y = y0;
	}
}


/**
 * @brief 滚屏显示汉字
 * 
 * @param num 汉字的个数
 * @param space 每一遍显示的间隔
 * @param mode 显示模式
 */
void OLED_ScrollDisplay(u8 num, u8 space, u8 mode)
{
	u8 i, n, t = 0, m = 0, r;
	while(1){
		if(m == 0){
			//写入一个汉字保存在OLED_GRAM[][]数组中
	    	OLED_ShowChinese(128, 24, t, 16, mode); 
			t++;
		}
		if(t == num){
			//显示间隔
			for(r = 0; r < 16 * space; r++){
				for(i = 1;i < 144; i++){
					for(n = 0; n < 8; n++){
						OLED_GRAM[i-1][n] = OLED_GRAM[i][n];
					}
				}

            OLED_Refresh();
			}

        	t = 0;
        }

		m++;
		if(m == 16){
			m = 0;
		}
		//实现左移
		for(i = 1; i < 144; i++){
			for(n=0;n<8;n++){
				OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
			}
		}
		OLED_Refresh();
	}
}


/**
 * @brief 显示图片
 * 
 * @param x 图片起点x坐标
 * @param y 图片起点y坐标
 * @param sizex 图片长度
 * @param sizey 图片宽度
 * @param BMP 要写入的图片数组
 * @param mode 显示模式
 */
void OLED_ShowPicture(u8 x, u8 y, u8 sizex, u8 sizey, u8 BMP[], u8 mode)
{
	u16 j = 0;
	u8 i, n, temp, m;
	u8 x0 = x, y0 = y;

	sizey = sizey / 8 + ((sizey % 8) ? 1 : 0);

	for(n = 0; n < sizey; n++){
		for(i = 0; i < sizex; i++){
			temp=BMP[j];
			j++;

			for(m=0;m<8;m++){
				if(temp&0x01)OLED_DrawPoint(x,y,mode);
				else OLED_DrawPoint(x,y,!mode);
				temp>>=1;
				y++;
			}

			x++;
			if((x - x0) == sizex){
				x = x0;
				y0 = y0 + 8;
			}

			y = y0;
     	}
	}
}


/**
 * @brief OLED初始化
 * 
 */
void OLED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOG,ENABLE);//使能PORTA~E,PORTG时钟
  	
	//GPIO初始化设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化
	
	//GPIO初始化设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化
	
	OLED_RES_Clr();
	delay_ms(200);
	OLED_RES_Set();
	
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0xFD,OLED_CMD);
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0xA0,OLED_CMD);
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xBF,OLED_CMD);// Set SEG Output Current Brightness
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0x25,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x34,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7)
	OLED_Clear();
	OLED_WR_Byte(0xAF,OLED_CMD);
}

