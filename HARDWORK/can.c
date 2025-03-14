/*************************************************************
  *文件名称：CAN外设功能源文件
	
	*数据定义：底盘、云台、摩擦轮、拨盘等使用can通讯控制的电机
						 的can数据定义。
						 CAN1数据发送检测标志位定义。
	
  *外部数据使用：主函数头文件中宏定义的ID、
	
	*函数定义：CAN1初始化函数、
	           CAN1数据发送函数、
						 CAN1发送中断服务函数、CAN1接收中断服务函数
	
  *外部自定义函数使用：无
  
	*功能说明：完成can外设所有相关结构体初始化，各种种功能函数的定义
	           通过can将电流值发送给各个电机,并通过接收中断将电机返回
						 的值存储到点击can数据结构体成员中。
						 通过对can1数据发送检测标志为的检测保证数据都发送出去
***************************************************************/


/****************头文件包含区******************************/
#include "main.h"
/****************数据定义区********************************/
/*电机CAN数据结构体对象定义*/
//CAN1
//底盘电机四个M2006
CAN_Data_TypeDef  CAN_DATA_Left,  //201左轮
								
									CAN_DATA_Right;  //203右轮
								  

									
/*相关标志位定义*/
char CAN1_Tx_Message_Flag;//


/****************函数功能区******************************/
/*
  *函数名称：CAN1初始化函数
  *参数输入：无
  *返回值：  无
  *硬件信息：STM32F427II  CAN1_RX(PD0)    CAN1_TX(PD1) 
  *功能说明：将can1初始化,设置发送和接收中断优先级
*/
void CAN1_Init(void)
{
	GPIO_InitTypeDef      GPIO_InitStruct;//定义gpIo初始化结构体变量
	NVIC_InitTypeDef      NVIC_InitStruct;//定义nvIc初始化结构体变量
	CAN_InitTypeDef       CAN_InitStruct;//定义can初始化结构体
	CAN_FilterInitTypeDef CAN_FilterInitStruct;//定义cam过滤器初始化结构体
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//开启gpIo总线时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//开启can时钟
	

	//GpIo初始化结构体成员赋值Pd 0.
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_0 ;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;               //推挽输出
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOD,&GPIO_InitStruct);
	
	//GpIo初始化结构体成员赋值pd1
	GPIO_InitStruct.GPIO_Pin   =  GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOD,&GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);       //gpiod0 映射接收
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);      //gpio d1 映射发送
	/*系统设置的中断优先级分组位2，  抢占数值越小，优先级越高*/	
	//设置can1接收口中断	
	NVIC_InitStruct.NVIC_IRQChannel                    = CAN1_RX0_IRQn;             
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority  = 1;               //2抢占
	NVIC_InitStruct.NVIC_IRQChannelSubPriority         = 1;                //0响应
	NVIC_InitStruct.NVIC_IRQChannelCmd                 = ENABLE ;
	NVIC_Init(&NVIC_InitStruct);
	//设置can1发送口中断
  NVIC_InitStruct.NVIC_IRQChannel                    = CAN1_TX_IRQn;              //  设置发送中断             注意与中断向量表区别  refer to stm32f4xx.h file
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority  = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority         = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd                 = ENABLE ;
	NVIC_Init(&NVIC_InitStruct);
 
 //在这里对can口进行复位 
  CAN_DeInit(CAN1);                         //复位can1，不然就会一直复位，循环
  CAN_StructInit(&CAN_InitStruct);          //调用can复位函数

// Can初始化结构体成员赋值，Can的传输速度为1Mb每秒  
	CAN_InitStruct.CAN_Prescaler = 3;                                               //CAN BaudRate 42/(1+9+4)/3=1Mbps  //分频系数(Fdiv)为3+1
	CAN_InitStruct.CAN_Mode      = CAN_Mode_Normal;                                 //模式设置 常规模式
	CAN_InitStruct.CAN_SJW       = CAN_SJW_1tq;                                     //重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
	CAN_InitStruct.CAN_BS1       = CAN_BS1_9tq;                                     //Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStruct.CAN_BS2       = CAN_BS2_4tq;                                     //Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
	CAN_InitStruct.CAN_TTCM      = DISABLE;                                         //非时间触发通信模式
	CAN_InitStruct.CAN_ABOM      = DISABLE;                                         //软件自动离线管理	  
	CAN_InitStruct.CAN_AWUM      = DISABLE;                                         //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
	CAN_InitStruct.CAN_NART      = DISABLE;	                                        //允许报文自动发送
	CAN_InitStruct.CAN_RFLM      = DISABLE;                                         //报文不锁定,新的覆盖旧的
	CAN_InitStruct.CAN_TXFP      = ENABLE;	                                        //优先级不由报文标识符决定？？
	CAN_Init(CAN1,&CAN_InitStruct);

