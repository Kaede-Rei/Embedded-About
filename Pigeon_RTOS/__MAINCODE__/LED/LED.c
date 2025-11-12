#include "stm32f10x.h"                  // Device header

void LED_Init(void)
{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);	//初始化LED，打开时钟，配置端口模式等
		
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOA,&GPIO_InitStructure);
	
		GPIO_SetBits(GPIOE,GPIO_Pin_5);
}

void LED1_ON(void)
{
	GPIO_ResetBits(GPIOE,GPIO_Pin_5);
}

void LED1_OFF(void)
{
	GPIO_SetBits(GPIOE,GPIO_Pin_5);
}

//void LED1_Turn(void)//取反
//{
//	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_1)==0)
//	{
//		GPIO_SetBits(GPIOA,GPIO_Pin_1);
//	}
//	else
//	{
//		GPIO_ResetBits(GPIOA,GPIO_Pin_1);
//	}
//}

//void LED2_ON(void)
//{
//	GPIO_ResetBits(GPIOA,GPIO_Pin_2);
//}

//void LED2_OFF(void)
//{
//	GPIO_SetBits(GPIOA,GPIO_Pin_2);
//}

//void LED2_Turn(void)
//{
//	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_2)==0)
//	{
//		GPIO_SetBits(GPIOA,GPIO_Pin_2);
//	}
//	else
//	{
//		GPIO_ResetBits(GPIOA,GPIO_Pin_2);
//	}
//}
