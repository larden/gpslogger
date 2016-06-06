#include <avr/pgmspace.h>
#include "nmea.h"

void nmea_parse_uart(uint8_t *data)
{
	uint8_t *c = data;
	
	while(1)
	{
		if (*c == '$') {
			flagRead = 1;	
			wordIdx = 0;
			prevIdx = 0;
			nowIdx = 0;
		}

		if (flagRead) {
			if (*c == '\r' || *c == '\n') {
				words[wordIdx][nowIdx-prevIdx] = 0;
				wordIdx++;
				flagRead = 0;
				flagDataReady = 1;
				break;
			} else {
			
			}
			
		}
	}
}
