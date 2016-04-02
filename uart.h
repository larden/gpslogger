#ifndef UART_H
#define UART_H


#include <stdint.h>
#include <avr/io.h>


/* Size of receiver/transmiter buffer */
#define UART_RX_BUFFER_SIZE 128 
#define UART_TX_BUFFER_SIZE 128


#define UART_BAUD_SELECT(baudRate,xtalCpu) (((xtalCpu)+8UL*(baudRate))/(16UL*(baudRate))-1UL)

/* 
** high byte error return code of uart_getc()
*/
#define UART_FRAME_ERROR      0x0800              /**< Framing Error by UART       */
#define UART_OVERRUN_ERROR    0x0400              /**< Overrun condition by UART   */
#define UART_BUFFER_OVERFLOW  0x0200              /**< receive ringbuffer overflow */
#define UART_NO_DATA          0x0100              /**< no receive data available   */

extern void UART_init(uint16_t baudrate);

extern uint16_t UART_getc(void);

extern uint16_t UART_peek(void);

extern uint16_t UART_available(void);

extern void UART_flush(void);

#endif // UART_H 

