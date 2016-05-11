#include <avr/pgmspace.h>
#include "nmea.h"
#include "uart.h"

int mstrcmp(const char *s1, const char *s2)
{
	while ((*s1 && *s2) && (*s1 == *s2)) {
		s1++;
		s2++;
	}
	return *s1 - *s2;
}

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
				nmea_parse_gpgga();
				break;
			} else {
				words[wordIdx][nowIdx - prevIdx] = *c;
				if (*c == ',') {
					words[wordIdx][nowIdx - prevIdx] = 0;
					wordIdx++;
					prevIdx = nowIdx;
				}
				nowIdx++;
			}

		}
		c++; //next character in buffer
	}
}

	/* $GPGGA
	 * $GPGGA,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh
	 * ex: $GPGGA,230600.501,4543.8895,N,02112.7238,E,1,03,3.3,96.7,M,39.0,M,,0000*6A,
	 *
	 * WORDS:
	 *  1    = UTC of Position
	 *  2    = Latitude
	 *  3    = N or S
	 *  4    = Longitude
	 *  5    = E or W
	 *  6    = GPS quality indicator (0=invalid; 1=GPS fix; 2=Diff. GPS fix)
	 *  7    = Number of satellites in use [not those in view]
	 *  8    = Horizontal dilution of position
	 *  9    = Antenna altitude above/below mean sea level (geoid)
	 *  10   = Meters  (Antenna height unit)
	 *  11   = Geoidal separation (Diff. between WGS-84 earth ellipsoid and mean sea level.
	 *      -geoid is below WGS-84 ellipsoid)
	 *  12   = Meters  (Units of geoidal separation)
	 *  13   = Age in seconds since last update from diff. reference station
	 *  14   = Diff. reference station ID#
	 *  15   = Checksum
	 */
void nmea_parse_gpgga()
{
	if (mstrcmp(words[0], "$GPGGA") == 0) {
		if(words[6][0] == '0') {
			latitude = 0;
			longtitude = 0;
			flagDataReady = 0;
		}

		//parse GPS data


		flagDataReady = 1;
	}
}
