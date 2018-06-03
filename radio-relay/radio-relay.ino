
/*
 * 30.05.2018 Для блока радиоуправляемого реле
 */
//--------------------------------------------------/
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <RTC.h>
#include "nrf24.c"
#include "nrf24.h"
#include "nRF24L01.h"
#include "radioPinFunctions.c"
#include <Pointer.h>
//--------------------------------------------------/
#define relay 7
#define buzzer 6
Pointer relay1;
Pointer button;
Pointer btn_delay;
RTC time;
LiquidCrystal_I2C lcd(0x3F,16,2);
//--------------------------------------------------/
char radioflag;
char radio_buf[4];
char tx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7}; //куда отправлять
char rx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7}; //адрес этого устройства
int value = 0;
char buzz_flag = 0;
unsigned long buzz_start = 0;
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
  lcd.init();                     
  lcd.backlight();
  nrf24_init();     
  nrf24_config(2,4); //второй канал, длина пакета 4 байта
  nrf24_tx_address(tx_address);
  nrf24_rx_address(rx_address);
  time.begin(RTC_DS3231);    //здесь инициализируется i2c шина (F_CPU/2)
  Wire.begin();
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
}
//==================================================/
void loop() 
{
  if(nrf24_dataReady())
  {
    nrf24_getData(radio_buf);        
  }
//--------------------------------------------------/
  if (relay1.point(1000))
  {
    PORTD ^= (1 << PD7);
  }
//--------------------------------------------------/
  if (button.point(100))
  {
    value=analogRead(1);
    if ((value > 800) && (value < 830))
    {
      lcd.setCursor(0,0);
      lcd.print("----------");
      lcd.setCursor(0,0);
      lcd.print("none_btn");
      lcd.setCursor(0,1);
      lcd.print(time.gettime("d-m H:i:s"));
    }
    else if ((value > 750) && (value < 790)){  
      lcd.setCursor(0,0);  
      lcd.print("----------");
      lcd.setCursor(0,0);
      lcd.print("select_btn");
      if (btn_delay.point(300))
      {
        digitalWrite(buzzer,HIGH);
        buzz_flag = 1;
        buzz_start=millis();
      }
    }
    else if ((value > 660) && (value < 700)){
      lcd.setCursor(0,0);
      lcd.print("----------");
      lcd.setCursor(0,0);
      lcd.print("left_btn");
      if (btn_delay.point(300))
      {
        digitalWrite(buzzer,HIGH);
        buzz_flag = 1;
        buzz_start=millis();
      }
    }
    else if ((value > 500) && (value < 530)){
      lcd.setCursor(0,0);
      lcd.print("----------");
      lcd.setCursor(0,0);
      lcd.print("right_btn");
      if (btn_delay.point(300))
      {
        digitalWrite(buzzer,HIGH);
        buzz_flag = 1;
        buzz_start=millis();
      }
    }
    else if (value < 30){
      lcd.setCursor(0,0);
      lcd.print("----------");
      lcd.setCursor(0,0);
      lcd.print("cancel_btn");
      if (btn_delay.point(300))
      {
        digitalWrite(buzzer,HIGH);
        buzz_flag = 1;
        buzz_start=millis();
      }
    }
    else {
      lcd.setCursor(0,0);
      lcd.print("----------");
      lcd.setCursor(0,0);
      lcd.print("error_btn");}
  }
//--------------------------------------------------/
  if ((millis()-buzz_start > 100)&&(buzz_flag == 1))
  {
    buzz_flag = 0;
    digitalWrite(buzzer, LOW);
  }
//--------------------------------------------------/
}
//==================================================/


