#include <util/delay.h>
#include <uart.h>
#include <bw_spi_lcd.h>
#include <bw_spi_dio.h>

int main(void)
{
	device_info_t device_info;

	device_info.slave_address = BW_LCD_DEFAULT_SLAVE_ADDRESS;

	UART_BEGIN();

	printf("bw_spi_lcd_start\n");
	bw_spi_lcd_start(&device_info);

	printf("bw_spi_lcd_reinit\n");
	bw_spi_lcd_reinit(&device_info);

	printf("bw_spi_lcd_cls\n");
	bw_spi_lcd_cls(&device_info);

	printf("bw_spi_lcd_text_line_1\n");
	bw_spi_lcd_text_line_1(&device_info, "AVR Atmel", 9);
	printf("bw_spi_lcd_text_line_2\n");
	bw_spi_lcd_text_line_2(&device_info, "Arduino Uno", 11);

	device_info.slave_address = BW_DIO_DEFAULT_SLAVE_ADDRESS;

	printf("bw_spi_dio_start\n");
	bw_spi_dio_start(&device_info);

	printf("bw_spi_dio_fsel_mask\n");
	bw_spi_dio_fsel_mask(&device_info, 0x7F);

	while (1)
	{
		printf("bw_spi_dio_output(&device_info, BW_DIO_PIN_IO0 | BW_DIO_PIN_IO2 | BW_DIO_PIN_IO4 | BW_DIO_PIN_IO6)\n");
		bw_spi_dio_output(&device_info, BW_DIO_PIN_IO0 | BW_DIO_PIN_IO2 | BW_DIO_PIN_IO4 | BW_DIO_PIN_IO6);
		_delay_ms(1000);
		printf("bw_spi_dio_output(&device_info, BW_DIO_PIN_IO1 | BW_DIO_PIN_IO3 | BW_DIO_PIN_IO5)\n");
		bw_spi_dio_output(&device_info, BW_DIO_PIN_IO1 | BW_DIO_PIN_IO3 | BW_DIO_PIN_IO5);
		_delay_ms(1000);
	}
}
