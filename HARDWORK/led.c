#include "main.h"
/***************************************数据定义区***************************************/


/***************************************函数处理区***************************************/
/**
  * @brief  LED灯初始化
  * @param  void
  * @retval void
  * @notes  LED_RED(PE7)  LED_GREEN(PF14)  
  */
void LED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOF,ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;//LED0和LED1对应IO口
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOF, &GPIO_InitStruct);//初始化GPIO

    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOE,ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;//LED0和LED1对应IO口
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOE, &GPIO_InitStruct);//初始化GPIO

    LED2_OFF;
    LED1_OFF;
}	

int led_t=2;//计时单位秒


void led_against(void)
{
 while(led_t-->0)
 {
    LED2_ON;	 
	 delay_ms(300);
    LED2_OFF;
	 delay_ms(300);
 }
 Beep_ON();
 delay_ms(500);
 Beep_OFF();
 LED2_ON;//绿灯常亮表示程序正在正常运行
 LED1_OFF;//关闭红灯
}


void led_R_twinkle(void )//红灯闪烁
{
while(1)//红灯一直闪烁
 {
    LED1_OFF;
	 delay_ms(300);
    LED1_ON;
	 delay_ms(300);
 }

}


void led_Y_twinkle(void )//绿色灯闪烁闪烁
{
while(1)
 {
    LED2_OFF;
	 delay_ms(300);
    LED2_ON;
	 delay_ms(300);
 }

}

