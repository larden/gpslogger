#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "uart.h"
#include "hd44780.h"

#define UART_BAUDRATE 4800UL
#define FOSC 16000000UL

#define LED_ON		PORTB |= (1<<PB0)
#define LED_OFF		PORTB &= ~(1<<PB0)

int main()
{
	uint16_t baudrate = UART_BAUD_SELECT(UART_BAUDRATE, FOSC);
    
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
	DDR |= (1<<PB0);

	/* Enable global interrupts */
	sei();

	for(;;) {
		LED_ON;
		_delay_ms(500);
		LED_OFF;
		_delay_ms(500);		
	}

	return 0;
}

