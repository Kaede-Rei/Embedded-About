#ifndef _HC_SR04_H_
#define _HC_SR04_H_

/*  ECHO相关配置  */
#define ECHO_Port 		GPIOA
#define ECHO_Pin 			GPIO_Pin_2
#define ECHO_EXTIPort GPIO_PortSourceGPIOA
#define ECHO_EXTIPin 	GPIO_PinSource2
#define ECHO_EXTILine EXTI_Line2
#define ECHO_NVICIRQ 	EXTI2_IRQn
/*  TRIG相关配置  */
#define TRIG_Port 		GPIOA
#define TRIG_Pin 			GPIO_Pin_3
/*  HC-SR04初始化函数  */
void HCSR04_Init(void);
/*  读取HC-SR04测量值  */
uint32_t HCSR04_Measure(void);

#endif
