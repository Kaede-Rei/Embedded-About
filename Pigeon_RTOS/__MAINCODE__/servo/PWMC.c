#include "stm32f10x.h"                  // Device header

void PWMC_Init(void)
{
	//RCC开启TIM外设
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	//配置GPIO（初始化为复用推挽输出）
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
		
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	//时钟源选择：内部时钟
	TIM_InternalClockConfig(TIM2);
	
	//配置时基单元（预分频器PSC、计数器CNT、自动重装寄存器ARR，用结构体配置这些参数）
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=20000-1;						//周期，ARR自动重装器的值
	TIM_TimeBaseInitStructure.TIM_Prescaler=72-1;					//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;   			//重复计时器，是高级计时器才有的，此处设为0
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	//配置输出比较单元（结构体统一配置）
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0;									//CCR的值，占空比=CCR/（ARR+1）,500到2500,对应0.5ms到2.5ms
	TIM_OC2Init(TIM2,&TIM_OCInitStructure);
	
	//使能定时器，运行控制
	TIM_Cmd(TIM2,ENABLE);
}
	
void PWMC_SetCompare2(uint16_t Compare)
{
	//单独更改通道2的CCR值的函数
	TIM_SetCompare2(TIM2,Compare);
}
