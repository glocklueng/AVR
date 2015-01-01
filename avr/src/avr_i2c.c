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
#include <avr/interrupt.h>
#include <util/twi.h>
#include "i2c.h"
#include "avr_i2c.h"

static unsigned char slave_address;		///<

/**
 * @ingroup I2C
 *
 */
void avr_i2c_begin(void)
{
	i2c_init();
	SCL_PORT |= _BV(SCL_BIT);   // enable pull up on TWI clock line
	SDA_PORT |= _BV(SDA_BIT);   // enable pull up on TWI data line
}


/**
 * @ingroup I2C
 *
 */
void avr_i2c_end(void)
{

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
	uint8_t rc;
	rc = i2c_start(slave_address | I2C_WRITE);
	if (rc)
		return rc;

	while (len--)
	{
		rc = i2c_write(*buf++);
		if (rc)
			return rc;
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
	uint8_t rc;
	rc = i2c_start(slave_address | I2C_READ);
	if (rc)
		return rc;

	while(len--) {
		*buf = i2c_read_ack();
		buf++;
	}
	i2c_stop();
	return 0;
}
