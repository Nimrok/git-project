/*
 * 30.05.2018 Для блока радиоуправляемого реле
 */
//--------------------------------------------------/
#include <Wire.h>
#include <RTC.h>
#include "nrf24.c"
#include "nrf24.h"
#include "nRF24L01.h"
#include "radioPinFunctions.c"
//--------------------------------------------------/
#define relay 7
RTC time;
//--------------------------------------------------/
char radioflag;
char radio_buf[4];
char tx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7}; //куда отправлять
char rx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7}; //адрес этого устройства
//==================================================/
void write24(int deviceaddress, unsigned int eeaddress, byte data ) 
{
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(data);
  Wire.endTransmission();
  delay(5);
}
//--------------------------------------------------/ 
byte read24(int deviceaddress, unsigned int eeaddress ) 
{
  byte rdata = 0xFF;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress,1);
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}
//==================================================/
void setup() 
{
  Serial.begin(9600);
  nrf24_init();     
  nrf24_config(2,4); //второй канал, длина пакета 4 байта
  nrf24_tx_address(tx_address);
  nrf24_rx_address(rx_address);
  time.begin(RTC_DS3231);    //здесь инициализируется i2c шина (F_CPU/2)
  Wire.begin();
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  write24(0x50, 0, 123);
}
//==================================================/
void loop() 
{
  if(nrf24_dataReady())
  {
    nrf24_getData(radio_buf);        
  }
//--------------------------------------------------/
  if(millis()%1000==0)
  {
    Serial.println(time.gettime("d-m-Y, H:i:s, D"));
    Serial.print(read24(0x50, 0), DEC); 
    delay(1);
  }
}
//==================================================/
