/*
 * spi_driver.c
 *
 *  Created on: Jul 17, 2026
 *      Author: kathir
 */


#include "spi_driver.h"

void cs_low(void)
{
	GPIOA->ODR &= ~(1<<12);

}
void cs_high(void)
{
	GPIOA->ODR |= (1<<12);

}

void SPI_Transmit(uint8_t *data, uint8_t size){

	cs_low();

	while(size--){
		while(!(SPI1->SR & (1<<1)));
		SPI1->DR = *data++;
		while(!(SPI1->SR & (1<<0)));
        volatile uint8_t dummy = SPI1->DR;
	}

	while((SPI1->SR & (1<<7)));

	cs_high();


}

void SPI_Receive(uint8_t *data, uint8_t size){

	cs_low();

	while(size--){
		while(!(SPI1->SR & (1<<1)));

		SPI1->DR = 0xFF;

		while(!(SPI1->SR & (1<<0)));

		*data++ = SPI1->DR;
	}

	while((SPI1->SR & (1<<7)));

	cs_high();

}

void SPI_TransReceive(uint8_t *tdata, uint8_t *rdata){

	cs_low();

	while(!(SPI1->SR & (1<<1)));

	SPI1->DR = *tdata++;

	while(!(SPI1->SR & (1<<0)));

	*rdata++ = SPI1->DR;

	while((SPI1->SR & (1<<7)));

	cs_high();

}


void SPI_Transmit_IT(SPI_Typedef *hspi,uint8_t *data, uint8_t size){
	if(hspi->tx_busy == 1){
		return;
	}

	hspi->tx_busy = 1;
	hspi->t_idx = 0;
	hspi->txbuf = data;
	hspi->txlen = size;

	SPI1->CR2 |= (1<<7);
	SPI1->CR2 |= (1<<6);
	cs_low();
}

void SPI_Receive_IT(SPI_Typedef *hspi,uint8_t *data, uint8_t size){
	if(hspi->tx_busy == 1){
		return;
	}

	hspi->rx_busy = 1;
	hspi->r_idx = 0;
	hspi->rxbuf = data;
	hspi->rxlen = size;

	SPI1->CR2 |= (1<<6);
	SPI1->CR2 |= (1<<7);
	cs_low();
}
