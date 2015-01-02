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
 * Start SPI operations.
 * Enables the SPI pins and set the clock divider.
 */
void avr_spi_begin(void)
{
	uint8_t value = DDRB;
	value |= _BV(SPI_MOSI_PIN); // output
	value &= ~_BV(SPI_MISO_PIN);// input
	value |= _BV(SPI_SCK_PIN);	// output
	value |= _BV(SPI_SS_PIN);	// output
	DDRB = value;

	PORTB |= _BV(SPI_SS_PIN);	// set SS to high

	SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR1) | _BV(SPR0);
}

/**
 * @ingroup SPI
 *
 * End SPI operations.
 */
void avr_spi_end(void) {
	SPCR = 0;
}

/**
 * @ingroup SPI
 * Transfers any number of bytes to the currently selected SPI slave.
 * @param s Buffer of bytes to send.
 * @param len Number of bytes in the buffer, and the number of bytes to send.
 */
void avr_spi_writenb(const char *buf, uint8_t len)
{
	PORTB &= ~_BV(SPI_SS_PIN);

	while (len--)
	{
		SPDR = *buf++;
		loop_until_bit_is_set(SPSR,SPIF);
		_delay_us(10); //TODO _delay_us
	}

	PORTB |= _BV(SPI_SS_PIN);
}

/**
 * @ingroup SPI
 * Transfers any number of bytes to and from the currently selected SPI slave.
 * The returned data from the slave replaces the transmitted data in the buffer.
 * @param buf Buffer of bytes to send. Received bytes will replace the contents.
 * @param len Number of bytes in the buffer, and the number of bytes to send/received.
 */
void avr_spi_transfern(char *buf, uint8_t len)
{
	PORTB &= ~_BV(SPI_SS_PIN);

	while (len--)
	{
		SPDR = *buf;
		loop_until_bit_is_set(SPSR,SPIF);
		*buf = SPDR;
		buf++;
		_delay_us(10); //TODO _delay_us
	}

	PORTB |= _BV(SPI_SS_PIN);
}

/**
 * @ingroup SPI
 * Sets the SPI bit order.
 * @param order The desired bit order, one of AVR_SPI_BIT_ORDER_*,
 * see \ref avrSPIBitOrder
 */
void avr_spi_setBitOrder(uint8_t order)
{
	SPCR |= ((order & 0x01) << DORD);
}

/**
 * @ingroup SPI
 * Sets the SPI clock divider and therefore the SPI clock speed.
 * @param divider The desired SPI clock divider, one of AVR_SPI_CLOCK_DIVIDER_*,
 * see \ref avrSPIClockDivider
 */
void avr_spi_setClockDivider(uint8_t divider)
{
	SPCR |= (((divider & 0x02) == 2) << SPR1);
	SPCR |= ((divider & 0x01) << SPR0);
	SPSR = (((divider & 0x04) == 4) << SPI2X);
}

/**
 * @ingroup SPI
 * Sets the SPI data mode. Sets the clock polarity and phase/
 * @param mode The desired data mode, one of AVR_SPI_MODE*,
 * see \ref avrSPIMode
 */
void avr_spi_setDataMode(uint8_t mode)
{
#if 0
	SPCR |= (((mode & 0x02) == 2) << CPOL);
	SPCR |= ((mode & 0x01) << CPHA);
#else
	SPCR |= ((mode & 0x03) << CPHA);
#endif
}
