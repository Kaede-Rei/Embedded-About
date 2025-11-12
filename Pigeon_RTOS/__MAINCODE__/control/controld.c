#include "stm32f10x.h"

void controld_Init(void)
{
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;		//PC：下料处的直流电机
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
}

void controldlow(void)
{
		GPIO_ResetBits(GPIOC,GPIO_Pin_6);		
		GPIO_ResetBits(GPIOC,GPIO_Pin_7);
		GPIO_SetBits(GPIOC,GPIO_Pin_8);
		GPIO_SetBits(GPIOC,GPIO_Pin_9);

}

void controldhigh(void)
{
		GPIO_SetBits(GPIOC,GPIO_Pin_6);		
		GPIO_SetBits(GPIOC,GPIO_Pin_7);
		GPIO_ResetBits(GPIOC,GPIO_Pin_8);
		GPIO_ResetBits(GPIOC,GPIO_Pin_9);

}

void controldstop(void)
{
		GPIO_ResetBits(GPIOC,GPIO_Pin_6);		
		GPIO_ResetBits(GPIOC,GPIO_Pin_7);
		GPIO_ResetBits(GPIOC,GPIO_Pin_8);
		GPIO_ResetBits(GPIOC,GPIO_Pin_9);

}
