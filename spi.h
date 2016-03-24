#ifndef __SPI_H
#define __SPI_H

#include <avr/io.h>

#define MOSI    (1<<PB3)
#define MISO    (1<<PB4)
#define CS      (1<<PB2)
#define SCK     (1<<PB5)
#define CS_DDR  DDRB
#define CS_ENABLE() (PORTB &= ~CS)
#define CS_DISABLE() (PORTB |= CS)

/* Seutp SPI register */
extern void SPI_init(void);

/* SPI transfer send/receive data in the same time */
extern uint8_t SPI_transfer(uint8_t data);

#endif
