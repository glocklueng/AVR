/**
 * @file avr_i2c.c
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

#include <stdlib.h>
#include <util/twi.h>
#include "avr_i2c.h"

/// In 7-bit addressing procedure, the slave address is transferred in the first byte after the Start condition.
/// The first seven bits of the byte comprise the slave address.
/// The eighth bit is the read/write flag where 0 indicates a write and 1 indicates a read.
/// \ref I2C_READ is used in \ref avr_i2c_read.
/// \ref I2C_WRITE is used in \ref avr_i2c_write.
typedef enum
{
	I2C_READ = 0x01,	///< R/W Bit Read
	I2C_WRITE = 0x00	///< R/W Bit Write
} avrI2CReadWriteFlag;

#define F_SCL		100000UL	///< SCL frequency
#define PRESCALER 	1			///< 4^TWPS, TWPS is located in TWISR which is the same register as the TWI Status bits. TWPS should therefore always be set to 0 to simplify the handling of the TWI Status bits.

static uint8_t slave_address;	///< Set with function \ref avr_i2c_setSlaveAddress

/**
 * @ingroup I2C
 *
 * @param address
 * @return 0 on success, 1 in the event of a write error.
 */
static uint8_t i2c_start(uint8_t address)
{
	// reset TWI control register
	TWCR = 0;
	// transmit START condition
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	// wait for end of transmission
	loop_until_bit_is_set(TWCR, TWINT);

	// check if the start condition was successfully transmitted
	if ((TWSR & 0xF8) != TW_START)
		return 1;

	// load slave address into data register
	TWDR = address;
	// start transmission of address
	TWCR = (1 << TWINT) | (1 << TWEN);
	// wait for end of transmission
	loop_until_bit_is_set(TWCR, TWINT);

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
 * @return 0 on success, 1 in the event of a write error.
 */
static uint8_t i2c_write(uint8_t data)
{
	// load data into data register
	TWDR = data;
	// start transmission of data
	TWCR = (1 << TWINT) | (1 << TWEN);
	// wait for end of transmission
	loop_until_bit_is_set(TWCR, TWINT);

	if ((TWSR & 0xF8) != TW_MT_DATA_ACK)
		return 1;

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
	loop_until_bit_is_set(TWCR, TWINT);
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
	loop_until_bit_is_set(TWCR, TWINT);
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
 * Start I2C operations.
 * Pull up resistors are activated for TWI clock line / TWI data line.
 * Baudrate is set with \ref F_SCL
 */
void avr_i2c_begin(void)
{
	TWBR = ((((F_CPU / F_SCL) / PRESCALER) - 16) / 2);
	SCL_PORT |= _BV(SCL_BIT);   // enable pull up on TWI clock line
	SDA_PORT |= _BV(SDA_BIT);   // enable pull up on TWI data line
}

/**
 * @ingroup I2C
 * End I2C operations.
 */
void avr_i2c_end(void)
{
	TWCR = 0;
}

/**
 * @ingroup I2C
 * Sets the I2C slave 7-bits address.
 * @param addr The I2C slave address.
 */
void avr_i2c_setSlaveAddress(const uint8_t addr)
{
	// The first seven bits of the byte comprise the slave address.
	slave_address = addr << 1;
}

/**
 * @ingroup I2C
 * Transfers any number of bytes to the currently selected I2C slave
 * (as previously set by \ref avr_i2c_setSlaveAddress).
 * @param buf Buffer of bytes to send.
 * @param len Number of bytes in the @p buf buffer, and the number of bytes to send.
 * @return 0 on success, 1 in the event of a write error.
 */
uint8_t avr_i2c_write(const char *buf, uint8_t len)
{
	// The eighth bit is the read/write flag where 0 indicates a write.
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
 * Transfers any number of bytes from the currently selected I2C slave
 * (as previously set by \ref avr_i2c_setSlaveAddress).
 * @param buf Buffer of bytes to receive.
 * @param len Number of bytes in the @p buf buffer, and the number of bytes to received.
 * @return 0 on success, 1 in the event of a read error.
 */
uint8_t avr_i2c_read(char *buf, uint8_t len)
{
	// The eighth bit is the read/write flag where 1 indicates a read.
	if (i2c_start(slave_address | I2C_READ))
		return 1;

	while(len--) {
		*buf = i2c_read_ack();
		buf++;
	}

	i2c_stop();

	return 0;
}
