#ifndef NMEA_H
#define NMEA_H


#include <stdint.h>
#include <avr/io.h>

void nmea_parse_uart(uint8_t *data);

uint8_t words[20][15];

uint8_t flagRead;
uint8_t flagDataReady;

uint8_t wordIdx;
uint8_t prevIdx;
uint8_t nowIdx;


int latitude;
int longtitude;

#endif // UART_H

