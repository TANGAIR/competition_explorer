/*************************************************************
  *文件名称：遥控器源文件
	
	*数据定义：遥控器解码后数据结构体、
	           控制信息数据结构体、
	           遥控器原始数据储存数组、
						 遥控器数据缓冲标志位
						 
  *外部数据使用：云台电机数据结构体、
	               云台初始数据结构体
	
	*函数定义：遥控器初始化函数、
	           遥控器数据解码函数、
					   Dma中断服务函数、
					   控制数据转换函数
	
  *外部自定义函数使用：无
  
	*功能说明：从遥控器读取数据进行解码，按照控制模式进行转换，
	           最终将控制数据储存到控制信息数据结构体中
						 缺少键盘鼠标模式的相关转换
***************************************************************/


/****************头文件包含区******************************/
#include "main.h"

/****************数据定义区********************************/

/*遥控器数据处理后，储存结构体定义*/
DBUS_Data_TypeDef DBUS;
Control_Info_TypeDef Control_Info;

/*遥控器原始数据储存数组定义*/
uint8_t DBUS_buffer[18];

/*相关标志位定义*/
short DR16_Flash_Val=0;


/****************函数功能区******************************/
/*
  *函数名称：遥控器初始化函数
  *参数输入：无
  *返回值：  无
  *硬件信息：DR16  USART1_RX-->PB7    DMA接收
  *功能说明：将遥控器数据用串口1接收以及DMA直接内存访问初始化,开启dma2中断，
	           建立DMA2数据传输流。
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
	USART_InitStructure.USART_BaudRate           =100000;//70Kbps   遥控器控制协议V1.4.pdf
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
  DMA_DeInit(DMA2_Stream5);                //为DMA配置通道
	DMA_InitStructure.DMA_Channel           =DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr=(uint32_t)&(	USART1->DR);           //起始地址
	DMA_InitStructure.DMA_Memory0BaseAddr   =(uint32_t)DBUS_buffer;              //存储变量
	DMA_InitStructure.DMA_DIR               =DMA_DIR_PeripheralToMemory;         //传输方向
	DMA_InitStructure.DMA_BufferSize        =DBUS_DATE_LENGTH+DBUS_SAFTY_LENGTH; //缓冲区长度
	DMA_InitStructure.DMA_PeripheralInc     =DMA_PeripheralInc_Disable;          //外设递增模式
	DMA_InitStructure.DMA_MemoryInc         =DMA_MemoryInc_Enable;               //内存递增模式
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;        //DMA访问时每次操作的数据长度
	DMA_InitStructure.DMA_MemoryDataSize    =DMA_PeripheralDataSize_Byte;        //DMA访问时每次操作的数据长度
	DMA_InitStructure.DMA_Mode              =DMA_Mode_Circular;                  //传输模式：连续不断
	DMA_InitStructure.DMA_Priority          =DMA_Priority_VeryHigh;              //优先级别
  DMA_InitStructure.DMA_FIFOMode          =DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold     =DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst       =DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst   =DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream5,&DMA_InitStructure);
	
	DMA_ITConfig(DMA2_Stream5,DMA_IT_TC,ENABLE);
	DMA_Cmd(DMA2_Stream5,ENABLE);	
}



/*
  *函数名称：遥控器数据解码函数
  *参数输入：无
  *返回值：  无
  *硬件信息：无
  *功能说明：将从遥控器中读取的18个字节的数据进行解码，储存到遥控器数据结构体中
             控制时ch0、ch2向右为正,ch1、ch3向上为正（与遥控器协议规定的方向相反）,
             鼠标X轴向右为正,Y轴向下为正
	*/
void DBUS_Data_Decode(void)
{
	
	//遥控器
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
	
	
	//鼠标
	DBUS.PC.X_Real[5]    = DBUS_buffer[6] | (DBUS_buffer[7] << 8);                                       
	DBUS.PC.X=0;
	for(short k=0;k<5;k++)//取过去5次的平均值
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
	
	
	//键盘
	DBUS.PC.Keyboard     = DBUS_buffer[14] | DBUS_buffer[15] << 8;                             
}




/*
  *函数名称：遥控器DMA中断服务函数
  *参数输入：无
  *返回值：  无
  *硬件信息：无
  *功能说明：每隔 14ms 通过 DBUS 接收一帧数据（18 字节），同时在此中断服务函数中
             对遥控器数据进行初步解码
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
  *函数名称：控制数据转换函数
  *参数输入：无
  *返回值：  无
  *硬件信息：无
  *功能说明：将之前解码获得的遥控器数据转换成控制信息，
             包括遥控器控制与键盘鼠标控制两个转换模式
	*/
void Control_Data_Process(void)
{
 if(DR16_Flash_Val>0)
 {
		DR16_Flash_Val--;
		/*遥控器模式*/
		if(DBUS.RC.Switch_Right != RC_SW_DOWN) //遥控器控制模式
		{
			//遥控器赋值
			if(DBUS.RC.Switch_Right == RC_SW_UP)
			{
				 //底盘电机		
				 Control_Info.Chassis.Direction_X=-DBUS.RC.ch0/3;
				 Control_Info.Chassis.Direction_Y=DBUS.RC.ch1/3;
				
				//云台电机
				 Control_Info.PTZ.Yaw=-DBUS.RC.ch2/2;
				 Control_Info.PTZ.Pitch=DBUS.RC.ch3/2;
			}
			 //  Control_Info.Chassis.Direction_Omega=((Yaw_Motor_205.Real_MechanicalAngle-PTZ_Init_Value.Initial_Yaw_Angle)/2);
				 if(abs(Control_Info.Chassis.Direction_Omega)<5)  Control_Info.Chassis.Direction_Omega =0;
			//翻盖电机
			if(DBUS.RC.Switch_Right == RC_SW_MID)
			{
			 Control_Info.Flip.Aim_Flip=-DBUS.RC.ch0/5;	
			}
		}

		/*键鼠模式*/
		else if(DBUS.RC.Switch_Right == RC_SW_DOWN)
		{
			 //暂无
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

















