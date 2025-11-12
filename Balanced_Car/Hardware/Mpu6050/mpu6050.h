#ifndef _MPU6050_H_
#define _MPU6050_H_
#include "sys.h"
#include "MPU6050_I2C.h"
#include "inv_mpu.h"

#define INT_Port 			GPIOB
#define INT_Pin 			GPIO_Pin_5
#define INT_EXTIPort 	GPIO_PortSourceGPIOB
#define INT_EXTIPin 	GPIO_PinSource5
#define INT_EXTILine 	EXTI_Line5
#define INT_NVICIRQ 	EXTI9_5_IRQn

#define MPU_SAMPLE_RATE_REG		0X19	//采样频率分频器
#define MPU_CFG_REG				0X1A			//配置寄存器
#define	GYRO_CONFIG		0x1B					//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C					//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F

#define MPU_FIFO_EN_REG			0X23		//FIFO使能寄存器
#define MPU_I2CMST_STA_REG		0X36	//IIC主机状态寄存器
#define MPU_INTBP_CFG_REG		0X37		//中断/旁路设置寄存器
#define MPU_INT_EN_REG			0X38		//中断使能寄存器
#define MPU_INT_STA_REG			0X3A		//中断状态寄存器
#define MPU_USER_CTRL_REG		0X6A		//用户控制寄存器

#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B 
#define	PWR_MGMT_2		0x6C
#define	MPU_DEVICE_ID_REG	  	0x75	
#define	MPU_ADDR	0x68 							//IIC地址寄存器0x68

u8   mpu6050_write(u8 addr, u8 reg, u8 len, u8* buf);//返回值 0：读成功  -1：读失败
u8   mpu6050_read (u8 addr, u8 reg, u8 len, u8 *buf);//返回值 0：读成功  -1：读失败
void mpu6050_write_reg(u8 reg, u8 dat);
u8   mpu6050_read_reg (u8 reg);
/*  MPU6050初始化函数  */
void MPU6050_Init(void);
/*  MPU6050读取温度函数  */
short MPU_Get_Temperature(void);
/*  MPU6050读取角速度值函数  */
u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz);
/*  MPU6050读取加速度值函数  */
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az);
/*  MPU6050读取角度值函数(在inv_mpu中)  */
//	u8 MPU6050_DMP_Get_Data(float *pitch,float *roll,float *yaw);

#endif
