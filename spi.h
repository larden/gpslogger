#ifndef __SPI_H
#define __SPI_H

#include <avr/io.h>

#define MOSI    (1<<PB3)
#define MISO    (1<<PB4)
#define SCK     (1<<PB5)

/* Seutp SPI register */
extern void SPI_init(void);

/* SPI transfer send/receive data in the same time */
extern uint8_t SPI_transfer(uint8_t data);

#endif
