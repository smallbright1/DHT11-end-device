#include "iic.h"

void delay_us(uint us)//��ʱ��������λ��1us
{
    uint i,j;

    for (i=0; i<us; i++)
        for (j=0; j<1; j++);
}
void IIC_Init(void)//��ʼ��IIC
{
    P1SEL &= ~(1<<6);//1111 1011 ��2λ��0��0��ʾ��ͨIO�ڣ�1��ʾ����ΪƬ�����裻����ͨIO�ڡ������
    P1DIR |=  (1<<6);//0000 0100 ��2λ��1��0��ʾ���룬    1��ʾ�����
    
    P1SEL &= ~(1<<7);//1111 0111 ��3λ��0��0��ʾ��ͨIO�ڣ�1��ʾ����ΪƬ�����裻����ͨIO�ڡ������
    P1DIR |=  (1<<7);//0000 1000 ��3λ��1��0��ʾ���룬    1��ʾ�����
    
    IIC_SCL=1;
    IIC_SDA=1;
}
void IIC_Start(void)//����IIC��ʼ�ź�
{
    SDA_OUT();     //sda�����
    IIC_SDA=1;
    IIC_SCL=1;
    delay_us(4);
    IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
    delay_us(4);
    IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
    delay_us(4);/*+++*/
}
void IIC_Stop(void)//����IICֹͣ�ź�
{
    SDA_OUT();//sda�����
    IIC_SCL=0;
    IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
    delay_us(4);
    IIC_SCL=1; 
    delay_us(1);/*+++*/
    IIC_SDA=1;//����I2C���߽����ź�
    delay_us(4);
}     
uchar IIC_Wait_Ack(void)//�ȴ�Ӧ���źŵ���,����ֵ��1������Ӧ��ʧ��;0������Ӧ��ɹ�
{
    uchar ucErrTime=0;
    SDA_IN();      //SDA����Ϊ����  
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
    IIC_SCL=0;//ʱ�����0
    return 0;
}
void IIC_Ack(void)//����ACKӦ��
{
    IIC_SCL=0;
    SDA_OUT();
    IIC_SDA=0;
    delay_us(20);/*2->20*/
    IIC_SCL=1;
    delay_us(2);
    IIC_SCL=0;
}
void IIC_NAck(void)//������ACKӦ��	
{
    IIC_SCL=0;
    SDA_OUT();
    IIC_SDA=1;
    delay_us(5);/*2->5*/
    IIC_SCL=1;
    delay_us(5);/*2->5*/
    IIC_SCL=0;
}
void IIC_Send_Byte(uchar txd)//IIC����һ���ֽ�,���شӻ�����Ӧ��//1,��Ӧ��;0,��Ӧ��	
{
    uchar t;   
    SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
        delay_us(5);   //��TEA5767��������ʱ���Ǳ����/*2->5*/
        IIC_SCL=1;
        delay_us(5); /*2->5*/
        IIC_SCL=0;	
        delay_us(5);/*2->5*/
    }
}
uchar IIC_Read_Byte(unsigned char ack)//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
{
    unsigned char i,receive=0;
    SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
    {
        IIC_SCL=0; 
        delay_us(5);/*2->5*/
        IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;
        delay_us(5); /*1->5*/
    }
    if(!ack) IIC_NAck();//����nACK
    else     IIC_Ack(); //����ACK   
    return receive;
}
