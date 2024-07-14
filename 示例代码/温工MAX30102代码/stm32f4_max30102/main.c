#include <stdio.h>
#include "stm32f4xx.h"
#include "sys.h"
#include "algorithm.h"
#include "max30102.h" 
#include "delay.h"
#include <string.h>

static GPIO_InitTypeDef		GPIO_InitStruct;
static NVIC_InitTypeDef		NVIC_InitStructure;
static USART_InitTypeDef 	USART_InitStructure;


uint32_t aun_ir_buffer[500]; //IR LED sensor data
int32_t n_ir_buffer_length;    //data length
uint32_t aun_red_buffer[500];    //Red LED sensor data
int32_t n_sp02; //SPO2 value
int8_t ch_spo2_valid;   //indicator to show if the SP02 calculation is valid
int32_t n_heart_rate;   //heart rate value
int8_t  ch_hr_valid;    //indicator to show if the heart rate calculation is valid
uint8_t uch_dummy;

#define MAX_BRIGHTNESS 255


int32_t fputc(int32_t ch,FILE *file)
{
	USART_SendData(USART1,ch);
	//�ȴ��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);

	return ch;
}

void usart1_init(uint32_t baud)
{

	//�˿�AӲ��ʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	
	
	//����1Ӳ��ʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	


	//����Ӳ��������GPIO���˿�A����9 10����
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_10;//��9 10����
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;			//���ù���ģʽ
	GPIO_InitStruct.GPIO_Speed=GPIO_High_Speed;		//���Ÿ��ٹ������յ�ָ������������ȱ�㣺���ĸ�
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;		//�����������������
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;		//����Ҫ����������
	GPIO_Init(GPIOA,&GPIO_InitStruct);


	//��PA9�������ӵ�����1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);

	//��PA10�������ӵ�����1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);



	USART_InitStructure.USART_BaudRate = baud;					//������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//8λ����λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;			//��У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//����Ҫ������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�����ڽ��պͷ�������
	USART_Init(USART1, &USART_InitStructure);

	//�������ݺ󴥷��ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	/* ���ô���1 */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART1, ENABLE);
}


//������
int32_t main(void)
{
	//variables to calculate the on-board LED brightness that reflects the heartbeats
	uint32_t un_min, un_max, un_prev_data;  
	int32_t i;
	int32_t n_brightness;
	float f_temp;

	uint8_t temp[6];

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	//ʹ�ܶ˿�F��Ӳ��ʱ�ӣ��˿�F���ܹ�����˵���˾��ǶԶ˿�F�ϵ�
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);	

	//����Ӳ��������GPIO���˿�F����9������
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9;			//��9 ������
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;		//���ģʽ
	GPIO_InitStruct.GPIO_Speed=GPIO_High_Speed;		//���Ÿ��ٹ������յ�ָ������������ȱ�㣺���ĸ�
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;		//�����������������
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;		//����Ҫ����������
	GPIO_Init(GPIOF,&GPIO_InitStruct);
	
	PFout(9)=1;

	//���ô���1������Ϊ115200bps
	usart1_init(115200);
	
	max30102_init();

	printf("This is max30102 test by teacher.wen\r\n");

	un_min=0x3FFFF;
	un_max=0;
	
	n_ir_buffer_length=500; //buffer length of 100 stores 5 seconds of samples running at 100sps
	//read the first 500 samples, and determine the signal range
    for(i=0;i<n_ir_buffer_length;i++)
    {
        while(MAX30102_INT==1);   //wait until the interrupt pin asserts
        
		max30102_FIFO_ReadBytes(REG_FIFO_DATA,temp);
		aun_red_buffer[i] =  (long)((long)((long)temp[0]&0x03)<<16) | (long)temp[1]<<8 | (long)temp[2];    // Combine values to get the actual number
		aun_ir_buffer[i] = (long)((long)((long)temp[3] & 0x03)<<16) |(long)temp[4]<<8 | (long)temp[5];   // Combine values to get the actual number
            
        if(un_min>aun_red_buffer[i])
            un_min=aun_red_buffer[i];    //update signal min
        if(un_max<aun_red_buffer[i])
            un_max=aun_red_buffer[i];    //update signal max
    }
	un_prev_data=aun_red_buffer[i];
	//calculate heart rate and SpO2 after first 500 samples (first 5 seconds of samples)
    maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid); 
	
	while(1)
	{
		i=0;
        un_min=0x3FFFF;
        un_max=0;
		n_ir_buffer_length=500;
		
		/* dumping the first 100 sets of samples in the memory and shift the last 400 sets of samples to the top
		
		   ��ǰ100������ת�����洢���У��������400�������Ƶ�����
		*/
		
        for(i=100;i<500;i++)
        {
            aun_red_buffer[i-100]=aun_red_buffer[i];
            aun_ir_buffer[i-100]=aun_ir_buffer[i];
            
            /* update the signal min and max 
			   �����ź���Сֵ�����ֵ
			*/
			
            if(un_min>aun_red_buffer[i])
				un_min=aun_red_buffer[i];
			
            if(un_max<aun_red_buffer[i])
				un_max=aun_red_buffer[i];
        }
		
		/* take 100 sets of samples before calculating the heart rate 
		
		   �ڼ�������֮ǰ�ɼ�100������
		*/
		
        for(i=400;i<500;i++)
        {
            un_prev_data=aun_red_buffer[i-1];
			
            while(MAX30102_INT==1);
			
            max30102_FIFO_ReadBytes(REG_FIFO_DATA,temp);
			
			/* ���ֵ�Ի��ʵ������ */
			aun_red_buffer[i] =  ((temp[0]&0x03)<<16) |(temp[1]<<8) | temp[2];   
			aun_ir_buffer[i] =   ((temp[3]&0x03)<<16) |(temp[4]<<8) | temp[5];   
        
            if(aun_red_buffer[i]>un_prev_data)
            {
                f_temp=aun_red_buffer[i]-un_prev_data;
				
                f_temp/=(un_max-un_min);
				
                f_temp*=MAX_BRIGHTNESS;
				
                n_brightness-=(int32_t)f_temp;
				
                if(n_brightness<0)
                    n_brightness=0;
            }
            else
            {
                f_temp=un_prev_data-aun_red_buffer[i];
				
                f_temp/=(un_max-un_min);
				
                f_temp*=MAX_BRIGHTNESS;
				
                n_brightness+=(int32_t)f_temp;
				
                if(n_brightness>MAX_BRIGHTNESS)
                    n_brightness=MAX_BRIGHTNESS;
            }
		}

		/* �������ʺ�Ѫ�����Ͷ� */
        maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);
		
		/* ͨ��UART�������ͼ��������͵��ն˳��� */
		if((ch_hr_valid == 1)&& (n_heart_rate>=60) && (n_heart_rate<=100))
		{

			printf("����=%d\r\n", n_heart_rate); 
		}

		
		if((ch_spo2_valid ==1)&& (n_sp02>=95) && (n_sp02<=100))
		{
			printf("Ѫ��Ũ��=%d\r\n", n_sp02); 
		}			
	
		//delay_ms(100);

	}
}


void USART1_IRQHandler(void)
{
	uint8_t d;
	
	//��鴮��1�Ƿ���յ�����
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
	{
		//��ȡ����
		d = USART_ReceiveData(USART1);
		
		//��ձ�־λ
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);	
	
	}
}


