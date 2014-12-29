/*
 * avr_i2c.h
 *
 *  Created on: Dec 29, 2014
 *      Author: pi
 */

#ifndef AVR_I2C_H_
#define AVR_I2C_H_

#include <stdint.h>

extern void avr_i2c_begin(void);
extern void avr_i2c_setSlaveAddress(const uint8_t);
extern void avr_i2c_setClockDivider(const uint16_t);
extern uint8_t avr_i2c_write(const char *, const uint32_t);
extern uint8_t avr_i2c_read(char*, const uint32_t);

inline static void avr_i2c_end(void)
{
}

#endif /* AVR_I2C_H_ */
