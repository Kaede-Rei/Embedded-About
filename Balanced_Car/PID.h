#ifndef _PID_H_
#define _PID_H_

#include "stm32f10x.h"                  // Device header
#include "Encoder.h"
#include "Mpu6050.h"
#include "TB6612.H"

/*
 *	直立环PD控制器
 *	参数：目标(期望)角度， 反馈(真实)角度， 反馈(真实)角速度 
 *	返回值：输出速度
 */
int Vertical_PID(float expectAngle, float actualAngle, float actualGyrX);
/*
 *	速度环PI控制器
 *	参数：目标(期望)速度， 反馈(真实)左编码器速度， 反馈(真实)右编码器速度
 *	返回值：输出角度
 */
int Speed_PID(int expectSpeed, int actualLeftSpeed, int actualRightSpeed);
/*
 *	转向环PD控制器
 *	参数：转向角度(误差)， 反馈(真实)角速度 
 *	返回值：输出角速度
 */
int Turn_PID(int expectTurn, float actualGyrZ);
/*  PID控制器  */
void Control(void);
/*  前进函数  */
void CarRun(void);
/*  后退函数  */
void CarBackRun(void);
/*  停止函数  */
void CarStop(void);
/*  左转函数  */
void CarTurnLeft(void);
/*  右转函数  */
void CarTurnRight(void);

#endif
