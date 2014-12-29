#include <util/delay.h>

#include <bw_spi_lcd.h>
#include <bw_spi_dio.h>

int main(void)
{
	device_info_t device_info;

	device_info.slave_address = BW_LCD_DEFAULT_SLAVE_ADDRESS;
	device_info.chip_select = 0;

	bw_spi_lcd_start(&device_info);

	bw_spi_lcd_reinit(&device_info);

	bw_spi_lcd_cls(&device_info);

	bw_spi_lcd_text_line_1(&device_info, "AVR_Atmel", 9);
	bw_spi_lcd_text_line_2(&device_info, "Arduino Uno", 11);

	device_info.slave_address = BW_DIO_DEFAULT_SLAVE_ADDRESS;

	bw_spi_dio_start(&device_info);

	bw_spi_dio_fsel_mask(&device_info, 0x7F);

	while (1)
	{
		bw_spi_dio_output(&device_info, BW_DIO_PIN_IO0 | BW_DIO_PIN_IO2 | BW_DIO_PIN_IO4 | BW_DIO_PIN_IO6);
		_delay_ms(1000);
		bw_spi_dio_output(&device_info, BW_DIO_PIN_IO1 | BW_DIO_PIN_IO3 | BW_DIO_PIN_IO5);
		_delay_ms(1000);
	}
}
