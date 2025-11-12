#include "stm32f10x.h"                  // Device header

void LightSensor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);			//¿ªÆôTIM2µÄÊ±ÖÓ

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
//	
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);
//	
//	EXTI_InitTypeDef EXTI_InitStructure;
//	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
//	EXTI_Init(&EXTI_InitStructure);
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	
//	NVIC_Init(&NVIC_InitStructure);
}

//uint8_t LightSensor_Get(void)
//{
//	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13);
//}

uint8_t LightSensor_Get1(void)
{
	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
	
}

//void EXTI15_10_IRQHandler(void)
//	{
//		
//		if (EXTI_GetITStatus(EXTI_Line11) == SET) {
//			if(LightSensor_Get1() == 0) {
//				GPIO_ResetBits(GPIOF, GPIO_Pin_2);
//			}
//				
//		EXTI_ClearITPendingBit(EXTI_Line11);	
//	}
//		
//	}


