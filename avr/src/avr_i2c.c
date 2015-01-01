/**
 * @file avr_i2c.c
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

#include <stdlib.h>
#include <util/twi.h>
#include "avr_i2c.h"

#define I2C_READ	0x01		///<
#define I2C_WRITE	0x00		///<

#define F_SCL		100000UL	///< SCL frequency
#define PRESCALER 	1

static uint8_t slave_address;	///<

/**
 * @ingroup I2C
 *
 * @param address
 * @return
 */
static uint8_t i2c_start(uint8_t address)
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
 * @ingroup I2C
 *
 * @param data
 * @return
 */
static uint8_t i2c_write(uint8_t data)
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
 * @ingroup I2C
 *
 * @return
 */
static uint8_t i2c_read_ack(void)
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
 * @ingroup I2C
 *
 * @return
 */
#if 0
static uint8_t i2c_read_nack(void)
{

	// start receiving without acknowledging reception
	TWCR = (1 << TWINT) | (1 << TWEN);
	// wait for end of transmission
	while (!(TWCR & (1 << TWINT)))
		;
	// return received data from TWDR
	return TWDR ;
}
#endif

/**
 * @ingroup I2C
 *
 */
static void i2c_stop(void)
{
	// transmit STOP condition
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

/**
 * @ingroup I2C
 *
 * Pull up resistors are activated for TWI clock line / TWI data line
 *
 */
void avr_i2c_begin(void)
{
	TWBR = ((((F_CPU / F_SCL) / PRESCALER) - 16) / 2);
	SCL_PORT |= _BV(SCL_BIT);   // enable pull up on TWI clock line
	SDA_PORT |= _BV(SDA_BIT);   // enable pull up on TWI data line
}

/**
 * @ingroup I2C
 *
 */
void avr_i2c_end(void)
{
	// TODO avr_i2c_end
}

/**
 * @ingroup I2C
 *
 * @param addr
 */
void avr_i2c_setSlaveAddress(const uint8_t addr)
{
	slave_address = addr << 1;
}

/**
 * @ingroup I2C
 *
 * @param buf
 * @param len
 * @return
 */
uint8_t avr_i2c_write(const char *buf, uint8_t len)
{
	if (i2c_start(slave_address | I2C_WRITE))
		return 1;

	while (len--)
	{
		if (i2c_write(*buf++))
			return 1;
	}

	i2c_stop();

	return 0;
}

/**
 * @ingroup I2C
 *
 * @param buf
 * @param len
 * @return
 */
uint8_t avr_i2c_read(char *buf, uint8_t len)
{
	if (i2c_start(slave_address | I2C_READ))
		return 1;

	while(len--) {
		*buf = i2c_read_ack();
		buf++;
	}

	i2c_stop();

	return 0;
}
