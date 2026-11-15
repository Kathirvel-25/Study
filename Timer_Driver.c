/*
 * Timer_Driver.c
 *
 *  Created on: Jul 19, 2026
 *      Author: kathir
 */
#include "Timer_Driver.h"


void Timer_InputCapture(void)
	{

      TIM2->PSC = 83;
      TIM2->ARR = 0xFFFFFFFF;

      TIM2->CCMR1 &= ~(0b11<<0);
      TIM2->CCMR1 |= (0b01<<0);

      TIM2->CCMR1 &= ~(0b1111<<4);
      TIM2->CCMR1 |= (0b0011<<4);

      TIM2->CCMR1 &= ~(3<<2);

      TIM2->CCER &= ~(1<<1 | (1<<3));

      TIM2->CCER |= (1<<0);

      TIM2->CR1 |= (1<<0);
}

pwm_data Calculation(void)
{
    uint32_t fall, rise, high, period, rise2;
    pwm_data pwm;

	while(!(TIM2->SR & (1<<1)));

	rise = TIM2->CCR1;

	TIM2->CCER |= (1<<1);  // Falling edge

	while(!(TIM2->SR & (1<<1)));

	fall = TIM2->CCR1;

	TIM2->CCER &= ~(1<<1);  // Rising edge

	while(!(TIM2->SR & (1<<1)));

	rise2 = TIM2->CCR1;


	high = fall - rise;
	period = rise2 - rise;

	pwm.frequency = 1000000UL/period;
	pwm.duty =  ((float)high*100.0f)*period;

	return pwm;
}


void Timer_OutputCompare(void)
{
	TIM2->PSC = 83;
	TIM2->ARR = 1999;

	TIM2->CCR1 = 1000;

	TIM2->CCMR1 &= ~(0b11 << 0);

	TIM2->CCMR1 &= ~(0b111 << 4);
	TIM2->CCMR1 |= (3<<4); //  configure as op

	TIM2->CCMR1 |= (1U << 3);


	TIM2->CCER |= (1<<0); //capture is anbled
	TIM2->CR1 |= (1<<0);

}

void delay_ms(uint32_t ms)
{
	TIM2->PSC = 83;
	TIM2->ARR = 999;

	while(ms--){
		TIM2->CNT = 0;
		TIM2->SR |= ~(1<<0);
		TIM2->CR1 |= (1<<0);
		while(!(TIM2->SR & (1<<0)));
		TIM2->CR1 &= ~(1<<0);
	}
}

void Timer_PWM_init(void)
{
	TIM2->PSC = 83;
	TIM2->ARR = 19999;

	TIM2->CCR1 = 1500;

	TIM2->CCMR1 &= ~(3<<0);

	TIM2->CCMR1 &= ~(7<<4);
	TIM2->CCMR1 |= (6<<4);

	TIM2->CCMR1 |= (1<<3);

	TIM2->CCER |= (1<<0);

	TIM2->CR1 |= (1<<7);

	TIM2->EGR |= (1<<0);

	TIM2->CR1 |= (1<<0);
}

void Servo_angle(uint8_t angel)
{
	if(angel > 180)
		angel = 180;

	TIM2->CCR1 = 1000 + ((angel*1000)/180);
}
