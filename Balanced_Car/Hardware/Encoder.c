#include "stm32f10x.h"                  // Device header
#include "TB6612.H"
#include "Encoder.H"
#include "Delay.h"

/*  Encoder配置GPIO函数  */
void GPIO_Encoder_Init(void)
{
		//	RCC_GPIOA|B
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
		//	GPIO_Init
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Pin = LeftSignalA_Pin;
		GPIO_Init(LeftSignalA_Port, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = LeftSignalB_Pin;
		GPIO_Init(LeftSignalB_Port, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = RightSignalA_Pin;
    GPIO_Init(RightSignalA_Port, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = RightSignalB_Pin;
    GPIO_Init(RightSignalB_Port, &GPIO_InitStructure);
}

/*  Encoder配置TIM函数  */
void TIM_Encoder_Init(void)
{
		//	RCC_TIM2|4
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM4, ENABLE);
		//	内部时钟
		TIM_InternalClockConfig(TIM2);
		TIM_InternalClockConfig(TIM4);
		//	TimeBase_Init
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 65536-1;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
		//	TIM_Enconder模式配置
    TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
		TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
		//	从0开始计数
		TIM_SetCounter(TIM2,0);
		TIM_SetCounter(TIM4,0);
    //	开启TIM
		TIM_Cmd(TIM2, ENABLE);
    TIM_Cmd(TIM4, ENABLE);
}

/*  Encoder初始化函数  */
void Encoder_Init(void)
{
		GPIO_Encoder_Init();
		TIM_Encoder_Init();
}

/*  读取左轮速度函数  */
int Read_LeftSpeed(void)
{
		int temp;
		temp = -(short)TIM_GetCounter(TIM2);
		TIM_SetCounter(TIM2, 0);
	
		return temp * (61.5 / 7);
}

/*  读取右轮速度函数  */
int Read_RightSpeed(void)
{
		int temp;
		temp = (short)TIM_GetCounter(TIM4);
		TIM_SetCounter(TIM4, 0);
	
		return temp *  (82.8 / 9);
}
