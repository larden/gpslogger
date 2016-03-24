#include <avr/io.h>
#include "spi.h"

extern void SPI_init(void)
{
    /* Set MOSI and SCK output, all others input */
    DDR_SPI = (1<<DD_MOSI) | (1<<DD_SCK);
    
    /* Enable interrupt, SPI, MASTER, set clock rate fck/16 */
    SPCR = (1<<SPIE) | (1<<SPE) | (1<<MSTR) | (1<<SPR0);
}
