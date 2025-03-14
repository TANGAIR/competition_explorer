/*************************************************************
  *�ļ����ƣ�CAN���蹦��Դ�ļ�
	
	*���ݶ��壺���̡���̨��Ħ���֡����̵�ʹ��canͨѶ���Ƶĵ��
						 ��can���ݶ��塣
						 CAN1���ݷ��ͼ���־λ���塣
	
  *�ⲿ����ʹ�ã�������ͷ�ļ��к궨���ID��
	
	*�������壺CAN1��ʼ��������
	           CAN1���ݷ��ͺ�����
						 CAN1�����жϷ�������CAN1�����жϷ�����
	
  *�ⲿ�Զ��庯��ʹ�ã���
  
	*����˵�������can����������ؽṹ���ʼ���������ֹ��ܺ����Ķ���
	           ͨ��can������ֵ���͸��������,��ͨ�������жϽ��������
						 ��ֵ�洢�����can���ݽṹ���Ա�С�
						 ͨ����can1���ݷ��ͼ���־Ϊ�ļ�Ᵽ֤���ݶ����ͳ�ȥ
***************************************************************/


/****************ͷ�ļ�������******************************/
#include "main.h"
/****************���ݶ�����********************************/
/*���CAN���ݽṹ�������*/
//CAN1
//���̵���ĸ�M2006
CAN_Data_TypeDef  CAN_DATA_Left,  //201����
								
									CAN_DATA_Right;  //203����
								  

									
/*��ر�־λ����*/
char CAN1_Tx_Message_Flag;//


/****************����������******************************/
/*
  *�������ƣ�CAN1��ʼ������
  *�������룺��
  *����ֵ��  ��
  *Ӳ����Ϣ��STM32F427II  CAN1_RX(PD0)    CAN1_TX(PD1) 
  *����˵������can1��ʼ��,���÷��ͺͽ����ж����ȼ�
*/
void CAN1_Init(void)
{
	GPIO_InitTypeDef      GPIO_InitStruct;//����gpIo��ʼ���ṹ�����
	NVIC_InitTypeDef      NVIC_InitStruct;//����nvIc��ʼ���ṹ�����
	CAN_InitTypeDef       CAN_InitStruct;//����can��ʼ���ṹ��
	CAN_FilterInitTypeDef CAN_FilterInitStruct;//����cam��������ʼ���ṹ��
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//����gpIo����ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//����canʱ��
	

	//GpIo��ʼ���ṹ���Ա��ֵPd 0.
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_0 ;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;               //�������
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOD,&GPIO_InitStruct);
	
	//GpIo��ʼ���ṹ���Ա��ֵpd1
	GPIO_InitStruct.GPIO_Pin   =  GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOD,&GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);       //gpiod0 ӳ�����
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);      //gpio d1 ӳ�䷢��
	/*ϵͳ���õ��ж����ȼ�����λ2��  ��ռ��ֵԽС�����ȼ�Խ��*/	
	//����can1���տ��ж�	
	NVIC_InitStruct.NVIC_IRQChannel                    = CAN1_RX0_IRQn;             
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority  = 1;               //2��ռ
	NVIC_InitStruct.NVIC_IRQChannelSubPriority         = 1;                //0��Ӧ
	NVIC_InitStruct.NVIC_IRQChannelCmd                 = ENABLE ;
	NVIC_Init(&NVIC_InitStruct);
	//����can1���Ϳ��ж�
  NVIC_InitStruct.NVIC_IRQChannel                    = CAN1_TX_IRQn;              //  ���÷����ж�             ע�����ж�����������  refer to stm32f4xx.h file
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority  = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority         = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd                 = ENABLE ;
	NVIC_Init(&NVIC_InitStruct);
 
 //�������can�ڽ��и�λ 
  CAN_DeInit(CAN1);                         //��λcan1����Ȼ�ͻ�һֱ��λ��ѭ��
  CAN_StructInit(&CAN_InitStruct);          //����can��λ����

// Can��ʼ���ṹ���Ա��ֵ��Can�Ĵ����ٶ�Ϊ1Mbÿ��  
	CAN_InitStruct.CAN_Prescaler = 3;                                               //CAN BaudRate 42/(1+9+4)/3=1Mbps  //��Ƶϵ��(Fdiv)Ϊ3+1
	CAN_InitStruct.CAN_Mode      = CAN_Mode_Normal;                                 //ģʽ���� ����ģʽ
	CAN_InitStruct.CAN_SJW       = CAN_SJW_1tq;                                     //����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1tq~CAN_SJW_4tq
	CAN_InitStruct.CAN_BS1       = CAN_BS1_9tq;                                     //Tbs1��ΧCAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStruct.CAN_BS2       = CAN_BS2_4tq;                                     //Tbs2��ΧCAN_BS2_1tq ~	CAN_BS2_8tq
	CAN_InitStruct.CAN_TTCM      = DISABLE;                                         //��ʱ�䴥��ͨ��ģʽ
	CAN_InitStruct.CAN_ABOM      = DISABLE;                                         //����Զ����߹���	  
	CAN_InitStruct.CAN_AWUM      = DISABLE;                                         //˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
	CAN_InitStruct.CAN_NART      = DISABLE;	                                        //�������Զ�����
	CAN_InitStruct.CAN_RFLM      = DISABLE;                                         //���Ĳ�����,�µĸ��Ǿɵ�
	CAN_InitStruct.CAN_TXFP      = ENABLE;	                                        //���ȼ����ɱ��ı�ʶ����������
	CAN_Init(CAN1,&CAN_InitStruct);

