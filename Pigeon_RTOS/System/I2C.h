#ifndef _I2C_H_
#define _I2C_H_

void I2C_Start(void);
void I2C_Stop(void);
void I2C_dataSend(uint8_t dataSend);
uint16_t I2C_dataRcvd(void);
void I2C_SendAck(uint8_t Ack);
uint8_t I2C_RcvdAck(void);

#endif
