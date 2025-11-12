#include "stm32f10x.h"

void controle_Init(void)
{
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOG,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_4 | GPIO_Pin_3 | GPIO_Pin_5;		//同时选中两个引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
}

void controlelow(void)
{
		GPIO_ResetBits(GPIOG,GPIO_Pin_3);		
		GPIO_ResetBits(GPIOG,GPIO_Pin_5);
		GPIO_SetBits(GPIOG,GPIO_Pin_4);
		GPIO_SetBits(GPIOG,GPIO_Pin_6);

}

void controlehigh(void)
{
		GPIO_SetBits(GPIOG,GPIO_Pin_3);		
		GPIO_SetBits(GPIOG,GPIO_Pin_5);
		GPIO_ResetBits(GPIOG,GPIO_Pin_4);
		GPIO_ResetBits(GPIOG,GPIO_Pin_6);

}

void controlestop(void)
{
		GPIO_ResetBits(GPIOG,GPIO_Pin_3);		
		GPIO_ResetBits(GPIOG,GPIO_Pin_5);
		GPIO_ResetBits(GPIOG,GPIO_Pin_4);
		GPIO_ResetBits(GPIOG,GPIO_Pin_6);

}
