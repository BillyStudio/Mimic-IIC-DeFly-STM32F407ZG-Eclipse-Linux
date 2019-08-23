/*
 * myiic.h
 *
 *  Created on: Aug 23, 2019
 *      Author: shane
 */

#ifndef INC_MYIIC_H_
#define INC_MYIIC_H_
#include "sys.h"

#define SDA_IN()	{GPIOE->MODER &= ~(3<<(5*2)); GPIOE->MODER |= 0<<5*2;}
#define SDA_OUT()	{GPIOE->MODER &= ~(3<<(5*2)); GPIOE->MODER |= 1<<5*2;}

#define IIC_SCL	PEout(10)
#define IIC_SDA PEout(9)
#define READ_SDA PEin(9)

void IIC_Init();
void IIC_Start();
void IIC_Stop();
void IIC_Send_Byte(u8 txd);
u8 IIC_Read_Byte(unsigned char);
u8 IIC_Wait_Ack();
void IIC_Ack();
void IIC_NAck();

void IIC_Write_One_Byte(u8,u8,u8);
u8 IIC_Read_One_Byte(u8,u8);

#endif /* INC_MYIIC_H_ */
