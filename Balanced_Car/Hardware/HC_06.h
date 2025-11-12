#ifndef _HC_O6_H_
#define _HC_06_H_

/*  HC06引脚宏定义  */
#define HC06_TXD_Port	GPIOB
#define HC06_TXD_Pin	GPIO_Pin_10
#define HC06_RXD_Port	GPIOB
#define HC06_RXD_Pin	GPIO_Pin_11
/*  HC06初始化  */
void HC06_Init(void);
/*  USART中断服务子程序  */
void USART3_IRQHandler(void);
/*  发送一个字符  */
void USART_SendChar(USART_TypeDef* USARTx, uint16_t charData);
/*  发送一串字符  */
void USART_SendString(USART_TypeDef* USARTx, char* pString);
/*  发送小数  */
void USART_SprintfFloat(USART_TypeDef* USARTx, float Float);

#endif
