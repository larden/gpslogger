#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <stdlib.h>

#include "uart.h"
#include "hd44780.h"
#include "spi.h"

#define UART_BAUDRATE 9600UL
#define FOSC 16000000UL

#define LED_ON		PORTB |= (1<<PB0)
#define LED_OFF		PORTB &= ~(1<<PB0)

volatile static unsigned int packet;

void parse_gps_message(char *msg)
{
	char *token;
	char number[5];

	token = strtok(msg, ",");
	
	if (strcmp(token, "$GPGGA") == 0) {
		packet++;
		LCD_Clear();
		LCD_Home();
		LCD_WriteText("Odebrano $GPGGA");
		LCD_GoTo(0,1);
		itoa(packet, number, 10);
		LCD_WriteText(number);
	}
}

void display_cordinate()
{

}

int main()
{
	uint16_t baudrate = UART_BAUD_SELECT(UART_BAUDRATE, FOSC);

	char buffor[128];

	packet = 0;

	register_uart_str_event_callback(parse_gps_message);

	/* Setup UART - only receiver */ 
	uart_init(baudrate);

	/* Setup SPI */
	SPI_init();

	/* Setup HD44870 */
	LCD_Initalize();
	LCD_Clear();
	LCD_Home();

	LCD_WriteText("GPS Logger");

	/* Set pin PB0 as output for LED */
	//DDRB |= (1<<PB0);

	/* Enable global interrupts */
	sei();

	for(;;) {
		LED_ON;
		_delay_ms(500);
		LED_OFF;
		_delay_ms(500);
		UART_STR_EVENT(buffor);

		//display_cordinate();

	}

	return 0;
}

