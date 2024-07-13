/**
   ******************************************************************************
   * @file    STM32F407ZET6_IIC_OLED\HARDWARE\OLED.h
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-07-11
   * @brief   
   ******************************************************************************
   * 
   ******************************************************************************
**/

#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    	
#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED IIC端口定义----------------  					   

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_6)//SCLK IIC接口的时钟信号
#define OLED_SCLK_Set() GPIO_SetBits(GPIOB,GPIO_Pin_6)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_7)//SDA IIC接口的数据信号
#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_7)

#define SDA_W(x)	PDout(10)=(x)//GPIO_WriteBit(GPIOB,GPIO_Pin_1,(x))
#define SDA_R		PDin(10)//GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)
#define SCL_W(x)	PBout(15)=(x)//GPIO_WriteBit(GPIOB,GPIO_Pin_2,(x))
 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//OLED控制用函数
void OLED_WR_Byte(unsigned dat,unsigned cmd);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p,u8 Char_Size);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void Delay_50ms(unsigned int Del_50ms);
void Delay_1ms(unsigned int Del_1ms);
void fill_picture(unsigned char fill_Data);
//void Picture();
void IIC_Start(void);
void IIC_Stop(void);
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);

uint8_t IIC_Wait_Slave_Ack(void);
//void IIC_Send_Ack(uint8_t ack);

void SDA_Pin_Mode(GPIOMode_TypeDef pin_mode);
#endif  
