#ifndef UART_H
#define UART_H


#include <stdint.h>
#include <avr/io.h>


/* Size of receiver/transmiter buffer */
#define UART_RX_BUFFER_SIZE  128

#define UART_BAUD_SELECT(baudRate,xtalCpu) (((xtalCpu)+8UL*(baudRate))/(16UL*(baudRate))-1UL)

extern volatile uint8_t ascii_line;

void uart_init(uint16_t baudrate);

uint8_t uart_getc(void);

uint8_t *uart_gets(uint8_t *buf);

int uart_available(void);

void uart_flush(void);

void UART_STR_EVENT(uint8_t *buf);
void register_uart_str_event_callback(void (*callback)(uint8_t * buf)); 

#endif // UART_H 

