/**
 * @file i2c.c
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
#include <util/twi.h>
#include "i2c.h"

#define F_SCL 100000UL // SCL frequency
#define PRESCALER 1

/**
 *
 */
void i2c_init(void)
{
	TWBR = ((((F_CPU / F_SCL) / PRESCALER) - 16) / 2);
}

/**
 *
 * @param address
 * @return
 */
uint8_t i2c_start(uint8_t address)
{
	// reset TWI control register
	TWCR = 0;
	// transmit START condition
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	// wait for end of transmission
	while (!(TWCR & (1 << TWINT)))
		;

	// check if the start condition was successfully transmitted
	if ((TWSR & 0xF8) != TW_START)
	{
		return 1;
	}

	// load slave address into data register
	TWDR = address;
	// start transmission of address
	TWCR = (1 << TWINT) | (1 << TWEN);
	// wait for end of transmission
	while (!(TWCR & (1 << TWINT)))
		;

	// check if the device has acknowledged the READ / WRITE mode
	uint8_t twst = TW_STATUS & 0xF8;
	if ((twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK))
		return 1;

	return 0;
}

/**
 *
 * @param data
 * @return
 */
uint8_t i2c_write(uint8_t data)
{
	// load data into data register
	TWDR = data;
	// start transmission of data
	TWCR = (1 << TWINT) | (1 << TWEN);
	// wait for end of transmission
	while (!(TWCR & (1 << TWINT)))
		;

	if ((TWSR & 0xF8) != TW_MT_DATA_ACK)
	{
		return 1;
	}

	return 0;
}

/**
 *
 * @return
 */
uint8_t i2c_read_ack(void)
{

	// start TWI module and acknowledge data after reception
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	// wait for end of transmission
	while (!(TWCR & (1 << TWINT)))
		;
	// return received data from TWDR
	return TWDR ;
}

/**
 *
 * @return
 */
uint8_t i2c_read_nack(void)
{

	// start receiving without acknowledging reception
	TWCR = (1 << TWINT) | (1 << TWEN);
	// wait for end of transmission
	while (!(TWCR & (1 << TWINT)))
		;
	// return received data from TWDR
	return TWDR ;
}

/**
 *
 */
void i2c_stop(void)
{
	// transmit STOP condition
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}
