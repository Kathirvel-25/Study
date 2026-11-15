/*
 * I2C_Driver.c
 *
 *  Created on: Jul 17, 2026
 *      Author: kathir
 */

#include "I2c_Driver.h"


void I2C_Init(void);

void I2C_Transmit_IT(uint8_t addr,uint8_t *tdata,uint8_t len, I2C_typedef *hi2c){

	if(hi2c->t_bussy == 1){
		return;
	}


	hi2c->t_bussy = 1;
	hi2c->txlen = len;
	hi2c->t_idx = 0;
	hi2c->slaveaddr = addr<<1|0;
	hi2c->txbuf = tdata;
	hi2c->State = I2C_STATE_IDILE;

	//Start bit
	I2C1->CR1 |= (1<<8);

}

void I2C_Receive_IT(uint8_t addr,uint8_t *tdata,uint8_t len, I2C_typedef *hi2c){

	if(hi2c->r_bussy == 1){
		return;
	}


	hi2c->r_bussy = 1;
	hi2c->rxlen = len;
	hi2c->r_idx = 0;
	hi2c->slaveaddr = addr<<1|1;
	hi2c->rxbuf = tdata;
	hi2c->State = I2C_STATE_IDILE;

	//Start bit
	I2C1->CR1 |= (1<<8);

}

void I2C_Receive(uint8_t *rdata,uint8_t addr, uint8_t len){

	I2C1->CR1 |= (1<<8);
	while(!(I2C1->SR1 & (1<<0)));
	I2C1->DR = (addr<<1)|1;
	while(!(I2C1->SR1 & (1<<1)));

	uint8_t size = len;

	if(len == 1){

		I2C1->CR1 &= ~(1<<10);

		volatile uint32_t temp;
		temp = I2C1->SR1 | I2C1->SR2;
		(void)temp;

		I2C1->CR1 |= (1<<9);

		*rdata = I2C1->DR;
	}

	else
	{
		I2C1->CR1 |= (1<<10);
		volatile uint32_t temp;
		temp = I2C1->SR1 | I2C1->SR2;
		(void)temp;

		while(size > 2){
			while(!(I2C1->SR1 & (1<<6)));
			*rdata = I2C1->DR;
			size--;
		}
		while(!(I2C1->SR1 & (1<<2)));

		I2C1->CR1 &= ~(1<<10);

		*rdata++ = I2C1->DR;

		I2C1->CR1 |= (1<<9);

		*rdata++ = I2C1->DR;
	}

}


void I2C_Transmit(uint8_t *tdata, uint8_t addr, uint8_t len)
{

	I2C1->CR1 |= (1<<8);
	while(!(I2C1->SR1 & (1<<0)));
	I2C1->DR = (addr<<1)|0;
	while(!(I2C1->SR1 & (1<<1)));

	volatile uint32_t temp;
	temp = I2C1->SR1 | I2C1->SR2;
	(void)temp;

	while(len--){
		while(!(I2C1->SR1 & (1<<7)));
		I2C1->DR = *tdata++;
	}

	while(!(I2C1->SR1 & (1<<2)));
	I2C1->CR1 |= (1<<9);

}


