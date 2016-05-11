#ifndef __NMEA_H
#define __NMEA_H

#include <avr/io.h>

volatile int nmea_dataready;

/* Storage for all words in message */
char tmp_words[20][15];

void nmea_parse_data(char data);

void parse_message();
#endif
