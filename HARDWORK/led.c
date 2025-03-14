#include "main.h"
/***************************************���ݶ�����***************************************/


/***************************************����������***************************************/
/**
  * @brief  LED�Ƴ�ʼ��
  * @param  void
  * @retval void
  * @notes  LED_RED(PE7)  LED_GREEN(PF14)  
  */
void LED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOF,ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;//LED0��LED1��ӦIO��
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOF, &GPIO_InitStruct);//��ʼ��GPIO

    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOE,ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;//LED0��LED1��ӦIO��
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOE, &GPIO_InitStruct);//��ʼ��GPIO

    LED2_OFF;
    LED1_OFF;
}	

int led_t=2;//��ʱ��λ��


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
 LED2_ON;//�̵Ƴ�����ʾ����������������
 LED1_OFF;//�رպ��
}


void led_R_twinkle(void )//�����˸
{
while(1)//���һֱ��˸
 {
    LED1_OFF;
	 delay_ms(300);
    LED1_ON;
	 delay_ms(300);
 }

}


void led_Y_twinkle(void )//��ɫ����˸��˸
{
while(1)
 {
    LED2_OFF;
	 delay_ms(300);
    LED2_ON;
	 delay_ms(300);
 }

}

