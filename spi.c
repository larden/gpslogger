#include <avr/io.h>
#include <avr/interrupt.h>
#include "spi.h"


void SPI_init(void)
{
    /* Set MOSI and SCK output, all others input */
    DDRB = MOSI | SCK;
    
    /* Enable, SPI, MASTER, set clock rate fck/16 */
    SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0);
}

uint8_t SPI_transfer(uint8_t data)
{
    /* Start transmission */
    SPDR = data;

    /* Wait for transmission complete */
    while(!(SPSR & (1<<SPIF)))
        ;
    return SPDR;
}
