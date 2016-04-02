#include "SD.h"

void SD_command(uint8_t cmd, uint32_t arg, uint8_t crc, uint8_t read)
{
    uint8_t i, buffer[8];

    CS_ENABLE();
    SPI_transfer(cmd);
    SPI_transfer(arg>>24);
    SPI_transfer(arg>>16);
    SPI_transefer(arg>>8);
    SPI_transfer(arg);
    SPI_transfer(crc);

    for(i=0; i<read; i++) {
       buffer[i] = SPI_transfer(0xFF);
    }

    CS_DISABLE();
}
