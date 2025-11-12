#include "stm32f10x.h"                  // Device header
#include "TB6612.H"

/*  TB6612配置GPIO函数  */
void GPIO_TB6612_Init(void)
{
		//  RCC_GPIOA | RCC_GPIOB
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
		//  GPIO_IN_Init
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Pin = AIN1_Pin;
		GPIO_Init(AIN1_Port, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = AIN2_Pin;
		GPIO_Init(AIN2_Port, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = BIN1_Pin;
		GPIO_Init(BIN1_Port, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = BIN2_Pin;
		GPIO_Init(BIN2_Port, &GPIO_InitStructure);
		//  GPIO_PWM_Init
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = PWMA_Pin;
    GPIO_Init(PWMA_Port, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = PWMB_Pin;
    GPIO_Init(PWMB_Port, &GPIO_InitStructure);
}

/*  TB6612配置PWM函数  */
void TIM_TB6612_Init(void)
{
		//	RCC_TIM1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
		//	内部时钟
		TIM_InternalClockConfig(TIM1);
		//	基础TIM配置
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
		TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInitStructure.TIM_Period = 1000-1;
		TIM_TimeBaseInitStructure.TIM_Prescaler = 72-1;
		TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
		//	输出比较模式配置
		TIM_OCInitTypeDef TIM_OCInitStructure;
		TIM_OCStructInit(&TIM_OCInitStructure);
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OC1Init(TIM1, &TIM_OCInitStructure);
		TIM_OC4Init(TIM1, &TIM_OCInitStructure);
		//	开启TIM1
    TIM_Cmd(TIM1, ENABLE);
		//	高级定时器TIM1开启输出比较模式
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

/*  TB6612初始化函数  */
void TB6612_Init(void)
{
		GPIO_TB6612_Init();
		TIM_TB6612_Init();
}

/*  TB6612左轮：+1000 ~ -1000  */
void TB6612_Left(int Speed)
{
		//	限幅
		Speed = Speed>+1000 ? +1000 : (Speed<-1000?-1000:Speed);
		if(Speed > 0){
				GPIO_WriteBit(AIN1_Port, AIN1_Pin, (BitAction)(0));
				GPIO_WriteBit(AIN2_Port, AIN2_Pin, (BitAction)(1));
				TIM_SetCompare1(TIM1, Speed);
		}
		else{
				GPIO_WriteBit(AIN1_Port, AIN1_Pin, (BitAction)(1));
				GPIO_WriteBit(AIN2_Port, AIN2_Pin, (BitAction)(0));
				TIM_SetCompare1(TIM1, -Speed);
		}
}

/*  TB6612右轮：+1000 ~ -1000  */
void TB6612_Right(int Speed)
{
		//	限幅
		Speed = Speed>+1000 ? +1000 : (Speed<-1000?-1000:Speed);
		if(Speed > 0){
				GPIO_WriteBit(BIN1_Port, BIN1_Pin, (BitAction)(0));
				GPIO_WriteBit(BIN2_Port, BIN2_Pin, (BitAction)(1));
				TIM_SetCompare4(TIM1, Speed);
		}
		else{
				GPIO_WriteBit(BIN1_Port, BIN1_Pin, (BitAction)(1));
				GPIO_WriteBit(BIN2_Port, BIN2_Pin, (BitAction)(0));
				TIM_SetCompare4(TIM1, -Speed);
		}
}

/*  TB6612左右轮：+1000 ~ -1000  */
void TB6612_Motor(int Left, int Right)
{
		TB6612_Left(Left);
		TB6612_Right(Right);
}