//配置接收过滤器,不进行任何过滤操作

	CAN_FilterInitStruct.CAN_FilterIdHigh         = 0x0000;                         //32位ID
	CAN_FilterInitStruct.CAN_FilterIdLow          = 0x0000;                         //32位ID
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh     = 0x0000;
	CAN_FilterInitStruct.CAN_FilterMaskIdLow      = 0x0000;                         //不过滤任何ID
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;               //过滤器0关联到FIFO0;
	CAN_FilterInitStruct.CAN_FilterNumber         = 0;                              //过滤器0,范围0---13
	CAN_FilterInitStruct.CAN_FilterMode           = CAN_FilterMode_IdMask;           //屏蔽模式
	CAN_FilterInitStruct.CAN_FilterScale          = CAN_FilterScale_32bit;
	CAN_FilterInitStruct.CAN_FilterActivation     = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStruct);
	
  CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);                                          //FIFO0消息挂号中断允许，打开接收中断
	CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE);                                           //打开发送中断


}


/*
  *函数名称：电机速度CAN1发送函数
  *参数输入：无
  *返回值：  无
  *硬件信息：STM32F427II  CAN1_RX(PD0)    CAN1_TX(PD1) 
  *功能说明：将经过pid计算后的电流值通过CAN1发送给电机
*/
void CAN1_TX(void)
{
	CanTxMsg CAN1_Tx_Message;

	CAN1_Tx_Message.IDE = CAN_ID_STD;                                               //标准帧
	CAN1_Tx_Message.RTR = CAN_RTR_DATA;                                           	//数据帧
	CAN1_Tx_Message.DLC = 8;                                                        //帧长度为8
	CAN1_Tx_Message.StdId = CAN_201_204_ID;                                         //帧ID为主机的CAN_ID

	CAN1_Tx_Message.Data[0] = (CAN_DATA_Left.Target_Speed>>8)&0xff;              //201接收电流高8位
	CAN1_Tx_Message.Data[1] = (CAN_DATA_Left.Target_Speed)&0xff;                 //201接收电流低8位
	CAN1_Tx_Message.Data[2] = 0;            //202接收电流高8位
	CAN1_Tx_Message.Data[3] = 0;                  //202接收电流低8位
	CAN1_Tx_Message.Data[4] = (CAN_DATA_Right.Target_Speed>>8)&0xff;              //203接收电流高8位
	CAN1_Tx_Message.Data[5] = (CAN_DATA_Right.Target_Speed)&0xff;                 //203接收电流低8位
	CAN1_Tx_Message.Data[6] = 0;             //204接收电流高8位
	CAN1_Tx_Message.Data[7] = 0;                //204接收电流低8位

	CAN1_Tx_Message_Flag = 0;
	CAN_Transmit(CAN1,&CAN1_Tx_Message);

	while(CAN1_Tx_Message_Flag == 0)
	{
   /*保证数据顺利发出*/
	}
}

/*
  *函数名称：CAN1发送中断服务函数
  *参数输入：无
  *返回值：  无
  *硬件信息：无 
  *功能说明：当信息发送之后，会产生一个中断，如果相应的标志位置一说明中断发送成功
             ，这时通过本函数交将发送信息标志位写1， Can1发送函数就不会卡在while循环里
             这个函数结合上面的函数一起构成了信息发送系统
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
  *函数名称：CAN1接收中断服务函数
  *参数输入：无
  *返回值：  无
  *硬件信息：无 
  *功能说明：将can通讯接收的数据储存到电机can数据结构体中，这里can数据结构体成员都是
             short类型的，因为根据通信协议遥控器把速度和角度值分成了两个字节发送，
             在接收时将两个字节的数据拼接起来，储存到数的类型就能够识别数据的正负
*/
void CAN1_RX0_IRQHandler(void)                                                  //中断向量表参考“startup_stm32f40_41xxx.s”
{   
	CanRxMsg CAN1_Rx_Message;	
	if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
	{
		CAN_Receive(CAN1,CAN_FIFO0,&CAN1_Rx_Message);
		if ( (CAN1_Rx_Message.IDE == CAN_Id_Standard) && (CAN1_Rx_Message.RTR == CAN_RTR_Data) && (CAN1_Rx_Message.DLC == 8) )//标准帧、数据帧、数据长度为8字节
		{

			switch (CAN1_Rx_Message.StdId)
			{
				/*底盘电机*/
				case CAN1_CHASSIS_ID_Left:  //左前-->0x201
				{
					CAN_DATA_Left.Real_Speed  =(CAN1_Rx_Message.Data[2]<<8)|(CAN1_Rx_Message.Data[3]);              //转速
				break;
				}
			
				case CAN1_CHASSIS_ID_Right:  //右后-->0x203
				{
					CAN_DATA_Right.Real_Speed  =(CAN1_Rx_Message.Data[2]<<8)|(CAN1_Rx_Message.Data[3]);              //转速
					break;
				}
				
				/*云台电机*/
				/*摩擦轮电机*/
				/*拨弹盘电机*/
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

