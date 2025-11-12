#include "main.h"
#include <stdint.h>
#include <stdbool.h>

/**
	* @brief  应用于RTOS系统的不堵塞CPU的微秒级延时
  * @param  xus：延时时长(范围：0~65535)
  * @retval 无
  */
/*  延时任务句柄  */
static TaskHandle_t delay_task_handle = NULL;  
/*  TIM6初始化函数  */
void TIM6_Init(void) 
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);  									// 使能TIM6时钟

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;            									// 16位计数器
    TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock / 1000000) - 1;  // 1MHz计数
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);  														// 使能更新中断
    TIM_Cmd(TIM6, ENABLE);  																								// 启动TIM6

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/*  延时函数  */
void RTOS_us_Delay(uint32_t us) {
    delay_task_handle = xTaskGetCurrentTaskHandle();  // 获取当前任务句柄

    uint32_t current_time = TIM6->CNT;
    uint32_t target_time = current_time + us;  				// 计算目标时间

    if (target_time < current_time) {  								// 处理溢出
        TIM6->CCR1 = target_time & 0xFFFF;  					// 设置捕获/比较寄存器
    } else {
        TIM6->CCR1 = target_time;  										// 设置捕获/比较寄存器
    }

    TIM6->SR = 0;  																		// 清除中断标志

    ulTaskNotifyTake(pdTRUE, portMAX_DELAY); 					// 等待任务通知
}
/*  TIM6中断服务程序  */
void TIM6_IRQHandler(void) {
    if (TIM_GetITStatus(TIM6, TIM_IT_CC1) != RESET)		// 处理捕获/比较中断
		{  
        if (delay_task_handle != NULL) {
            xTaskNotifyGive(delay_task_handle);  			// 发送任务通知
            delay_task_handle = NULL;  								// 清空任务句柄
        }
        TIM_ClearITPendingBit(TIM6, TIM_IT_CC1);  		// 清除捕获/比较中断标志
    }
}

/**
  * @brief  微秒级延时
  * @param  xus 延时时长，范围：0~233015
  * @retval 无
  */
void Delay_us(uint32_t xus)
{
	SysTick->LOAD = 72 * xus;				//设置定时器重装值
	SysTick->VAL = 0x00;					//清空当前计数值
	SysTick->CTRL = 0x00000005;				//设置时钟源为HCLK，启动定时器
	while(!(SysTick->CTRL & 0x00010000));	//等待计数到0
	SysTick->CTRL = 0x00000004;				//关闭定时器
}

/**
  * @brief  毫秒级延时
  * @param  xms 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_ms(uint32_t xms)
{
	while(xms--)
	{
		Delay_us(1000);
	}
}
 
/**
  * @brief  秒级延时
  * @param  xs 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_s(uint32_t xs)
{
	while(xs--)
	{
		Delay_ms(1000);
	}
} 
