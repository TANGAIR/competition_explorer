#include "main.h"
/**
  * @brief  �ض���fputc���������ð�����ģʽ��
  * @param  void
  * @retval void
	* @notes  void
  */
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
// ��������
int fputc(int ch, FILE *f)
{
	while (USART_GetFlagStatus(USART6,USART_FLAG_TC) == RESET);//�ȴ�֮ǰ���ַ��������
	USART_SendData(USART6, (uint8_t)ch);
	return (ch);
}
#endif

/**
  * @brief  USART6���ڴ�ӡ��ʼ��
  * @param  void
  * @retval void
  * @notes  USART6_TX-->PG14  USART6_RX-->PG9
  */
void USART6_Init()
{	
  GPIO_InitTypeDef   GPIO_InitStruct;
	USART_InitTypeDef  USART_InitStruct;
	NVIC_InitTypeDef   NVIC_InitStruct;
	//DMA_InitTypeDef   DMA_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
	
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART6);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOG,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_14;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOG,&GPIO_InitStruct);
	
	USART_InitStruct.USART_BaudRate            = 9600;
	USART_InitStruct.USART_WordLength          = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits            = USART_StopBits_1;
	USART_InitStruct.USART_Parity              = USART_Parity_No;
	USART_InitStruct.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART6,&USART_InitStruct);
	
	USART_Cmd(USART6,ENABLE);		
	USART_ITConfig(USART6,USART_IT_RXNE,ENABLE);
	
	NVIC_InitStruct.NVIC_IRQChannel                    = USART6_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority  = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority         = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd                 = ENABLE ;
	NVIC_Init(&NVIC_InitStruct);  
}

/**
  * @brief  USART6���ڴ�ӡ�ж�
  * @param  void
  * @retval void
  * @notes  void
  */
void USART6_IRQHandler(void)
{
  if ( USART_GetITStatus( USART6, USART_IT_RXNE | USART_IT_ORE_RX ) != RESET )
  {
      
  }
  USART_ClearFlag ( USART6,USART_IT_RXNE | USART_IT_ORE_RX );
}

