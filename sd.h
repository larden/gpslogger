#ifndef __SD_H
#define __SD_H

#include <avr/io.h>

#define CS		(1<<PB2)
#define CS_DDR		DDRB
#define CS_ENABLE()	(PORTB &= ~CS)
#define CS_DISABLE()	(PORTB |= CS)

/* Send command to SD card via SPI interface */
extern void SD_command(uint8_t cmd, uint32_t arg, uint8_t crc, uint8_t read);

#endif
