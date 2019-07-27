#ifndef __IIC_H
#define __IIC_H
#include<ioCC2530.h>
#define uchar unsigned char
#define uint  unsigned int

#define SDA_IN()  {P1DIR &= ~(1<<7);}//IO方向设置
#define SDA_OUT() {P1DIR |=  (1<<7);}

#define IIC_SCL    P1_6
#define IIC_SDA    P1_7
#define READ_SDA   P1_7

void  IIC_Init(void);                  //初始化IIC的IO口				 
void  IIC_Start(void);		       //发送IIC开始信号
void  IIC_Stop(void);	  	       //发送IIC停止信号
uchar IIC_Wait_Ack(void); 	       //IIC等待ACK信号
void  IIC_Ack(void);		       //IIC发送ACK信号
void  IIC_NAck(void);		       //IIC不发送ACK信号
void  IIC_Send_Byte(uchar txd);	       //IIC发送一个字节
uchar IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
void delay_us(uint us);

#endif
