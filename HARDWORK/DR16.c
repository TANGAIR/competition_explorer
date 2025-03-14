/*************************************************************
  *�ļ����ƣ�ң����Դ�ļ�
	
	*���ݶ��壺ң������������ݽṹ�塢
	           ������Ϣ���ݽṹ�塢
	           ң����ԭʼ���ݴ������顢
						 ң�������ݻ����־λ
						 
  *�ⲿ����ʹ�ã���̨������ݽṹ�塢
	               ��̨��ʼ���ݽṹ��
	
	*�������壺ң������ʼ��������
	           ң�������ݽ��뺯����
					   Dma�жϷ�������
					   ��������ת������
	
  *�ⲿ�Զ��庯��ʹ�ã���
  
	*����˵������ң������ȡ���ݽ��н��룬���տ���ģʽ����ת����
	           ���ս��������ݴ��浽������Ϣ���ݽṹ����
						 ȱ�ټ������ģʽ�����ת��
***************************************************************/


/****************ͷ�ļ�������******************************/
#include "main.h"

/****************���ݶ�����********************************/

/*ң�������ݴ���󣬴���ṹ�嶨��*/
DBUS_Data_TypeDef DBUS;
Control_Info_TypeDef Control_Info;

/*ң����ԭʼ���ݴ������鶨��*/
uint8_t DBUS_buffer[18];

/*��ر�־λ����*/
short DR16_Flash_Val=0;


/****************����������******************************/
/*
  *�������ƣ�ң������ʼ������
  *�������룺��
  *����ֵ��  ��
  *Ӳ����Ϣ��DR16  USART1_RX-->PB7    DMA����
  *����˵������ң���������ô���1�����Լ�DMAֱ���ڴ���ʳ�ʼ��,����dma2�жϣ�
	           ����DMA2���ݴ�������
	*/
void DR16_Init(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;
	USART_InitTypeDef  USART_InitStructure;
	DMA_InitTypeDef    DMA_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	/* -------------- Enable Module Clock Source ----------------------------*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_DMA2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
   
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_USART1);
	
	/* -------------- Configure GPIO ---------------------------------------*/
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	/* -------------- Configure USART ---------------------------------------*/
	USART_DeInit(USART1);
	USART_InitStructure.USART_BaudRate           =100000;//70Kbps   ң��������Э��V1.4.pdf
	USART_InitStructure.USART_WordLength         =USART_WordLength_8b;
	USART_InitStructure.USART_StopBits           =USART_StopBits_1;
	USART_InitStructure.USART_Parity             =USART_Parity_Even;
	USART_InitStructure.USART_Mode               =USART_Mode_Rx;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_Init(USART1,&USART_InitStructure);
	
	USART_Cmd(USART1,ENABLE);
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
	
	/* -------------- Configure NVIC ---------------------------------------*/
	NVIC_InitStructure.NVIC_IRQChannel                   =DMA2_Stream5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        =2;
	NVIC_InitStructure.NVIC_IRQChannelCmd                =ENABLE ;
	NVIC_Init(&NVIC_InitStructure);
	
	/* -------------- Configure DMA ---------------------------------------*/
  DMA_DeInit(DMA2_Stream5);                //ΪDMA����ͨ��
	DMA_InitStructure.DMA_Channel           =DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr=(uint32_t)&(	USART1->DR);           //��ʼ��ַ
	DMA_InitStructure.DMA_Memory0BaseAddr   =(uint32_t)DBUS_buffer;              //�洢����
	DMA_InitStructure.DMA_DIR               =DMA_DIR_PeripheralToMemory;         //���䷽��
	DMA_InitStructure.DMA_BufferSize        =DBUS_DATE_LENGTH+DBUS_SAFTY_LENGTH; //����������
	DMA_InitStructure.DMA_PeripheralInc     =DMA_PeripheralInc_Disable;          //�������ģʽ
	DMA_InitStructure.DMA_MemoryInc         =DMA_MemoryInc_Enable;               //�ڴ����ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;        //DMA����ʱÿ�β��������ݳ���
	DMA_InitStructure.DMA_MemoryDataSize    =DMA_PeripheralDataSize_Byte;        //DMA����ʱÿ�β��������ݳ���
	DMA_InitStructure.DMA_Mode              =DMA_Mode_Circular;                  //����ģʽ����������
	DMA_InitStructure.DMA_Priority          =DMA_Priority_VeryHigh;              //���ȼ���
  DMA_InitStructure.DMA_FIFOMode          =DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold     =DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst       =DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst   =DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream5,&DMA_InitStructure);
	
	DMA_ITConfig(DMA2_Stream5,DMA_IT_TC,ENABLE);
	DMA_Cmd(DMA2_Stream5,ENABLE);	
}



/*
  *�������ƣ�ң�������ݽ��뺯��
  *�������룺��
  *����ֵ��  ��
  *Ӳ����Ϣ����
  *����˵��������ң�����ж�ȡ��18���ֽڵ����ݽ��н��룬���浽ң�������ݽṹ����
             ����ʱch0��ch2����Ϊ��,ch1��ch3����Ϊ������ң����Э��涨�ķ����෴��,
             ���X������Ϊ��,Y������Ϊ��
	*/
