#include "nrf24.c"
#include "nrf24.h"
#include "nRF24L01.h"
#include "radioPinFunctions.c"
char radioflag;
char radio_buf[4];
char tx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};
char rx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};

void setup() 
{
  Serial.begin(9600);
 nrf24_init();     
 nrf24_config(2,4);
 nrf24_tx_address(tx_address);
 nrf24_rx_address(rx_address);
}

void loop() 
{
   if(nrf24_dataReady())
        {
            nrf24_getData(radio_buf);        
            Serial.write(radio_buf[0]);
            Serial.write(radio_buf[1]);
            Serial.write(radio_buf[2]);
            Serial.write(radio_buf[3]);
        radio_buf[0] = 0;
        radio_buf[1] = 0;
        radio_buf[2] = 0;
        radio_buf[3] = 0;
        Serial.write(radio_buf[0]);
            Serial.write(radio_buf[1]);
            Serial.write(radio_buf[2]);
            Serial.write(radio_buf[3]);
        }
}

