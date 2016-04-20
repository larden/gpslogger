#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "uart.h"

/*
 *  constants and macros
 */

/* size of RX/TX buffers */
#define UART_RX_BUFFER_MASK ( UART_RX_BUFFER_SIZE - 1)

#define UART_RECEIVE_INTERRUPT   USART_RX_vect
#define UART_STATUS   UCSR0A
#define UART_CONTROL  UCSR0B
#define UART_DATA     UDR0
#define UART_UDRIE    UDRIE0
#define UART_8BIT     0x011

/*
 *  Module global variables
 */
volatile int new_msg; 

static volatile char UART_RxBuf[UART_RX_BUFFER_SIZE];
static volatile char UART_RxHead;
static volatile char UART_RxTail;

/* Pointer for callback function for event UART_STR_EVENT() */
static void (*uart_str_event_callback)(char c);

void register_uart_str_event_callback(void (*callback)(char c))
{
    uart_str_event_callback = callback;
}

void UART_STR_EVENT()
{
    char character;

    if (new_msg) {
        if (uart_str_event_callback) {
            character = uart_getc();
            (*uart_str_event_callback)(character);
        } else {
            uart_flush();
        }
    }
}

ISR(UART_RECEIVE_INTERRUPT)
{
    register uint8_t tmphead;
    register uint8_t data;
 
    /* read UART data register */ 
    data = UART_DATA;

    if (data == '$') {
        /* ignore bytes before */
        uart_flush();
        
        /* event that new messege is receiving */ 
        new_msg = 1;
    } else if (data == '\n' || data == '\r') {
        new_msg = 0;
    }

    /* calculate buffer index */ 
    tmphead = (UART_RxHead + 1) & UART_RX_BUFFER_MASK;

    if (tmphead == UART_RxTail) {
        /* error: receive buffer overflow */
        uart_flush();
    } else {
        /* store new buffer head */
        UART_RxHead = tmphead;
        /* store received data in buffer */
        UART_RxBuf[tmphead] = data;
    }
}


/* initialize UART and set baudrate */
void uart_init(uint16_t baudrate)
{
	UART_RxHead = 0;
	UART_RxTail = 0;

    /* asynchronous UART, 8-bit data, 1-bit stop, no parity */
	UCSR0C = (UART_8BIT<<UCSZ00);

	UBRR0H = (uint8_t)(baudrate>>8);
	UBRR0L = (uint8_t) baudrate;

	/* Enable USART receiver and transmitter and receive complete interrupt */
	UART_CONTROL = (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);
}

char uart_getc(void)
{
	if ( UART_RxHead == UART_RxTail ) {
		return -1;   /* no data available */
	}

	/* calculate /store buffer index */
	UART_RxTail = (UART_RxTail + 1) & UART_RX_BUFFER_MASK;

	return UART_RxBuf[UART_RxTail];
}

/* Determine the number of bytes waiting in the receive buffer */
int uart_available(void)
{
	return (UART_RX_BUFFER_SIZE + UART_RxHead - UART_RxTail) & UART_RX_BUFFER_MASK;
}

/* Flush bytes waiting the receive buffer.  Acutally ignores them. */
void uart_flush(void)
{
	UART_RxHead = UART_RxTail;
}
