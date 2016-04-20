#ifndef __NMEA_H
#define __NMEA_H

#include <avr/io.h>

volatile int nmea_dataready;

void nmea_parse_data(char data);

#endif
