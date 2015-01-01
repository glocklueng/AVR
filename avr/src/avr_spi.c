/**
 * @file avr_spi.c
 *
 */
/* Copyright (C) 2015 by Arjan van Vught <pm @ http://www.raspberrypi.org/forum/>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <avr/io.h>
#include <util/delay.h>
#include "avr_spi.h"

/**
 * @ingroup SPI
 *
 */
void avr_spi_begin(void)
{
	DDRB |= (1 << SPI_MOSI_PIN); 	// output
	DDRB &= ~(1 << SPI_MISO_PIN);	// input
	DDRB |= (1 << SPI_SCK_PIN);		// output
	DDRB |= (1 << SPI_SS_PIN);		// output

	PORTB |= (1 << SPI_SS_PIN);

	SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR1) | _BV(SPR0);
}

/**
 * @ingroup SPI
 *
 */
void avr_spi_end(void) {

}

/**
 * @ingroup SPI
 *
 * @param s
 * @param len
 */
void avr_spi_writenb(const char *s, uint8_t len)
{
	PORTB &= ~(1 << SPI_SS_PIN);

	while (len--)
	{
		SPDR = *s++;
		while (!(SPSR & _BV(SPIF)))
			;
		_delay_us(10); //TODO _delay_us
	}
	PORTB |= (1 << SPI_SS_PIN);
}

/**
 * @ingroup SPI
 *
 * @param s
 * @param len
 */
void avr_spi_transfern(char *s, uint8_t len)
{
	PORTB &= ~(1 << SPI_SS_PIN);

	while (len--)
	{
		SPDR = *s;
		while (!(SPSR & _BV(SPIF)))
			;
		*s = SPDR;
		_delay_us(10); //TODO _delay_us
		s++;
	}

	PORTB |= (1 << SPI_SS_PIN);
}
