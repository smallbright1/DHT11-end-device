#include "iic.h"

void delay_us(uint us)//延时函数，单位：1us
{
    uint i,j;

    for (i=0; i<us; i++)
        for (j=0; j<1; j++);
}
void IIC_Init(void)//初始化IIC
{
    P1SEL &= ~(1<<6);//1111 1011 第2位清0，0表示普通IO口，1表示复用为片上外设；（普通IO口、输出）
    P1DIR |=  (1<<6);//0000 0100 第2位置1，0表示输入，    1表示输出；
    
    P1SEL &= ~(1<<7);//1111 0111 第3位清0，0表示普通IO口，1表示复用为片上外设；（普通IO口、输出）
    P1DIR |=  (1<<7);//0000 1000 第3位置1，0表示输入，    1表示输出；
    
    IIC_SCL=1;
    IIC_SDA=1;
}
void IIC_Start(void)//产生IIC起始信号
{
    SDA_OUT();     //sda线输出
    IIC_SDA=1;
    IIC_SCL=1;
    delay_us(4);
    IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
    delay_us(4);
    IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
    delay_us(4);/*+++*/
}
void IIC_Stop(void)//产生IIC停止信号
{
    SDA_OUT();//sda线输出
    IIC_SCL=0;
    IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
    delay_us(4);
    IIC_SCL=1; 
    delay_us(1);/*+++*/
    IIC_SDA=1;//发送I2C总线结束信号
    delay_us(4);
}     
uchar IIC_Wait_Ack(void)//等待应答信号到来,返回值：1，接收应答失败;0，接收应答成功
{
    uchar ucErrTime=0;
    SDA_IN();      //SDA设置为输入  
    IIC_SDA=1;delay_us(1);	   
    IIC_SCL=1;delay_us(1);	 
    while(READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_SCL=0;//时钟输出0
    return 0;
}
void IIC_Ack(void)//产生ACK应答
{
    IIC_SCL=0;
    SDA_OUT();
    IIC_SDA=0;
    delay_us(20);/*2->20*/
    IIC_SCL=1;
    delay_us(2);
    IIC_SCL=0;
}
void IIC_NAck(void)//不产生ACK应答	
{
    IIC_SCL=0;
    SDA_OUT();
    IIC_SDA=1;
    delay_us(5);/*2->5*/
    IIC_SCL=1;
    delay_us(5);/*2->5*/
    IIC_SCL=0;
}
void IIC_Send_Byte(uchar txd)//IIC发送一个字节,返回从机有无应答//1,有应答;0,无应答	
{
    uchar t;   
    SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
        delay_us(5);   //对TEA5767这三个延时都是必须的/*2->5*/
        IIC_SCL=1;
        delay_us(5); /*2->5*/
        IIC_SCL=0;	
        delay_us(5);/*2->5*/
    }
}
uchar IIC_Read_Byte(unsigned char ack)//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
{
    unsigned char i,receive=0;
    SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
    {
        IIC_SCL=0; 
        delay_us(5);/*2->5*/
        IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;
        delay_us(5); /*1->5*/
    }
    if(!ack) IIC_NAck();//发送nACK
    else     IIC_Ack(); //发送ACK   
    return receive;
}