//���ý��չ�����,�������κι��˲���

	CAN_FilterInitStruct.CAN_FilterIdHigh         = 0x0000;                         //32λID
	CAN_FilterInitStruct.CAN_FilterIdLow          = 0x0000;                         //32λID
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh     = 0x0000;
	CAN_FilterInitStruct.CAN_FilterMaskIdLow      = 0x0000;                         //�������κ�ID
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;               //������0������FIFO0;
	CAN_FilterInitStruct.CAN_FilterNumber         = 0;                              //������0,��Χ0---13
	CAN_FilterInitStruct.CAN_FilterMode           = CAN_FilterMode_IdMask;           //����ģʽ
	CAN_FilterInitStruct.CAN_FilterScale          = CAN_FilterScale_32bit;
	CAN_FilterInitStruct.CAN_FilterActivation     = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStruct);
	
  CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);                                          //FIFO0��Ϣ�Һ��ж������򿪽����ж�
	CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE);                                           //�򿪷����ж�


}


/*
  *�������ƣ�����ٶ�CAN1���ͺ���
  *�������룺��
  *����ֵ��  ��
  *Ӳ����Ϣ��STM32F427II  CAN1_RX(PD0)    CAN1_TX(PD1) 
  *����˵����������pid�����ĵ���ֵͨ��CAN1���͸����
*/
void CAN1_TX(void)
{
	CanTxMsg CAN1_Tx_Message;

	CAN1_Tx_Message.IDE = CAN_ID_STD;                                               //��׼֡
	CAN1_Tx_Message.RTR = CAN_RTR_DATA;                                           	//����֡
	CAN1_Tx_Message.DLC = 8;                                                        //֡����Ϊ8
	CAN1_Tx_Message.StdId = CAN_201_204_ID;                                         //֡IDΪ������CAN_ID

	CAN1_Tx_Message.Data[0] = (CAN_DATA_Left.Target_Speed>>8)&0xff;              //201���յ�����8λ
	CAN1_Tx_Message.Data[1] = (CAN_DATA_Left.Target_Speed)&0xff;                 //201���յ�����8λ
	CAN1_Tx_Message.Data[2] = 0;            //202���յ�����8λ
	CAN1_Tx_Message.Data[3] = 0;                  //202���յ�����8λ
	CAN1_Tx_Message.Data[4] = (CAN_DATA_Right.Target_Speed>>8)&0xff;              //203���յ�����8λ
	CAN1_Tx_Message.Data[5] = (CAN_DATA_Right.Target_Speed)&0xff;                 //203���յ�����8λ
	CAN1_Tx_Message.Data[6] = 0;             //204���յ�����8λ
	CAN1_Tx_Message.Data[7] = 0;                //204���յ�����8λ

	CAN1_Tx_Message_Flag = 0;
	CAN_Transmit(CAN1,&CAN1_Tx_Message);

	while(CAN1_Tx_Message_Flag == 0)
	{
   /*��֤����˳������*/
	}
}

/*
  *�������ƣ�CAN1�����жϷ�����
  *�������룺��
  *����ֵ��  ��
  *Ӳ����Ϣ���� 
  *����˵��������Ϣ����֮�󣬻����һ���жϣ������Ӧ�ı�־λ��һ˵���жϷ��ͳɹ�
             ����ʱͨ������������������Ϣ��־λд1�� Can1���ͺ����Ͳ��Ῠ��whileѭ����
             ��������������ĺ���һ�𹹳�����Ϣ����ϵͳ
*/
void CAN1_TX_IRQHandler(void)
{
  if (CAN_GetITStatus (CAN1,CAN_IT_TME)!=RESET)                       
	{
		CAN1_Tx_Message_Flag=1;
		CAN_ClearITPendingBit(CAN1,CAN_IT_TME);    
	}
}



/*
  *�������ƣ�CAN1�����жϷ�����
  *�������룺��
  *����ֵ��  ��
  *Ӳ����Ϣ���� 
  *����˵������canͨѶ���յ����ݴ��浽���can���ݽṹ���У�����can���ݽṹ���Ա����
             short���͵ģ���Ϊ����ͨ��Э��ң�������ٶȺͽǶ�ֵ�ֳ��������ֽڷ��ͣ�
             �ڽ���ʱ�������ֽڵ�����ƴ�����������浽�������;��ܹ�ʶ�����ݵ�����
*/
void CAN1_RX0_IRQHandler(void)                                                  //�ж�������ο���startup_stm32f40_41xxx.s��
{   
	CanRxMsg CAN1_Rx_Message;	
	if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
	{
		CAN_Receive(CAN1,CAN_FIFO0,&CAN1_Rx_Message);
		if ( (CAN1_Rx_Message.IDE == CAN_Id_Standard) && (CAN1_Rx_Message.RTR == CAN_RTR_Data) && (CAN1_Rx_Message.DLC == 8) )//��׼֡������֡�����ݳ���Ϊ8�ֽ�
		{

			switch (CAN1_Rx_Message.StdId)
			{
				/*���̵��*/
				case CAN1_CHASSIS_ID_Left:  //��ǰ-->0x201
				{
					CAN_DATA_Left.Real_Speed  =(CAN1_Rx_Message.Data[2]<<8)|(CAN1_Rx_Message.Data[3]);              //ת��
				break;
				}
			
				case CAN1_CHASSIS_ID_Right:  //�Һ�-->0x203
				{
					CAN_DATA_Right.Real_Speed  =(CAN1_Rx_Message.Data[2]<<8)|(CAN1_Rx_Message.Data[3]);              //ת��
					break;
				}
				
				/*��̨���*/
				/*Ħ���ֵ��*/
				/*�����̵��*/
				/**/
        default:
				{
					break;
				}   
			}
		}
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);		
	}
}

