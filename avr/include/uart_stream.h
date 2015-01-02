/**
 * @file uart_stream.h
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

#ifndef UART_STREAM_H_
#define UART_STREAM_H_

#include <stdio.h>
#include "avr_uart.h"

extern int uart_putchar(char c, FILE *stream);
extern int uart_getchar(FILE *stream);

FILE uart_stream = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);
///< FDEV_SETUP_STREAM macro can be used to setup a buffer which is valid for stdio operations.

#define UART_BEGIN() {avr_uart_begin();stdout = stdin = &uart_stream;}
///< Init the UART and redirect both STDIN and STDOUT to UART.
///< This enables us to use AVR Libc provided functions to read and write to serial port.

#endif /* UART_STREAM_H_ */
