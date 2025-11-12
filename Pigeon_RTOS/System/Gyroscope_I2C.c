#include "stm32f10x.h"                  // Device header
#include "I2C.H"
#include "GPIO_SETInit.h"
#include "Gyroscope_REG.h"

#define MPU6050_Address 0xD0

void MPU6050_WriteReg(uint8_t RegAddress,uint8_t Data)
{
	I2C_Start();
	I2C_dataSend(MPU6050_Address);
	I2C_RcvdAck();
	I2C_dataSend(RegAddress);
	I2C_RcvdAck();
	I2C_dataSend(Data);
	I2C_RcvdAck();
	I2C_Stop();
}

uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	I2C_Start();
	I2C_dataSend(MPU6050_Address);
	I2C_RcvdAck();
	I2C_dataSend(RegAddress);
	I2C_RcvdAck();
	
	uint8_t Data;
	
	I2C_Start();
	I2C_dataSend(MPU6050_Address | 0x01);
	I2C_RcvdAck();
	Data=I2C_dataRcvd();
	I2C_SendAck(1);
	I2C_Stop();
	
	return Data;
}

void MPU6050Init()
{
	GPIO_GyroI2C_Init();
	MPU6050_WriteReg(MPU6050_PWR_1,0x01);
	MPU6050_WriteReg(MPU6050_PWR_2,0x00);
	MPU6050_WriteReg(MPU6050_DIV,0x09);
	MPU6050_WriteReg(MPU6050_CONFIG,0x06);
	MPU6050_WriteReg(MPU6050_G_CONF,0x18);
	MPU6050_WriteReg(MPU6050_A_CONF,0x18);
}

void MPU6050_GetData(int16_t* AX,int16_t* AY,int16_t* AZ,
											int16_t* GX,int16_t* GY,int16_t* GZ)
{
	uint16_t DataH,DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCX_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCX_L);
	*AX = (DataH << 8)|DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCY_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCY_L);
	*AY = (DataH << 8)|DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCZ_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCZ_L);
	*AZ = (DataH << 8)|DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRX_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRX_L);
	*GX = (DataH << 8)|DataL;

	DataH = MPU6050_ReadReg(MPU6050_GYRY_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRY_L);
	*GY = (DataH << 8)|DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRZ_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRZ_L);
	*GZ = (DataH << 8)|DataL;
}
	
