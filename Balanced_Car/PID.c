#include "stm32f10x.h"
#include "Encoder.h"
#include "Mpu6050.h"
#include "TB6612.H"
#include "oled.h"

float Vertical_Kp = 18, Vertical_Kd = 0.125;
float Speed_Kp = 0.142, Speed_Ki = 0.000710;
float Turn_Kp = 10, Turn_Kd = 0.0;
float BlancedOffset = 4.2;

int Encoder_Left, Encoder_Right;
float Pitch, Roll, Yaw;
short GyrX, GyrY, GyrZ;

int Vertical_Out, Speed_Out, Turn_Out, PWM_Out;
int expectSpeed = 0, expectTurn = 0;
int Turn_Left_Out, Turn_Right_Out;
char TurnLeft_Flag = 0, TurnRight_Flag = 0;

/*
 *	直立环PD控制器
 *	参数：目标(期望)角度， 反馈(真实)角度， 反馈(真实)角速度 
 *	返回值：输出速度
 */
int Vertical_PID(float expectAngle, float actualAngle, float actualGyrX)
{
		return Vertical_Kp*(expectAngle-actualAngle) - Vertical_Kd*actualGyrX;
}

/*
 *	速度环PI控制器
 *	参数：目标(期望)速度， 反馈(真实)左编码器速度， 反馈(真实)右编码器速度
 *	返回值：输出角度
 */
int Speed_PID(int expectSpeed, int actualLeftSpeed, int actualRightSpeed)
{
		int Err, Err_LowOut;
		static int Err_Last, Integral;
		static float a = 0.7;
		//	计算误差
		Err = expectSpeed - (actualLeftSpeed+actualRightSpeed);
		//	低通滤波
		Err_LowOut = a*Err + (1-a)*Err_Last;
		Err_Last = Err_LowOut;
		//	积分与限幅( -2000 ~ +2000 )
		Integral += Err_LowOut;
		Integral = Integral>+2000 ? +2000 : (Integral<-2000 ? -2000 : Integral);
		//	返回输出值
		return -(Speed_Kp*Err_LowOut + Speed_Ki*Integral);
}

/*
 *	转向环PD控制器
 *	参数：转向角度(误差)， 反馈(真实)角速度 
 *	返回值：输出PWM
 */
int Turn_PID(int expectTurn, float actualGyrZ)
{
		return Turn_Kp*expectTurn - Turn_Kd*actualGyrZ;
}

/*  PID控制  */
void Control(void)
{
		//	读取编码器与陀螺仪数据
		Encoder_Left = Read_LeftSpeed();
		Encoder_Right = Read_RightSpeed();
		MPU6050_DMP_Get_Data(&Pitch, &Roll, &Yaw);
		MPU_Get_Gyroscope(&GyrX, &GyrY, &GyrZ);
		//	传入PID控制器，计算电机输出值
		Speed_Out = Speed_PID(expectSpeed, Encoder_Left, Encoder_Right);
		Vertical_Out = Vertical_PID(Speed_Out+BlancedOffset, Roll, GyrX);
		Turn_Out = Turn_PID(expectTurn, GyrZ);
		//	差速输出计算
		PWM_Out = Vertical_Out;
		if(TurnLeft_Flag == 1){
				Turn_Left_Out = PWM_Out + Turn_Out;
				Turn_Right_Out = PWM_Out - Turn_Out;
		}
		else if(TurnRight_Flag == 1){
				Turn_Left_Out = PWM_Out - Turn_Out;
				Turn_Right_Out = PWM_Out + Turn_Out;
		}
		else{
				Turn_Left_Out = PWM_Out;
				Turn_Right_Out = PWM_Out;
		}
		//	安全保护
		if(Roll > 70 || Roll < -70){
				TB6612_Left(0);
				TB6612_Right(0);
		}
		//	输出电机速度
		else{
				TB6612_Left(Turn_Left_Out);
				TB6612_Right(Turn_Right_Out);
		}
}

/*  前进后退停止左转右转函数  */
void CarRun(void)
{
		expectSpeed = 150;
		expectTurn = 0;
		TurnLeft_Flag = 0;
		TurnRight_Flag = 0;
		Turn_Kd = 0;					//恢复Kd约束
}

void CarBackRun(void)
{
		expectSpeed = -100;
		expectTurn = 0;
		TurnLeft_Flag = 0;
		TurnRight_Flag = 0;
		Turn_Kd = 0;					//恢复Kd约束
}

void CarStop(void)
{
		expectSpeed = 0;
		expectTurn = 0;
		TurnLeft_Flag = 0;
		TurnRight_Flag = 0;
		Turn_Kd = 0;					//恢复Kd约束
}

void CarTurnLeft(void)
{
		expectTurn = 10;
		expectSpeed = 100;
		TurnLeft_Flag = 1;
		TurnRight_Flag = 0;
		Turn_Kd = 0;					//转向，取消约束
}

void CarTurnRight(void)
{
		expectTurn = 10;
		expectSpeed = 100;
		TurnLeft_Flag = 0;
		TurnRight_Flag = 1;
		Turn_Kd = 0;					//转向，取消约束
}
