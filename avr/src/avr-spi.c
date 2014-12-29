#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "spi.h"
#include "avr_spi.h"

void avr_spi_begin(void)
{
	DDRB |= (1 << SPI_MOSI_PIN); 	// output
	DDRB &= ~(1 << SPI_MISO_PIN);	// input
	DDRB |= (1 << SPI_SCK_PIN);		// output
	DDRB |= (1 << SPI_SS_PIN);		// output

	PORTB |= (1 << SPI_SS_PIN);

	SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR1) | _BV(SPR0);
}

void avr_spi_writenb(const char *s, uint8_t len)
{
	PORTB &= ~(1 << SPI_SS_PIN);

	while (len--)
	{
		SPDR = *s++;
		while (!(SPSR & _BV(SPIF)))
			;
		_delay_us(10);
	}
	PORTB |= (1 << SPI_SS_PIN);
}


