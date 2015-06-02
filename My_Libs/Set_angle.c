#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "Set_angle.h"
#include "main.h"
//#include "Delay.h"

#define SERVO_180_1 500
#define SERVO_0_1 250

#define SERVO_180_2 576
#define SERVO_0_2 140

//#define SERVO_180_2 495
#define SERVO_180_3 700
#define SERVO_0_3 230

#define SERVO_180_4 1000
#define SERVO_0_4 250

//#define SERVO_180_4 1000
//#define SERVO_0_4 320
int i;
int PWM;
int t=250;
//Задержка
/*
void delay(void) {
volatile uint32_t i;
for (i=1; i != 0xFFFF; i++)
;
}*/

//Устанавливает угол в градусах для непонятных серво
void set_pos(int pos1,int pos2,int pos3,int pos4) {
	float tmp1=(SERVO_180_2 - SERVO_0_2) /180+0.2;
	float tmp2=(SERVO_180_2 - SERVO_0_2) /180;
	float tmp3=(SERVO_180_3 - SERVO_0_3) /180+0.2;
	float tmp4=(SERVO_180_4 - SERVO_0_4) /180+0.2;
	
	//pos3*=-1;
	TIM3->CCR1 = SERVO_0_1 + tmp1 * pos1;//PA6
	TIM3->CCR2 = SERVO_0_2 + tmp2 * pos2;//PA7
	TIM3->CCR3 = SERVO_0_3 + tmp3 * pos3;//PB0
	TIM3->CCR4 = SERVO_0_4 + tmp4 * pos4;//PB1

}
//Устанавливает угол в градусах для Tower Pro Micro Servo
void set_pos_ms(int pos) {
/*float tmp=(SERVO_180_MS - SERVO_0_MS) /180+0.23;
//TIM2->CCR2 = SERVO_0_MS + tmp * pos;
	TIM2->CCR3 = SERVO_0_MS + tmp * pos;*/
}
