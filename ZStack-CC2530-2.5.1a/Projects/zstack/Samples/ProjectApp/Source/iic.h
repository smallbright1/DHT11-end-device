#ifndef __IIC_H
#define __IIC_H
#include<ioCC2530.h>
#define uchar unsigned char
#define uint  unsigned int

#define SDA_IN()  {P1DIR &= ~(1<<7);}//IO��������
#define SDA_OUT() {P1DIR |=  (1<<7);}

#define IIC_SCL    P1_6
#define IIC_SDA    P1_7
#define READ_SDA   P1_7

void  IIC_Init(void);                  //��ʼ��IIC��IO��				 
void  IIC_Start(void);		       //����IIC��ʼ�ź�
void  IIC_Stop(void);	  	       //����IICֹͣ�ź�
uchar IIC_Wait_Ack(void); 	       //IIC�ȴ�ACK�ź�
void  IIC_Ack(void);		       //IIC����ACK�ź�
void  IIC_NAck(void);		       //IIC������ACK�ź�
void  IIC_Send_Byte(uchar txd);	       //IIC����һ���ֽ�
uchar IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
void delay_us(uint us);

#endif
