#ifndef _Gyroscope_I2C_H_
#define _Gyroscope_I2C_H_

void MPU6050_WriteReg(uint8_t RegAddress,uint8_t Data);
uint8_t MPU6050_ReadReg(uint8_t RegAddress);

void MPU6050Init(void);
void MPU6050_GetData(int16_t* AX,int16_t* AY,int16_t* AZ,
											int16_t* GX,int16_t* GY,int16_t* GZ);

#endif
