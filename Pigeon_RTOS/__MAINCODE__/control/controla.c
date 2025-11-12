#include "stm32f10x.h"

void controla_Init(void)
{
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_4 | GPIO_Pin_7 | GPIO_Pin_5;		//PA：控制电机翻转180°以及真空泵电磁阀
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
}

void controlalow(void)
{
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);		
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
}

void controlahigh(void)
{
		GPIO_SetBits(GPIOA,GPIO_Pin_6);		
		GPIO_SetBits(GPIOA,GPIO_Pin_7);
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);

}

void controlastop(void)
{
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);		
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);

}

