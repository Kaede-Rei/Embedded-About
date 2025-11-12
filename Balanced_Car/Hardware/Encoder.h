#ifndef _Encoder_H_
#define _Encoder_H_

/*  编码器信号引脚宏定义  */
#define LeftSignalA_Port 	GPIOA
#define LeftSignalA_Pin 	GPIO_Pin_0
#define LeftSignalB_Port 	GPIOA
#define LeftSignalB_Pin		GPIO_Pin_1
#define RightSignalA_Port GPIOB
#define RightSignalA_Pin 	GPIO_Pin_6
#define RightSignalB_Port GPIOB
#define RightSignalB_Pin 	GPIO_Pin_7
/*  Encoder初始化函数  */
void Encoder_Init(void);
/*  读取左轮速度函数  */
int Read_LeftSpeed(void);
/*  读取右轮速度函数  */
int Read_RightSpeed(void);
	
#endif
