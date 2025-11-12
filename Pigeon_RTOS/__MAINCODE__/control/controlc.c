#include "stm32f10x.h"

void controlc_Init(void)
{
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOG,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_15;		//PG：直流电机水平推动饲料到平台
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
}

void controlclow(void)
{
		GPIO_ResetBits(GPIOG,GPIO_Pin_13);		
		GPIO_ResetBits(GPIOG,GPIO_Pin_15);
		GPIO_SetBits(GPIOG,GPIO_Pin_12);
		GPIO_SetBits(GPIOG,GPIO_Pin_14);

}

void controlchigh(void)
{
		GPIO_SetBits(GPIOG,GPIO_Pin_13);		
		GPIO_SetBits(GPIOG,GPIO_Pin_15);
		GPIO_ResetBits(GPIOG,GPIO_Pin_12);
		GPIO_ResetBits(GPIOG,GPIO_Pin_14);
}

void controlcstop(void)
{
		GPIO_ResetBits(GPIOG,GPIO_Pin_13);		
		GPIO_ResetBits(GPIOG,GPIO_Pin_15);
		GPIO_ResetBits(GPIOG,GPIO_Pin_12);
		GPIO_ResetBits(GPIOG,GPIO_Pin_14);
}


