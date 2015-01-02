/**
 * @defgroup Firmware firmware
 * @brief Atmel AVR 8-bit Microcontrollers
 *
 * The AVR is a modified Harvard architecture 8-bit RISC single-chip microcontroller,
 * which was developed by Atmel in 1996. The AVR was one of the first microcontroller families to
 * use on-chip flash memory for program storage, as opposed to one-time programmable ROM, EPROM,
 * or EEPROM used by other microcontrollers at the time.
 * <br>
 * http://en.wikipedia.org/wiki/Atmel_AVR
 */

/**
 * @defgroup I2C i2c
 * @ingroup Firmware
 * @brief The Two Wire serial Interface (TWI) is compatible with Philips I2C protocol.
 *
 * The bus was developed to allow simple, robust and cost effective communication
 * between integrated circuits in electronics.
 * The strengths of the TWI bus includes the capability of addressing up to 128 devices
 * on the same bus, arbitration, and the possibility to have multiple masters on the bus.
 * A hardware TWI module is included in most of the AVR devices available.
 * <br>
 * <b>AVR315: Using the TWI module as I2C master</b>
 * <br>
 * http://www.atmel.com/images/doc2564.pdf
 */

/**
 * @defgroup SPI spi
 * @ingroup Firmware
 * @brief The Serial Peripheral Interface or SPI bus is a synchronous serial data link, that operates in full duplex mode.
 *
 * The SPI allows high-speed synchronous data transfer between the AVR and peripheral devices or between several AVR devices.
 * The interconnection between two SPI devices always happens between a master device and a slave device.
 * <br>
 * <b>AVR151: Setup And Use of The SPI</b>
 * <br>
 * http://www.atmel.com/images/doc2585.pdf
 */


/**
 * @defgroup UART UART
 * @ingroup Firmware
 * @brief Polled UART
 *
 * The application is continuously checking the UDRE bit in the UART Status Register to
 * control when the UART has finished sending a byte. When receiving data,
 * the application is continuously checking the RXC bit in the UART Status Register to
 * control when the UART has completed receiving a byte.
 *
 * <br>
 * <b>AVR306: Using the AVR UART in C<b>
 * <br>
 * http://www.atmel.com/Images/doc1451.pdf
 */

/**
 * @defgroup UART-STREAM UART stream
 * @ingroup UART
 * @brief Redirecting STDIN and STDOUT to UART
 *
 * Based on http://www.appelsiini.net/2011/simple-usart-with-avr-libc
 *
 */
