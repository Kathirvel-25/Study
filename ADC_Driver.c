/*
 * ADC_Driver.c
 *
 *  Created on: Nov 15, 2026
 *      Author: kathir
 */

#include "ADC_Driver.h"


void ADC_Single_Mode_Init(void)
{
    RCC->AHB1ENR |= (1 << 0);

    GPIOA->MODER &= ~(0x3 << 0);
    GPIOA->MODER |= (0x3 << 0);

    RCC->APB2ENR |= (1 << 8);

    // ADC Prescale Fclk/4

    ADC1->CCR &= ~(3 << 16);
    ADC1->CCR |= (1 << 16);

    //scan = 0
    ADC1->CR1 &= (1 << 8);

    //12-BIT ADC
    ADC1->CR1 &= ~(0x3 << 24);

    //Single Conversion
    ADC1->CR2 &= ~(1 << 1);

    // One Conversion
    ADC1->SQR1 &= ~(0xF << 20);

    //Select Channel 0
    ADC1->SQR3 &= ~(0x1F << 0);

    //Configure the Sampling Time
    ADC1->SMPR2 &= ~(7 << 0);
    ADC1->SMPR2 |= (4 << 0);

    //ADC ON
    ADC1->CR2 |= (1 << 0);
}

uint16_t ADC_Single_Mode_Val(void)
{
	uint16_t adc_val;

	ADC1->CR2 |= (1 << 30);

	while(!(ADC1->SR & (1 << 1)));

	adc_val = ADC1->DR;

	return adc_val;
}

void ADC_Continuous_Mode_Init(void)
{
	 RCC->AHB1ENR |= (1 << 0);

	 RCC->APB2ENR |= (1 << 8);

	 GPIOA->MODER &= ~(3 << 0);
	 GPIOA->MODER |= (3 << 0);

	 ADC1->CCR &= ~(3 << 16);
	 ADC1->CCR |= (1 << 16);

	 ADC1->CR1 &= ~(3 << 24);

	 ADC1->CR1 &= ~(1 << 8);

	 ADC1->CR1 &= ~(1 << 1);
	 ADC1->CR1 |= (1 << 1);

	 ADC1->SMPR2 &= ~(7 << 0);
	 ADC1->SMPR2 |= (4 << 0);

	 ADC1->SQR1 &= ~(0xF << 20);

	 ADC1->CR2 |= (1 << 0);

	 ADC1->CR2 |= (1 << 30);
}

uint16_t ADC_Continuous_Mode_Val(void)
{
	if((ADC1->SR & (1 << 1)))
	{
		return ADC1->DR;
	}
}

void ADC_Discon_Mode_Init(void)
{
	RCC->AHB1ENR |= (1 << 0);
	RCC->APB2ENR |= (1 << 8);

	for(uint8_t i = 0; i < 3; i++)
	{
		GPIOA->MODER &= ~(3U << (i*2));
		GPIOA->MODER |= (3U << (i*2));
	}

	ADC1->CR1 &= ~(3U << 24);

	ADC1->CCR &= ~(3U << 16);
	ADC1->CCR |= (1 << 16);

	ADC1->CR1 &= ~(7U << 13);
	ADC1->CR1 |= (1 << 13);

	for(uint8_t i = 0; i < 3; i++)
	{
		ADC1->SMPR2 &= ~(7U << (i*3));
		ADC1->SMPR2 |= (4U << (i*3));
	}

	ADC1->SQR1 &= ~(0xFU << 20);
	ADC1->SQR1 |= (2U << 20);

	for(uint8_t i = 0; i < 3; i++)
		ADC1->SQR3 |= (i << (i * 5));

	ADC1->CR2 = 0;

	ADC1->CR1 |= (1 << 8);
	ADC1->CR1 |= (1 << 11);

	ADC1->CR1 |= (1 << 13);

	ADC1->CR2 |= (1 << 10);

	ADC1->CR2 |= (1 << 0);
}


void ADC_Discon_Mode_Val(uint16_t *val, uint8_t count)
{
	ADC1->CR2 |= (1 << 30);

	for(uint8_t i = 0; i < count; i++)
	{
		while(!(ADC1->SR & (1 << 1)));
		val[i] = ADC1->DR;
	}

}
