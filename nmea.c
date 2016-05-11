#include "nmea.h"
#include "uart.h"

/* pointer for words */
static volatile int idx;
/* to keep position in words */
static volatile int prev_idx;
static volatile int now_idx;

//Function starts parse data byte by byte when event UART gets '$' character
void nmea_parse_data(char data)
{
    if (data == '$') {
        //init parse parameters
        nmea_dataready = 0;
        
        idx = 0;
        prev_idx = 0;
        now_idx = 0;
    }

    if (new_msg == 1) {
        if (data == '\r' || data == '\n')
        {
            tmp_words[idx][now_idx - prev_idx] = 0;
            idx++;
            parse_message();
        } else {
            tmp_words[idx][now_idx-prev_idx] = data;
            if (data == ',') {
                tmp_words[idx][now_idx-prev_idx] = 0;
                idx++;
                prev_idx=now_idx;
            }
            else now_idx++;
                
        }
    
    }
}

void parse_message()
{
    //check if message starts with $GPGGA
    //if yes then parse massage
}