void DBUS_Data_Decode(void)
{
	
	//ң����
	DBUS.RC.ch0          = ((( DBUS_buffer[0] | (DBUS_buffer [1]<<8) ) & 0x07FF)-1024);                       //channel-0
	DBUS.RC.ch1          = ((( DBUS_buffer[1]>>3 | DBUS_buffer[2]<<5 ) & 0x07FF)-1024);                       //channel-1
	DBUS.RC.ch2          = ((( DBUS_buffer[2]>>6 | DBUS_buffer[3]<<2 | DBUS_buffer[4]<<10 ) & 0x07FF)-1024);  //channel-2
	DBUS.RC.ch3          = (( DBUS_buffer[4]>>1 | DBUS_buffer[5]<<7 ) & 0x07FF)-1024;                         //channel-3	
	DBUS.RC.Switch_Left  = (( DBUS_buffer[5]>>4 ) & 0x00C )>>2;                                               //Switch_Left
	DBUS.RC.Switch_Right = ( DBUS_buffer[5]>>4 ) & 0x003;                                                     //Switch_Right
	if(abs(DBUS.RC.ch0)<10)  DBUS.RC.ch0=0;
	if(abs(DBUS.RC.ch1)<10)  DBUS.RC.ch1=0;
	if(abs(DBUS.RC.ch2)<10)  DBUS.RC.ch2=0;
	if(abs(DBUS.RC.ch3)<10)  DBUS.RC.ch3=0;
	
	
	//���
	DBUS.PC.X_Real[5]    = DBUS_buffer[6] | (DBUS_buffer[7] << 8);                                       
	DBUS.PC.X=0;
	for(short k=0;k<5;k++)//ȡ��ȥ5�ε�ƽ��ֵ
	{
		DBUS.PC.X_Real[k]=DBUS.PC.X_Real[k+1];
		DBUS.PC.X+=DBUS.PC.X_Real[k];
	}
	DBUS.PC.X/=5;
	if(DBUS.PC.X>50) DBUS.PC.X=50;
	if(DBUS.PC.X<-50) DBUS.PC.X=-50;
	
	DBUS.PC.Y_Real[5]       = DBUS_buffer[8] | (DBUS_buffer[9] << 8);   
	DBUS.PC.Y=0;
	for(short k=0;k<5;k++)
	{
		DBUS.PC.Y_Real[k]=DBUS.PC.Y_Real[k+1];
		DBUS.PC.Y+=DBUS.PC.Y_Real[k];
	}
	DBUS.PC.Y/=5;
	if(DBUS.PC.Y>50) DBUS.PC.Y=50;
	if(DBUS.PC.Y<-50) DBUS.PC.Y=-50;
 
	DBUS.PC.Press_Left   = DBUS_buffer[12];  
	DBUS.PC.Press_Right  = DBUS_buffer[13];  
	
	
	//����
	DBUS.PC.Keyboard     = DBUS_buffer[14] | DBUS_buffer[15] << 8;                             
}




/*
  *�������ƣ�ң����DMA�жϷ�����
  *�������룺��
  *����ֵ��  ��
  *Ӳ����Ϣ����
  *����˵����ÿ�� 14ms ͨ�� DBUS ����һ֡���ݣ�18 �ֽڣ���ͬʱ�ڴ��жϷ�������
             ��ң�������ݽ��г�������
	*/
void DMA2_Stream5_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA2_Stream5,DMA_IT_TCIF5))
	{
		DBUS_Data_Decode();
    DR16_Flash_Val=DR_DECENT_TIM;	
		DMA_ClearITPendingBit(DMA2_Stream5,DMA_IT_TCIF5);
		DMA_ClearFlag(DMA2_Stream5,DMA_IT_TCIF5);
	}
}



/*
  *�������ƣ���������ת������
  *�������룺��
  *����ֵ��  ��
  *Ӳ����Ϣ����
  *����˵������֮ǰ�����õ�ң��������ת���ɿ�����Ϣ��
             ����ң�����������������������ת��ģʽ
	*/
void Control_Data_Process(void)
{
 if(DR16_Flash_Val>0)
 {
		DR16_Flash_Val--;
		/*ң����ģʽ*/
		if(DBUS.RC.Switch_Right != RC_SW_DOWN) //ң��������ģʽ
		{
			//ң������ֵ
			if(DBUS.RC.Switch_Right == RC_SW_UP)
			{
				 //���̵��		
				 Control_Info.Chassis.Direction_X=-DBUS.RC.ch0/3;
				 Control_Info.Chassis.Direction_Y=DBUS.RC.ch1/3;
				
				//��̨���
				 Control_Info.PTZ.Yaw=-DBUS.RC.ch2/2;
				 Control_Info.PTZ.Pitch=DBUS.RC.ch3/2;
			}
			 //  Control_Info.Chassis.Direction_Omega=((Yaw_Motor_205.Real_MechanicalAngle-PTZ_Init_Value.Initial_Yaw_Angle)/2);
				 if(abs(Control_Info.Chassis.Direction_Omega)<5)  Control_Info.Chassis.Direction_Omega =0;
			//���ǵ��
			if(DBUS.RC.Switch_Right == RC_SW_MID)
			{
			 Control_Info.Flip.Aim_Flip=-DBUS.RC.ch0/5;	
			}
		}

		/*����ģʽ*/
		else if(DBUS.RC.Switch_Right == RC_SW_DOWN)
		{
			 //����
		}
 }
 else
 {
	 Control_Info.Chassis.Direction_X=0;
   Control_Info.Chassis.Direction_Y=0;
	 Control_Info.Chassis.Direction_Omega=0;
	 Control_Info.PTZ.Yaw=0;
	 Control_Info.PTZ.Pitch=0;
 }

}

















