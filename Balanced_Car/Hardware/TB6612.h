#ifndef _TB6612_H_
#define _TB6612_H_

/*  TB6612引脚宏定义  */
#define PWMA_Port GPIOA
#define PWMA_Pin 	GPIO_Pin_11
#define PWMB_Port GPIOA
#define PWMB_Pin 	GPIO_Pin_8
#define AIN2_Port GPIOB
#define AIN2_Pin 	GPIO_Pin_12
#define AIN1_Port GPIOB
#define AIN1_Pin 	GPIO_Pin_13
#define BIN1_Port GPIOB
#define BIN1_Pin 	GPIO_Pin_14
#define BIN2_Port GPIOB
#define BIN2_Pin 	GPIO_Pin_15
/*  TB6612初始化函数  */
void TB6612_Init(void);
/*  TB6612左轮：+1000 ~ -1000  */
void TB6612_Left(int Speed);
/*  TB6612右轮：+1000 ~ -1000  */
void TB6612_Right(int Speed);
/*  TB6612左右轮：+1000 ~ -1000  */
void TB6612_Motor(int Left, int Right);

#endif
