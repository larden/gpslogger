#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <stdlib.h>

#include "uart.h"
#include "hd44780.h"
#include "spi.h"
#include "nmea.h"

#define UART_BAUDRATE 9600UL
#define FOSC 16000000UL

#define LED_ON		PORTB |= (1<<PB0)
#define LED_OFF		PORTB &= ~(1<<PB0)

volatile static unsigned int packets;

void parse_gps_message(uint8_t *msg)
{
	uint8_t *token;
	uint8_t number[16];

	packets++;
	//token = strtok(msg, ",");
	strlcpy(number, msg, 15);

	LCD_Clear();
	LCD_Home();
	LCD_WriteText(number);
	LCD_GoTo(0,1);
	itoa(packets, number, 10);
	LCD_WriteText(number);
}

void display_cordinate()
{

}

int main()
{
	uint16_t baudrate = UART_BAUD_SELECT(UART_BAUDRATE, FOSC);

	uint8_t buffor[128];

	packets = 0;

	//register_uart_str_event_callback(parse_gps_message);
	register_uart_str_event_callback(nmea_parse_uart);

	/* Setup UART - only receiver */ 
	uart_init(baudrate);

	/* Setup SPI */
	SPI_init();

	/* Setup HD44870 */
	LCD_Initalize();
	LCD_Clear();
	LCD_Home();

	LCD_WriteText("GPS Logger");

	/* wait for gps module */
	_delay_ms(10000);

	/* Set pin PB0 as output for LED */
	//DDRB |= (1<<PB0);

	/* Enable global interrupts */
	sei();

	for(;;) {
		LCD_Clear();
		LCD_Home();

		UART_STR_EVENT(buffor);

		if (flagDataReady) {
			LCD_WriteText(words[0]);
		} else {
			LCD_WriteText("No GPS data, fix 0");
		}
		_delay_ms(50);

	}

	return 0;
}

