#include "stm32f10x.h"                  // Device header
#include "HC_SR04.H"
#include "DELAY.H"

uint32_t TIME_distance = 0;							//HC-SR04高电平持续时间(单位us)

/*  配置HC-SR04的GPIO  */
void GPIO_HCSR04_Init(void)
{
		//  RCC_GPIOA
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		//  GPIO_Init
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Pin = TRIG_Pin;
		GPIO_Init(TRIG_Port, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//可浮空可下拉，最好浮空
		GPIO_InitStructure.GPIO_Pin = ECHO_Pin;
		GPIO_Init(ECHO_Port, &GPIO_InitStructure);
		GPIO_ResetBits(TRIG_Port, TRIG_Pin);	//拉低TRIG引脚防止误判
		//	RCC_AFIO
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		//	AFIO_Init
		GPIO_EXTILineConfig(ECHO_EXTIPort, ECHO_EXTIPin);
		//	EXTI_Init
		EXTI_InitTypeDef EXTI_InitStructure;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_InitStructure.EXTI_Line = ECHO_EXTILine;
		EXTI_Init(&EXTI_InitStructure);
		//	NVIC_Init
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannel = ECHO_NVICIRQ;
		NVIC_Init(&NVIC_InitStructure);
}

/*  配置HC-SR04的定时器  */
void TIM_HCSR04_Init(void)
{
		//	RCC_TIM3
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		//	配置TIM3使用内部时钟源
		TIM_InternalClockConfig(TIM3);
		//	配置时钟分频、计数模式、自动重装值、预分频值、重复计数次数(仅高级定时器有)
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
		TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInitStructure.TIM_Period = 10-1;				//72MHz / (10*72) =
		TIM_TimeBaseInitStructure.TIM_Prescaler = 72-1;			//100KHz ——> 定时10us
		TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
		//	清除TIM3的更新中断标志
		TIM_ClearFlag(TIM3, TIM_IT_Update);
		//	使能TIM3的更新中断
		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
		//	NVIC_Init
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_Init(&NVIC_InitStructure);
}

/*  HC-SR04初始化函数  */
void HCSR04_Init(void)
{
		GPIO_HCSR04_Init();
		TIM_HCSR04_Init();
}

/*  读取测量结果函数  */
uint32_t HCSR04_Measure(void)
{
		TIME_distance = 0;					//测量前先赋0
		
		GPIO_WriteBit(TRIG_Port, TRIG_Pin, Bit_SET);
		delay_us(10);								//10us高电平TRIG引脚触发测距
		GPIO_WriteBit(TRIG_Port, TRIG_Pin, Bit_RESET);
		delay_ms(60);								//一定要间隔60ms，否则前后周期冲突

		return ( TIME_distance * 17 ) / 10;					//转换为毫米
}

/*  ECHO中断，开启与关闭定时器  */
void EXTI2_IRQHandler()
{
		if(EXTI_GetITStatus(ECHO_EXTILine) == SET){
				if( GPIO_ReadInputDataBit(ECHO_Port, ECHO_Pin) ){
						TIM_Cmd(TIM3, ENABLE);
				}
				if( !GPIO_ReadInputDataBit(ECHO_Port, ECHO_Pin) ){
						TIM_Cmd(TIM3, DISABLE);
				}
				EXTI_ClearITPendingBit(ECHO_EXTILine);
		}
}

/*  计时(TIME_distance)ms  */
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_FLAG_Update) == SET)
	{
		TIME_distance++;					//每10us自增一次
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);					//清除更新中断标志位
	}
}
