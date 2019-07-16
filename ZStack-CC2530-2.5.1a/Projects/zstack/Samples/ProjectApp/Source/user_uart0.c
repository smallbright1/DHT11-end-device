#include "user_uart0.h"
#include "hal_uart.h"
#include "string.h"
//#include "user_printf.h"
#include "AF.h"
#include "ProjectApp.h"

uint8 UART0_RX_BUFF[UART0_RX_BUFF_MAX];//���ջ�����
uint8 UART0_RX_STA = 0;                //����״̬���
uint8 UART0_RX_LEN = 0;                //�������ݳ���

void USER_Uart0_Init( uint8 baudRate )
{
  halUARTCfg_t uartConfig;

  /* UART Configuration */
  uartConfig.configured           = TRUE;
  uartConfig.baudRate             = baudRate;
  uartConfig.flowControl          = USER_UART_DEFAULT_OVERFLOW;
  uartConfig.flowControlThreshold = USER_UART_DEFAULT_THRESHOLD;
  uartConfig.rx.maxBufSize        = USER_UART_DEFAULT_MAX_RX_BUFF;
  uartConfig.tx.maxBufSize        = USER_UART_DEFAULT_MAX_TX_BUFF;
  uartConfig.idleTimeout          = USER_UART_DEFAULT_IDLE_TIMEOUT;
  uartConfig.intEnable            = TRUE;
  uartConfig.callBackFunc         = Uart0_Process;

  /* Start UART */
  HalUARTOpen (USER_UART_DEFAULT_PORT, &uartConfig);
}

void Uart0_Process( uint8 port, uint8 event )
{
  uint8 Res;
  (void)event;  // Intentionally unreferenced parameter
 
  while (Hal_UART_RxBufLen(port))
  {
    HalUARTRead (port, &Res, 1);//��ȡһ���ֽ�
    UART0_RX_BUFF[UART0_RX_STA&0x3F]=Res ;
    UART0_RX_STA++;
 
    if((UART0_RX_STA&0x80)==0)//����δ���
    {
      if(UART0_RX_STA&0x40)//ǰһ�����յ������ݽ������ġ�ǰһ�롱
      {
        if     (Res==0x0A)UART0_RX_STA |= 0x80;//���������
        else if(Res!=0x0D)UART0_RX_STA &= 0xBF;//���ǽ���������ǰ�����������־��0
      }
      else
      {
        if(Res==0x0D)UART0_RX_STA |= 0x40;
      }
    }
 
    if((UART0_RX_STA&0x80)==0x80)
    {
      UART0_RX_LEN = (UART0_RX_STA&0x3F)-2;
      Uart0_Handle();
      Uart0_Reset();
      break;
    }
  }
}
uint8 DestMacR2[8]={0x9C,0X7C,0XDD,0X18,0X00,0X4B,0X12,0X00};

void Uart0_Handle(void)
{
  if(strstr((const char*)UART0_RX_BUFF, "turn on fan"))
  {
      afAddrType_t dstAddr;
      dstAddr.addrMode = (afAddrMode_t)Addr64Bit;
      dstAddr.endPoint = PROJECTAPP_ENDPOINT;
      memcpy(dstAddr.addr.extAddr,DestMacR2,8);
      
      AF_DataRequest( &dstAddr,
                  &ProjectApp_epDesc,
                  PROJECTAPP_P2P_CLUSTERID,
                  strlen("turn on fan"),
                  "turn on fan",
                  &ProjectApp_TransID,
                  AF_DISCV_ROUTE,
                  AF_DEFAULT_RADIUS );
  }
  else if(strstr((const char*)UART0_RX_BUFF,"turn off fan"))
  {
     afAddrType_t dstAddr;
      dstAddr.addrMode = (afAddrMode_t)Addr64Bit;
      dstAddr.endPoint = PROJECTAPP_ENDPOINT;
      memcpy(dstAddr.addr.extAddr,DestMacR2,8);
      
      AF_DataRequest( &dstAddr,
                  &ProjectApp_epDesc,
                  PROJECTAPP_P2P_CLUSTERID,
                  strlen("turn off fan"),
                  "turn off fan",
                  &ProjectApp_TransID,
                  AF_DISCV_ROUTE,
                  AF_DEFAULT_RADIUS );
  }
    else if(strstr((const char*)UART0_RX_BUFF,"turn on beep"))
  {
     afAddrType_t dstAddr;
      dstAddr.addrMode = (afAddrMode_t)Addr64Bit;
      dstAddr.endPoint = PROJECTAPP_ENDPOINT;
      memcpy(dstAddr.addr.extAddr,DestMacR2,8);
      
      AF_DataRequest( &dstAddr,
                  &ProjectApp_epDesc,
                  PROJECTAPP_P2P_CLUSTERID,
                  strlen("turn on beep"),
                  "turn on beep",
                  &ProjectApp_TransID,
                  AF_DISCV_ROUTE,
                  AF_DEFAULT_RADIUS );
  }
    else if(strstr((const char*)UART0_RX_BUFF,"turn off beep"))
  {
     afAddrType_t dstAddr;
      dstAddr.addrMode = (afAddrMode_t)Addr64Bit;
      dstAddr.endPoint = PROJECTAPP_ENDPOINT;
      memcpy(dstAddr.addr.extAddr,DestMacR2,8);
      
      AF_DataRequest( &dstAddr,
                  &ProjectApp_epDesc,
                  PROJECTAPP_P2P_CLUSTERID,
                  strlen("turn off beep"),
                  "turn off beep",
                  &ProjectApp_TransID,
                  AF_DISCV_ROUTE,
                  AF_DEFAULT_RADIUS );
  }
}

void Uart0_Reset(void)
{
  memset(UART0_RX_BUFF,0,UART0_RX_BUFF_MAX);
  UART0_RX_STA = 0;
}
