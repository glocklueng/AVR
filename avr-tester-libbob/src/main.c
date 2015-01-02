/**
 * @file main.c
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

#include <util/delay.h>
#include "uart_stream.h"
#include "bw_spi.h"
#include "bw_spi_lcd.h"
#include "bw_i2c.h"
#include "bw_i2c_dio.h"
#include "bw_i2c_lcd.h"
#include "mcp7941x.h"

int main(void)
{
	device_info_t device_info;

	UART_BEGIN();

	device_info.slave_address = BW_LCD_DEFAULT_SLAVE_ADDRESS;

	/** SPI **/
	printf("bw_spi_lcd_start\n");
	bw_spi_lcd_start(&device_info);

	printf("bw_spi_read_id\n");
	bw_spi_read_id(&device_info);

	printf("bw_spi_lcd_reinit\n");
	bw_spi_lcd_reinit(&device_info);

	uint8_t backlight;
	printf("bw_spi_lcd_get_backlight := ");
	bw_spi_lcd_get_backlight(&device_info, &backlight);
	printf("%d\n", backlight);

	uint8_t contrast;
	printf("bw_spi_lcd_get_contrast := ");
	bw_spi_lcd_get_contrast(&device_info,&contrast);
	printf("%d\n", contrast);

	printf("bw_spi_lcd_cls\n");
	bw_spi_lcd_cls(&device_info);

	printf("bw_spi_lcd_text_line_1\n");
	bw_spi_lcd_text_line_1(&device_info, "AVR Atmel", 9);
	printf("bw_spi_lcd_text_line_2\n");
	bw_spi_lcd_text_line_2(&device_info, "Arduino Uno", 11);

	/** I2C **/
	printf("bw_i2c_lcd_start\n");
	bw_i2c_lcd_start(BW_LCD_DEFAULT_SLAVE_ADDRESS);

	printf("bw_i2c_read_id\n");
	bw_i2c_read_id(&device_info);

	printf("bw_i2c_lcd_reinit\n");
	bw_i2c_lcd_reinit();

	printf("bw_i2c_lcd_cls\n");
	bw_i2c_lcd_cls();

	printf("bw_i2c_lcd_text_line_1\n");
	bw_i2c_lcd_text_line_1("AVR Atmel", 9);

	/** RTC **/
	struct rtc_time tm_rtc;
	printf("mcp7941x_start\n");
	mcp7941x_start(0x00);
	printf("mcp7941x_get_date_time\n");
	mcp7941x_get_date_time(&tm_rtc);
	printf("%.2d:%.2d:%.2d %.2d-%.2d-%.2d\n", tm_rtc.tm_hour, tm_rtc.tm_min, tm_rtc.tm_sec, tm_rtc.tm_mday, tm_rtc.tm_mon, tm_rtc.tm_year);

	device_info.slave_address = BW_DIO_DEFAULT_SLAVE_ADDRESS;

	printf("bw_i2c_dio_start\n");
	bw_i2c_dio_start(&device_info);

	printf("bw_i2c_read_id\n");
	bw_i2c_read_id(&device_info);

	printf("bw_i2c_dio_fsel_mask\n");
	bw_i2c_dio_fsel_mask(&device_info, 0x7F);

	char buf[9];
	while (1)
	{
		mcp7941x_get_date_time(&tm_rtc);
		sprintf(buf, "%.2d:%.2d:%.2d", tm_rtc.tm_hour, tm_rtc.tm_min, tm_rtc.tm_sec);
		bw_i2c_lcd_text_line_2(buf, 8);

		device_info.slave_address = BW_DIO_DEFAULT_SLAVE_ADDRESS;
		bw_i2c_dio_output(&device_info, BW_DIO_PIN_IO0 | BW_DIO_PIN_IO2 | BW_DIO_PIN_IO4 | BW_DIO_PIN_IO6);
		_delay_ms(500);

		bw_i2c_dio_output(&device_info, BW_DIO_PIN_IO1 | BW_DIO_PIN_IO3 | BW_DIO_PIN_IO5);
		_delay_ms(500);
	}
}
