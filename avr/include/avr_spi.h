/**
 * @file avr_spi.h
 *
 */
/* Copyright (C) 2015 by Arjan van Vught <pm @ http://forum.bitwizard.nl/>
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
#ifndef AVR_SPI_H_
#define AVR_SPI_H_

#include <stdint.h>

/// Specifies the SPI data bit ordering for \ref avr_spi_setBitOrder()
typedef enum
{
	AVR_SPI_BIT_ORDER_LSBFIRST = 1,	///< LSB First, send least significant bit (bit 0) first
	AVR_SPI_BIT_ORDER_MSBFIRST = 0	///< MSB First, send most significant bit (bit 7) first
} avrSPIBitOrder;

/// Specify the SPI data mode to be passed to \ref avr_spi_setDataMode()
typedef enum
{
	AVR_SPI_MODE0 = 0x00,	///< CPOL = 0, CPHA = 0
	AVR_SPI_MODE1 = 0x01,	///< CPOL = 0, CPHA = 1
	AVR_SPI_MODE2 = 0x02,	///< CPOL = 1, CPHA = 0
	AVR_SPI_MODE3 = 0x03	///< CPOL = 1, CPHA = 1
} avrSPIMode;

/// Specifies the divider used to generate the SPI clock from the system clock.
typedef enum
{
	AVR_SPI_CLOCK_DIVIDER_4 = 0x00,		///< chip clock/4
	AVR_SPI_CLOCK_DIVIDER_16 = 0x01,	///< chip clock/16
	AVR_SPI_CLOCK_DIVIDER_64 = 0x02,	///< chip clock/64
	AVR_SPI_CLOCK_DIVIDER_128 = 0x04,	///< chip clock/128
	AVR_SPI_CLOCK_DIVIDER_2 = 0x05,		///< chip clock/2
	AVR_SPI_CLOCK_DIVIDER_8 = 0x05,		///< chip clock/8
	AVR_SPI_CLOCK_DIVIDER_32 = 0x06		///< chip clock/32
} avrSPIClockDivider;

// create alias for the different SPI chip pins - code assumes all on port B
#if (defined(__AVR_AT90USB82__) || defined(__AVR_AT90USB162__))
 #define SPI_SS_PIN PORTB0
 #define SPI_SCK_PIN PORTB1
 #define SPI_MOSI_PIN PORTB2
 #define SPI_MISO_PIN PORTB3
#elif (defined(__AVR_ATmega48__) || defined(_AVR_ATmega88__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__))
 #define SPI_SS_PIN PORTB2
 #define SPI_SCK_PIN PORTB5
 #define SPI_MOSI_PIN PORTB3
 #define SPI_MISO_PIN PORTB4
#else
 #error unknown processor - add to avr_spi.h
#endif

extern void avr_spi_begin(void);
extern void avr_spi_end(void);
extern void avr_spi_transfern(char*, const uint8_t);
extern void avr_spi_writenb(const char *, uint8_t);
extern void avr_spi_setBitOrder(uint8_t);
extern void avr_spi_setClockDivider(uint8_t);
extern void avr_spi_setDataMode(uint8_t);

#endif /* AVR_SPI_H_ */
