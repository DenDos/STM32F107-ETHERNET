#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "InitS.h"
#include "misc.h"
#include "stm32f10x_exti.h"
uint16_t LEDS1 = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_14 | GPIO_Pin_15;//Константа с диодами 6789 
////Разрешаем тактирование всего///////////
void RCC_IN(void)
{	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOG|RCC_APB2Periph_GPIOF, ENABLE);
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3,ENABLE);
RCC->APB2ENR|= RCC_APB2ENR_AFIOEN;//разрешаем тактирование альтернативных функций
}
	//////инициаллизация лампочки/////
void initLED(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
////////Инициаллизация Джойстика/////////////////
void Init_Joystick(void)
{
	GPIO_InitTypeDef PORT;
	//Джайстик 
	PORT.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  PORT.GPIO_Speed = GPIO_Speed_50MHz;
  PORT.GPIO_Mode = GPIO_Mode_IPU; 
  GPIO_Init(GPIOC, &PORT);
	//////////////////
}
//////////Инициаллизация Прерывания Реле////////////
	void Init_Int (void)
		{
			
//		NVIC_InitTypeDef nvic;
		EXTI_InitTypeDef exti;	
			
		AFIO->EXTICR [6>>0x02] |= AFIO_EXTICR2_EXTI6_PG; 	
			 
    exti.EXTI_Mode = EXTI_Mode_Interrupt; 
    exti.EXTI_Trigger = EXTI_Trigger_Falling; 
    exti.EXTI_LineCmd = ENABLE;   
		exti.EXTI_Line =EXTI_Line6;;
		EXTI_Init(&exti);
			
		NVIC_EnableIRQ(EXTI9_5_IRQn);
		}
//Инициаллизация  таймера TIM2 для ШИМ
void InitServo_PWM_TIM2(void)
{
GPIO_InitTypeDef PORT;
PORT.GPIO_Pin = (GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_0|GPIO_Pin_3);
PORT.GPIO_Mode = GPIO_Mode_AF_PP;
PORT.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &PORT);
	
	//	канал 4
		TIM2->CCER |= (TIM_CCER_CC4E);
			TIM2->CCMR2|= (TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2);	
	//канал 3
		TIM2->CCER |= (TIM_CCER_CC3E);
			TIM2->CCMR2|= (TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2);	
	//канал 2
		TIM2->CCER |= (TIM_CCER_CC2E);
			TIM2->CCMR1|= (TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2);	
	//	канал 1
		TIM2->CCER |= (TIM_CCER_CC1E);
			TIM2->CCMR1|= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);		

TIM2->PSC = 351;
TIM2->ARR = 4096;	
TIM2->CR1 |= TIM_CR1_CEN;
}

////////////Инициаллизация  таймера TIM3 для ШИМ /////////////////
void InitServo_PWM_TIM3(void)
{
GPIO_InitTypeDef PORT;
PORT.GPIO_Pin = (GPIO_Pin_6|GPIO_Pin_7);
PORT.GPIO_Mode = GPIO_Mode_AF_PP;
PORT.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &PORT);
	PORT.GPIO_Pin = (GPIO_Pin_0|GPIO_Pin_1);
	PORT.GPIO_Mode = GPIO_Mode_AF_PP;
	PORT.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &PORT);
	/**/
	//	канал 4
		TIM3->CCER |= (TIM_CCER_CC4E);
			TIM3->CCMR2|= (TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2);	
	//канал 3
		TIM3->CCER |= (TIM_CCER_CC3E);
			TIM3->CCMR2|= (TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2);	
	//канал 2
		TIM3->CCER |= (TIM_CCER_CC2E);
			TIM3->CCMR1|= (TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2);	
	//	канал 1
		TIM3->CCER |= (TIM_CCER_CC1E);
			TIM3->CCMR1|= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);		

TIM3->PSC = 351;
TIM3->ARR = 4096;	
TIM3->CR1 |= TIM_CR1_CEN;
}
////////////Инициаллизируем реле////////////
void Rele(void)
{
GPIO_InitTypeDef PORT;
	PORT.GPIO_Pin = (GPIO_Pin_7);
	PORT.GPIO_Mode = GPIO_Mode_AIN;
	PORT.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &PORT);
}
void InitAll(void)
{
RCC_IN();
	InitServo_PWM_TIM2();
		InitServo_PWM_TIM3();
			//Init_Joystick();
				//Init_Int();
					initLED();

}
