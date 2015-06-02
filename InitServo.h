#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

GPIO_InitTypeDef PORT;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
PORT.GPIO_Pin = (GPIO_Pin_1);
PORT.GPIO_Mode = GPIO_Mode_AF_PP;
PORT.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &PORT);
TIM2->CCER |= (TIM_CCER_CC2E);
TIM2->CCMR1|= (TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2);	
TIM2->PSC = 351;
TIM2->ARR = 4096;	
TIM2->CR1 |= TIM_CR1_CEN;