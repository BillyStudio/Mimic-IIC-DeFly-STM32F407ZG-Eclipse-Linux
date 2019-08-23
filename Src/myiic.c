#include "myiic.h"
#include "delay.h"

/* myiic.c */
void IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_Initure.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOH, &GPIO_Initure);

    IIC_SDA = 1;
    IIC_SCL = 1;
}

// Start signal
void IIC_Start(void)
{
    SDA_OUT();
    IIC_SDA = 1;
    IIC_SCL = 1;
    delay_us(4);
    IIC_SDA = 0;    // START: when CLK=1, DATA from 1 to 0
    delay_us(4);
    IIC_SCL = 0;    // Lock IIC, prepare to send or receive data;
}

// Stop signal
void IIC_Stop(void)
{
    SDA_OUT();
    IIC_SCL = 0;
    IIC_SDA = 0;
    delay_us(4);
    IIC_SCL = 1;    // STOP: when CLK=1, DATA from 0 to 1
    delay_us(4);
    IIC_SDA = 1;
}

u8 IIC_Wait_Ack(void)
{
    u8 ucErrTime = 0;
    SDA_IN();
    IIC_SDA = 1; delay_us(1);
    IIC_SCL = 1; delay_us(1);
    while (READ_SDA){
        ucErrTime ++;
        if (ucErrTime > 250){
            IIC_Stop();
            return 1;
        }
    }
    IIC_SCL = 0;
    return 0;
}

void IIC_Ack(void)
{
    IIC_SCL = 0;
    SDA_OUT();
    IIC_SDA = 0; delay_us(2);
    IIC_SCL = 1; delay_us(2);
    IIC_SCL = 0;
}

void IIC_NAck(void)
{
    IIC_SCL = 0;
    SDA_OUT();
    IIC_SDA = 1; delay_us(2);
    IIC_SCL = 1; delay_us(2);
    IIC_SCL = 0;
}

void IIC_Send_Byte(u8 txd)
{
    u8 t;
    SDA_OUT();
    IIC_SCL = 0;
    for (t = 0; t < 8; t++){
        IIC_SDA = (txd & 0x80) >> 7;
        txd <<= 1; delay_us(2);
        IIC_SCL = 1; delay_us(2);
        IIC_SCL = 0; delay_us(2);
    }
}

u8 IIC_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    SDA_IN();
    for (i=0; i<8; i++){
        IIC_SCL = 0; delay_us(2);
        IIC_SCL = 1;
        receive <<= 1;
        if (READ_SDA) ++receive;
        delay_us(1);
    }
    if (!ack) IIC_NAck();
    else IIC_Ack();
    return receive;
}



