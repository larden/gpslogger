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
volatile uint8_t ascii_line; 

static volatile uint8_t UART_RxBuf[UART_RX_BUFFER_SIZE];
static volatile uint8_t UART_RxHead;
static volatile uint8_t UART_RxTail;

/* Pointer for callback function for event UART_STR_EVENT() */
static void (*uart_str_event_callback)(char *buf);

void register_uart_str_event_callback(void (*callback)(char *buf))
{
    uart_str_event_callback = callback;
}

void UART_STR_EVENT(char *buf)
{
    if (ascii_line) {
        if (uart_str_event_callback) {
            uart_gets(buf);
            (*uart_str_event_callback)(buf);
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
        
    /* calculate buffer index */ 
    tmphead = (UART_RxHead + 1) & UART_RX_BUFFER_MASK;
    
    if (tmphead == UART_RxTail) {
        /* error: receive buffer overflow */
        uart_flush();
    } else {
        switch (data) {
            case 0:                 /* ignore byte 0 */
            case 10: break;         /* ignore LF */
            case 13: ascii_line++;  /* new line in buffor */
            default:   /* store new index */
                    UART_RxHead = tmphead;
                    /* store received data in buffer */
                    UART_RxBuf[tmphead] = data;
        }
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

int uart_getc(void)
{
	if ( UART_RxHead == UART_RxTail ) {
		return -1;   /* no data available */
	}

	/* calculate /store buffer index */
	UART_RxTail = (UART_RxTail + 1) & UART_RX_BUFFER_MASK;

	return UART_RxBuf[UART_RxTail];
}

char *uart_gets(char *buf)
{
    char c;
    char *p = buf;
    
    if (ascii_line) {
        while ((c = uart_getc())) {
            if (c == 13 || c < 0)
                break;
            *buf++ = c;
        }

        *buf = 0;
        ascii_line--;
    }

    return p;
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
