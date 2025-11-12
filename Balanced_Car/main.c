#include "stm32f10x.h"                  // Device header
#include "DELAY.H"
#include "OLED.H"
#include "ENCODER.H"
#include "HC_06.H"
#include "HC_SR04.H"
#include "TB6612.H"
#include "MPU6050.H"
#include "PID.H"

/*  小车PID控制相关参数  */
extern float Pitch, Roll, Yaw;
extern short GyrX, GyrY, GyrZ;
extern int Encoder_Left, Encoder_Right;

extern float Vertical_Kp, Vertical_Kd;
extern float Speed_Kp, Speed_Ki;
extern float Turn_Kp, Turn_Kd;

/*  平衡车初始化函数  */
void BalancedCar_Init(void)
{
		OLED_Init();
		MPU6050_Init();
		TB6612_Init();
		Encoder_Init();
		HC06_Init();
}

int main()
{
		BalancedCar_Init();
		
		OLED_ShowString(0, 0, "Roll:", OLED_8X16);
		OLED_ShowString(0, 16, "GyrX:", OLED_8X16);
		OLED_ShowString(0, 32, "Speed:", OLED_8X16);
	
		while(1){
				USART_SprintfFloat(USART3, Roll);					//用于PID调参
				OLED_ShowFloat(5*8, 0*16, Roll, OLED_8X16);
				OLED_ShowSignedNum(5*8, 1*16, GyrZ, 5, OLED_8X16);
				OLED_ShowSignedNum(6*8, 2*16, Encoder_Left+Encoder_Right, 5, OLED_8X16);
				OLED_Update();
		}
}
