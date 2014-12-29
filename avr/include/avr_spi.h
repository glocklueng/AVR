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

inline static void avr_spi_end(void)
{
}

#endif /* AVR_SPI_H_ */
