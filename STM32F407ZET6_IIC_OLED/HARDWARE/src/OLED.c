#include "OLED.h"

#include "stdlib.h"
#include "OLED_Font.h"  	 
#include "Delay.h"

static GPIO_InitTypeDef  GPIO_InitStructure;

//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   
/**********************************************
//IIC Start
**********************************************/
/**********************************************
//IIC Start
**********************************************/
void IIC_Start(void)
{
	/*	读和写的前期准备是主机发给从机，所以是输出*/
	SDA_Pin_Mode(GPIO_Mode_OUT);
	
	/*	手册描述：当SCL为高电平时SDA的下降沿（高到低）
		叫做起始条件。
		所以，先把两根线都置高，表示空闲状态，再让数据
		线置低，这时时钟线还是高，就是在告诉从机开始了，
		最后再做一个时钟线置低操作。
	*/
	SDA_W(Bit_SET);//数据线高
	SCL_W(Bit_SET);//时钟线高
	Delay_us(5);
	
	SDA_W(Bit_RESET);//数据线低
	Delay_us(5);
	
	SCL_W(Bit_RESET);//时钟线低
	Delay_us(5);
}

/*	数据线需要读和写，所以需要看情况更改引脚模式*/
void SDA_Pin_Mode(GPIOMode_TypeDef pin_mode)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode 	= pin_mode;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop(void)
{
	SDA_Pin_Mode(GPIO_Mode_OUT);//确保SDA引脚为输出模式
	
	/*
		手册描述：当SCL为高电平时SDA的上山沿（低到高）
		叫做起始条件。
	*/
	SDA_W(Bit_RESET);//数据线低
	SCL_W(Bit_SET);//时钟线高
	Delay_us(5);
	
	SDA_W(Bit_SET);//数据线高
	Delay_us(5);
	
	//通信结束了，不需要再对时钟线置低
}

uint8_t IIC_Wait_Slave_Ack(void)
{
	uint8_t ack;
	SDA_Pin_Mode(GPIO_Mode_IN);//确保SDA引脚为输入模式
	
	/*
		手册描述：总线上的接收器每收到一个字节就产生
		一个应答，主器件必须产生一个对应的额外的时钟
		脉冲
		1.意思是说，在接收一个字节后，时钟线要设置一个脉冲，
		直接拉高时钟线，然后读数据线，判断从机是否应答，
		低电平就是有应答，高电平就是没有应答
		2.拉高时钟线，告诉采集方当前SDA引脚的电平是可靠的，
		可以读取访问
	*/
	SCL_W(Bit_SET);
	Delay_us(5);
	
	//读取SDA引脚的电平
	if(SDA_R)
		ack=1;//无应答
	else
		ack=0;//有应答
	
	//拉低时钟线
	SCL_W(Bit_RESET);
	Delay_us(5);	
		
	return ack;
}

//void IIC_Send_Ack(uint8_t ack)
//{
//	SDA_Pin_Mode(GPIO_Mode_OUT);//确保SDA引脚为输出模式

//	/*	
//		在发送应答前，时钟线高低都可以正常发送应答，
//		但根据时序图，置高合理点，数据线必须先置低，
//		然后发送应答
//	*/
//	SCL_W(Bit_SET);/*这是为了在发送应答信号之前暂停时钟
//					数据交换只允许发生在时钟线为低的时候*/
//	SDA_W(Bit_RESET);//这是为了确保在发送应答信号之前，数据线是低电平
//	Delay_us(5);
//	
//	if(ack)
//	{
//		SDA_W(Bit_SET);
//	}
//	else
//	{
//		SDA_W(Bit_RESET);
//	}
//	
//	Delay_us(5);
//	
//	SCL_W(Bit_SET);//这是为了结束应答信号的发送。
//	Delay_us(5);
//	SCL_W(Bit_RESET);//这是为了释放时钟线，准备下一次数据传输
//	Delay_us(5);	
//}

/**********************************************
// IIC Write byte
**********************************************/

void Write_IIC_Byte(unsigned char byte)
{
	int32_t i;
	
	SDA_Pin_Mode(GPIO_Mode_OUT);//确保SDA引脚为输出模式
	
	SCL_W(Bit_RESET);//拉低，允许数据交换
	Delay_us(5);
	
	for(i=7; i>=0; i--)
	{
		//优先去设置SDA引脚的电平
		if(byte & (1<<i))
			SDA_W(Bit_SET);
		else
			SDA_W(Bit_RESET);
	
		Delay_us(5);
		
		//拉高时钟线，告诉采集方当前SDA引脚的电平是可靠的，可以读取访问
		SCL_W(Bit_SET);
		Delay_us(5);
	
		//拉低时钟线，告诉采集方当前SDA引脚的电平是不可靠的，因为当前要变更SDA引脚的电平
		SCL_W(Bit_RESET);
		Delay_us(5);	
	}
}
/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
   IIC_Start();
   Write_IIC_Byte(0x78);            //Slave address,SA0=0
	IIC_Wait_Slave_Ack();	
   Write_IIC_Byte(0x00);			//write command
	IIC_Wait_Slave_Ack();	
   Write_IIC_Byte(IIC_Command); 
	IIC_Wait_Slave_Ack();	
   IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
   IIC_Start();
   Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
	IIC_Wait_Slave_Ack();	
   Write_IIC_Byte(0x40);			//write data
	IIC_Wait_Slave_Ack();	
   Write_IIC_Byte(IIC_Data);
	IIC_Wait_Slave_Ack();	
   IIC_Stop();
}
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
			{

   Write_IIC_Data(dat);
   
		}
	else {
   Write_IIC_Command(dat);
		
	}


}


/********************************************
// fill_Picture
********************************************/
void fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WR_Byte(0xb0+m,0);		//page0-page1
		OLED_WR_Byte(0x00,0);		//low column start address
		OLED_WR_Byte(0x10,0);		//high column start address
		for(n=0;n<128;n++)
			{
				OLED_WR_Byte(fill_Data,1);
			}
	}
}


/***********************Delay****************************************/
void Delay_50ms(unsigned int Del_50ms)
{
	unsigned int m;
	for(;Del_50ms>0;Del_50ms--)
		for(m=6245;m>0;m--);
}

void Delay_1ms(unsigned int Del_1ms)
{
	unsigned char j;
	while(Del_1ms--)
	{	
		for(j=0;j<123;j++);
	}
}

//坐标设置

	void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}   	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}
void OLED_On(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
	} //更新显示
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(Char_Size ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			}
			else {	
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
				
			}
}
//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 
//显示一个字符号串
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//显示汉字
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 

//初始化SSD1306					    
void OLED_Init(void)
{ 	
 /* 使能硬件时钟:端口硬件时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10;
	GPIO_Init(GPIOD, &GPIO_InitStructure);	
	
	//GPIO_WriteBit(GPIOB,GPIO_Pin_8,Bit_SET);
	//GPIO_WriteBit(GPIOB,GPIO_Pin_9,Bit_SET);	
	SDA_W(Bit_SET);
	SCL_W(Bit_SET);	
 	
 
	Delay_ms(200);

	OLED_WR_Byte(0xAE,OLED_CMD);//--display off
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD); // contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--128   
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
}  
