#include "main.h"

void Motory_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3; 	//PF2ÊÇENA£¬PF1ÊÇdir£¬PF3ÊÇPUL
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	GPIO_SetBits(GPIOF, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
}  

void Motory_Run(uint32_t dir, uint32_t num, uint32_t speed)
{
	if(dir == 1)
	{
		GPIO_SetBits(GPIOF,GPIO_Pin_1);
	}
	
	if(dir==0)
	{
		GPIO_ResetBits(GPIOF,GPIO_Pin_1);
	}
	
	for(uint32_t i=0;i<=(num*200);i++)
	{
		//Delay_us(speed);
		vTaskDelay(pdMS_TO_TICKS(speed/1000));
		//RTOS_us_Delay(speed);
		GPIOF->ODR ^= GPIO_Pin_3;
	}

}
