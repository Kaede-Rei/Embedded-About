#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void I2C_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)BitValue);
	Delay_us(10);
}

void I2C_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_11,(BitAction)BitValue);
	Delay_us(10);
}

uint8_t I2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);
	Delay_us(10);
	return BitValue;
}


void I2C_Start(void)
{
	I2C_W_SDA(1);
	I2C_W_SCL(1);
	I2C_W_SDA(0);
	I2C_W_SCL(0);
}

void I2C_Stop(void)
{
	I2C_W_SDA(0);
	I2C_W_SCL(1);
	I2C_W_SDA(1);
}

void I2C_dataSend(uint8_t dataSend)
{
	for(uint8_t i=0;i<8;i++)
	{
		I2C_W_SDA( dataSend & (0x80 >> i) );
		I2C_W_SCL(1);
		I2C_W_SCL(0);
	}
}

uint8_t I2C_dataRcvd(void)
{
	uint8_t dataRcvd=0x00;	//0000 0000
	I2C_W_SDA(1);
	for(uint8_t i=0;i<8;i++)
	{
		I2C_W_SCL(1);
		if(I2C_R_SDA() == 1){dataRcvd |= (0x80>>i);}
		I2C_W_SCL(0);
	}
	return dataRcvd;
}

void I2C_SendAck(uint8_t Ack)
{
	I2C_W_SDA(Ack);
	I2C_W_SCL(1);
	I2C_W_SCL(0);
}

uint16_t I2C_RcvdAck(void)
{
	uint16_t Ack;
	I2C_W_SDA(1);
	I2C_W_SCL(1);
	Ack=I2C_R_SDA();
	I2C_W_SCL(0);
	return Ack;
}
