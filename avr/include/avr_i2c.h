/**
 * @file avr_i2c.h
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

#ifndef AVR_I2C_H_
#define AVR_I2C_H_

#include <stdint.h>

// create alias for the different I2C chip pins - code assumes all on port C
#if (defined(__AVR_AT90USB82__) || defined(__AVR_AT90USB162__))
#elif (defined(__AVR_ATmega48__) || defined(_AVR_ATmega88__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__))
 #define SCL_PORT    PORTC
 #define SCL_BIT     PORTC5
 #define SDA_PORT    PORTC
 #define SDA_BIT     PORTC4
#else
 #error unknown processor - add to avr_i2c.h
#endif

extern void avr_i2c_begin(void);
extern void avr_i2c_end(void);
extern void avr_i2c_setSlaveAddress(const uint8_t);
extern uint8_t avr_i2c_write(const char *, uint8_t);
extern uint8_t avr_i2c_read(char*, const uint8_t);

#endif /* AVR_I2C_H_ */
