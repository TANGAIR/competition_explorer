#ifndef _LED_H
#define _LED_H
#include "sys.h"

#define  LED1_ON      GPIO_ResetBits(GPIOE, GPIO_Pin_7)
#define  LED1_OFF     GPIO_SetBits(GPIOE, GPIO_Pin_7)
#define  LED_RED_TOGGLE  GPIO_ToggleBits(GPIOE, GPIO_Pin_7)

#define  LED2_ON        GPIO_ResetBits(GPIOF, GPIO_Pin_14)
#define  LED2_OFF       GPIO_SetBits(GPIOF, GPIO_Pin_14)
#define  LED_GREEN_TOGGLE    GPIO_ToggleBits(GPIOF, GPIO_Pin_14)

void LED_Init(void);

void led_against(void);//≈‹¬Ìµ∆≥Ã–Ú

void led_R_twinkle(void );//∫Ïµ∆…¡À∏

void led_Y_twinkle(void );//¬Ãµ∆…¡À∏

#endif
