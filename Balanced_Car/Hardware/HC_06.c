#include "stm32f10x.h"                  // Device header
#include "HC_06.H"
#include "PID.H"
#include "oled.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

/*  PID调参相关参数  */
extern float Vertical_Kp, Vertical_Kd;
extern float Speed_Kp, Speed_Ki;
extern float Turn_Kp, Turn_Kd;

/*  HC06配置GPIO  */
void HC06_GPIO_Init(void)
{
		//	RCC
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    //	GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = HC06_TXD_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(HC06_TXD_Port, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = HC06_RXD_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(HC06_RXD_Port, &GPIO_InitStructure);
}

/*  HC06配置USART  */
void HC06_USART_Init(void)
{
		//	RCC
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		//	USART_Init
		USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
		//	开启接收数据中断
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
		//	NVIC
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_Init(&NVIC_InitStructure);
		//	开启USART
    USART_Cmd(USART3, ENABLE);
}

/*  HC06初始化  */
void HC06_Init(void)
{
		HC06_GPIO_Init();
		HC06_USART_Init();
}

/*  串口接收字符串转换为小数  */
float StrToFloat(char* pString)
{
    uint16_t i = 0, j = 0, t = 0;
    float temp = 0;
		uint16_t len;
		len = strlen(pString);
    for(i = 0; i < len; i++)
    {
        if(pString[i] == '.') //判断小数点存在的位置
        {
            t = 1;  					//小数点存在标志
            j = i;  					//记下小数点存在的位置
        }
        else
        {
            if(t)							//如果在小数点后面
            {
                temp = (float)(pString[i] - '0') / pow(10, (i-j)) + temp; //改为小数
            }
            else 							//在小数点前面
            {
                temp=(float)(pString[i] - '0') + temp*10; 
            }
        }
    }
    return temp;
}

/*  USART中断服务子程序  */
void USART3_IRQHandler(void)
{
		uint8_t data;
		static uint8_t buffer[20];
		static uint8_t buffer_num = 0;
		static char pFloat[10];
		
		if( USART_GetITStatus(USART3, USART_IT_RXNE) != RESET ){
				data = USART_ReceiveData(USART3);					//接收单个数据
				buffer[buffer_num++] = data;							//单次命令数据缓存
				
				//	蓝牙遥控
				if(data == 0x08){													//前进
						CarRun();
				}
				else if(data == 0x02){										//后退
						CarBackRun();
				}
				else if(data == 0x05){										//停止
						CarStop();
				}
				else if(data == 0x04){										//左转
						CarTurnLeft();
				}
				else if(data == 0x06){										//右转
						CarTurnRight();
				}
			
				//	VOFA调节PID参数
				static int PID_Flag = 0;
				if(data == '<'){
						PID_Flag = 1;
						buffer_num = 1;
						memset(buffer, 0, sizeof(buffer));		//重置buffer清空上次数据缓存
						memset(pFloat, 0, sizeof(pFloat));		//重置pFloat确保清空上次转换后数据缓存
				}
				else if( data == '>' || buffer_num > 20 ){
						PID_Flag = 0;													//如果数据错乱直接停止PID调参
						buffer_num = 0;
				}
				if(PID_Flag == 1){	
						if(buffer_num > 5){
								if(data == '#'){									//小数部分结束，开始调参
										if(buffer[2]=='p' && buffer[3]=='1'){
												Vertical_Kp = StrToFloat(pFloat);
										}
										else if(buffer[2]=='d' && buffer[3]=='1'){
												Vertical_Kd = StrToFloat(pFloat);
										}
										else if(buffer[2]=='p' && buffer[3]=='2'){
												Speed_Kp = StrToFloat(pFloat);
										}
										else if(buffer[2]=='i' && buffer[3]=='2'){
												Speed_Ki = StrToFloat(pFloat);
										}
										else if(buffer[2]=='p' && buffer[3]=='3'){
												Turn_Kp = StrToFloat(pFloat);
										}
										else if(buffer[2]=='d' && buffer[3]=='3'){
												Turn_Kd = StrToFloat(pFloat);
										}
										memset(pFloat, 0, sizeof(pFloat));		//调完一次重置一次
								}
								else{
										pFloat[buffer_num-6] = data;					//单次命令小数部分数据缓存
								}
						}
				}
				USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		}
}

/*  发送一个字符  */
void USART_SendChar(USART_TypeDef* USARTx, uint16_t charData)
{
	USART_SendData(USARTx, charData);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) != 1);
	USART_SendData(USARTx, '\n');
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) != 1);
}

/*  发送一串字符  */
void USART_SendString(USART_TypeDef* USARTx, char* pString)
{
		uint16_t len;
		len = strlen(pString);
		for(uint16_t i = 0; i < len; i++){
				USART_SendData(USARTx, *pString++);
				while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) != 1);
		}
		USART_SendData(USARTx, '\n');
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) != 1);
}

/*  发送小数  */
void USART_SprintfFloat(USART_TypeDef* USARTx, float Float)
{
		char buffer[20];
		sprintf(buffer, "%f", Float);
		for(int i = 0; i < 20; i++){
				USART_SendData(USARTx, buffer[i]);
				while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) != 1);
		}
		USART_SendData(USARTx, '\n');
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) != 1);
}
