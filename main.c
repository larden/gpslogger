#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "uart.h"
#include "hd44780.h"

#define UART_BAUDRATE 4800UL
#define FOSC 16000000UL

#define LED_ON		PORTB |= (1<<PB0)
#define LED_OFF		PORTB &= ~(1<<PB0)

void parse_gps_message(char *msg)
{

}

int main()
{
	uint16_t baudrate = UART_BAUD_SELECT(UART_BAUDRATE, FOSC);
    
    char buffor[128];

    register_uart_str_event_callback(parse_gps_message);

	/* Setup UART - only receiver */ 
	UART_init(baudrate);

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
        /*
		LED_ON;
		_delay_ms(500);
		LED_OFF;
		_delay_ms(500);*/
        UART_STR_EVENT(buffor);

	}

	return 0;
}

