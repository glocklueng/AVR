/*
 * avr-spi.h
 *
 *  Created on: Dec 29, 2014
 *      Author: pi
 */

#ifndef AVR_SPI_H_
#define AVR_SPI_H_

#include <stdint.h>

extern void avr_spi_begin(void);
extern void avr_spi_transfern(char*, const uint8_t);
extern void avr_spi_writenb(const char *, uint8_t);

//TODO
#define bcm2835_spi_begin			avr_spi_begin
#define bcm2835_spi_writenb			avr_spi_writenb
#define bcm2835_spi_transfern		avr_spi_transfern
//TODO
inline static void bcm2835_spi_end(void)
{
}

#endif /* AVR_SPI_H_ */
