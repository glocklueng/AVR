/**
 * @file uart_steam.c
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

#include <stdio.h>
#include <avr/io.h>

/**
 * @ingroup UART-STREAM
 *
 * @param stream
 * @return
 */
int uart_getchar(FILE *stream) {
    loop_until_bit_is_set(UCSR0A, RXC0);
    if (UCSR0A & _BV(FE0))
    	return _FDEV_EOF;
    if (UCSR0A & _BV(DOR0))
    	return _FDEV_ERR;
    return UDR0;
}

/**
 * @ingroup UART-STREAM
 *
 * @param c
 * @param stream
 * @return
 */
int uart_putchar(char c, FILE *stream) {

	if (c == '\n') {
        uart_putchar('\r', stream);
    }

	loop_until_bit_is_set(UCSR0A, UDRE0);

	UDR0 = c;

    return 0;
}
