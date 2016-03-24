#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "uart.h"

#define UART_BAUDRATE 4800UL
#define FOSC 16000000UL


int main()
{
    uint16_t baudrate = UART_BAUD_SELECT(UART_BAUDRATE, FOSC);
    
    /* Initialze UART - only receiver */ 
    UART_init(baudrate);


	for(;;) {
    
    }
    return 0;
}
