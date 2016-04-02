#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "uart.h"

/*
 *  constants and macros
 */

/* size of RX/TX buffers */
#define UART_RX_BUFFER_MASK ( UART_RX_BUFFER_SIZE - 1)
#define UART_TX_BUFFER_MASK ( UART_TX_BUFFER_SIZE - 1)

#define UART_RECEIVE_INTERRUPT   USART_RX_vect
#define UART_TRANSMIT_INTERRUPT  USART_UDRE_vect
#define UART_STATUS   UCSR0A
#define UART_CONTROL  UCSR0B
#define UART_DATA     UDR0
#define UART_UDRIE    UDRIE0
#define UART_8BIT     0x011
/*
 *  Module global variables
 */
static volatile uint8_t UART_RxBuf[UART_RX_BUFFER_SIZE];
		
static volatile uint8_t UART_RxHead;
static volatile uint8_t UART_RxTail;
static volatile uint8_t UART_LastRxError;
		

ISR(UART_RECEIVE_INTERRUPT)
/*************************************************************************
Function: UART Receive Complete interrupt
Purpose:  called when the UART has received a character
**************************************************************************/
{
    uint16_t tmphead;
    uint8_t data;
    uint8_t usr;
    uint8_t lastRxError;
 
    /* read UART status register and UART data register */ 
    usr  = UART_STATUS;
    data = UART_DATA;
    
    lastRxError = (usr & (_BV(FE0)|_BV(DOR0)) );
        
    /* calculate buffer index */ 
    tmphead = ( UART_RxHead + 1) & UART_RX_BUFFER_MASK;
    
    if ( tmphead == UART_RxTail ) {
        /* error: receive buffer overflow */
        lastRxError = UART_BUFFER_OVERFLOW >> 8;
    } else {
        /* store new index */
        UART_RxHead = tmphead;
        /* store received data in buffer */
        UART_RxBuf[tmphead] = data;
    }
    UART_LastRxError = lastRxError;   
}


/* initialize UART and set baudrate */
void UART_init(uint16_t baudrate)
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

/* 
return byte from ringbuffer
Returns:  lower byte:  received byte from ringbuffer
          higher byte: last receive error
*/
uint16_t UART_getc(void)
{
	uint16_t tmptail;
	uint8_t data;

	if ( UART_RxHead == UART_RxTail ) {
		return UART_NO_DATA;   /* no data available */
	}

	/* calculate /store buffer index */
	tmptail = (UART_RxTail + 1) & UART_RX_BUFFER_MASK;
	UART_RxTail = tmptail;

	/* get data from receive buffer */
	data = UART_RxBuf[tmptail];

	return (UART_LastRxError << 8) + data;

}

/* Returns the next byte (character) of incoming UART data without
 * removing it from the ring buffer. That is, successive calls to
 * uartN_peek() will return the same character, as will the next
 * call to uartN_getc()
*/

uint16_t UART_peek(void)
{
	uint16_t tmptail;
	uint8_t data;

	if ( UART_RxHead == UART_RxTail ) {
		return UART_NO_DATA;   /* no data available */
	}

	tmptail = (UART_RxTail + 1) & UART_RX_BUFFER_MASK;

	/* get data from receive buffer */
	data = UART_RxBuf[tmptail];

	return (UART_LastRxError << 8) + data;

}

/* Determine the number of bytes waiting in the receive buffer */
uint16_t UART_available(void)
{
	return (UART_RX_BUFFER_SIZE + UART_RxHead - UART_RxTail) & UART_RX_BUFFER_MASK;
}

/* Flush bytes waiting the receive buffer.  Acutally ignores them. */
void UART_flush(void)
{
	UART_RxHead = UART_RxTail;
}

