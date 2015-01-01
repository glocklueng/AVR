/**
 * @file avr_uart.c
 *
 */
/* Copyright (C) 2014 by Arjan van Vught <pm @ http://www.raspberrypi.org/forum/>
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

#undef BAUD
#define BAUD 38400				///<
#include <util/setbaud.h>

/**
 * @ingroup UART
 *
 */
void avr_uart_begin(void)
{
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;

#if USE_2X
	UCSR0A |= _BV(U2X0);
#else
	UCSR0A &= ~(_BV(U2X0));
#endif

	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
	UCSR0B = _BV(RXEN0) | _BV(TXEN0); /* Enable RX and TX */
}

/**
 * @ingroup UART
 *
 * @param c
 */
void avr_uart_send(const uint8_t c)
{
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
}

/**
 * @ingroup UART
 *
 * @return
 */
uint8_t avr_uart_recieve(void)
{
	loop_until_bit_is_set(UCSR0A, RXC0);
	return UDR0 ;
}
