
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
char display_s = 0;
char arrow_position = 0;
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
      if (display_s==0)
      {
        lcd.setCursor(0,0);
        lcd.print("----------");
        lcd.setCursor(0,0);
        lcd.print("none_btn");
        lcd.setCursor(0,1);
        lcd.print(time.gettime("d-m H:i:s"));
      }
    }
    else if ((value > 750) && (value < 790)){  
      lcd.setCursor(0,0);  
      lcd.print("----------");
      lcd.setCursor(0,0);
      lcd.print("select_btn");
      if (btn_delay.point(300))
      {
        digitalWrite(buzzer,HIGH);
        display_s = 1;
        lcd.clear();
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
        arrow_position--;
        if (arrow_position<0)
        {
          arrow_position=3;
        }
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
        arrow_position++;
        if (arrow_position>3)
        {
          arrow_position=0;
        }
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
        display_s = 0;
        lcd.clear();
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
  if (display_s==1)
  {
    create(0,"      MENU      ",4,"El1","El2","El3","El4");
    set_arrow(4,arrow_position);
  }
//--------------------------------------------------/
}
//==================================================/
//
//
//Функция принимает количество элементов на экране и позицию стрелки
//Формирует на экране положение стрелки
//позицию стрелки задают клавиши left right
void set_arrow(char element_num, char arrow_position)
{
  switch (element_num)
  { 
    case 1: 
    break;
    case 2:
      lcd.setCursor(5,1);
      if (arrow_position==0)
      {
        lcd.print('<');
      }
      else if (arrow_position==1)
      {
        lcd.print(' ');
      }
      lcd.setCursor(13,1);
      if (arrow_position==0)
      {
        lcd.print(' ');
      }
      else if (arrow_position==1)
      {
        lcd.print('<');
      }
    break;
    case 3:
      lcd.setCursor(4,1);
      if (arrow_position==0)
      {
        lcd.print('<');
      }
      else if (arrow_position==1)
      {
        lcd.print(' ');
      }
      else if (arrow_position==2)
      {
        lcd.print(' ');
      }
      lcd.setCursor(9,1);
      if (arrow_position==0)
      {
        lcd.print(' ');
      }
      else if (arrow_position==1)
      {
        lcd.print('<');
      }
      else if (arrow_position==2)
      {
        lcd.print(' ');
      }
      lcd.setCursor(14,1);
      if (arrow_position==0)
      {
        lcd.print(' ');
      }
      else if (arrow_position==1)
      {
        lcd.print(' ');
      }
      else if (arrow_position==2)
      {
        lcd.print('<');
      }
    break;
    case 4:
    lcd.setCursor(3,1);
      if (arrow_position==0)
      {
        lcd.print('<');
      }
      else if (arrow_position==1)
      {
        lcd.print(' ');
      }
      else if (arrow_position==2)
      {
        lcd.print(' ');
      }
      else if (arrow_position==3)
      {
        lcd.print(' ');
      }
      lcd.setCursor(7,1);
      if (arrow_position==0)
      {
        lcd.print(' ');
      }
      else if (arrow_position==1)
      {
        lcd.print('<');
      }
      else if (arrow_position==2)
      {
        lcd.print(' ');
      }
      else if (arrow_position==3)
      {
        lcd.print(' ');
      }
      lcd.setCursor(11,1);
      if (arrow_position==0)
      {
        lcd.print(' ');
      }
      else if (arrow_position==1)
      {
        lcd.print(' ');
      }
      else if (arrow_position==2)
      {
        lcd.print('<');
      }
      else if (arrow_position==3)
      {
        lcd.print(' ');
      }
      lcd.setCursor(15,1);
      if (arrow_position==0)
      {
        lcd.print(' ');
      }
      else if (arrow_position==1)
      {
        lcd.print(' ');
      }
      else if (arrow_position==2)
      {
        lcd.print(' ');
      }
      else if (arrow_position==3)
      {
        lcd.print('<');
      }
    break;
  }
}
void create(char global_num,const char* str1, char element_num,const char* str11,const char* str12,const char* str13,const char* str14)
{
  switch (element_num)
  {
    case 1:
    lcd.setCursor(0,0);
    lcd.print(str1);
    lcd.setCursor(6,1);
    lcd.print(str11);
    break;
    case 2:
    lcd.setCursor(0,0);
    lcd.print(str1);
    lcd.setCursor(2,1);
    lcd.print(str11);
    lcd.setCursor(10,1);
    lcd.print(str12);
    break;
    case 3:
    lcd.setCursor(0,0);
    lcd.print(str1);
    lcd.setCursor(1,1);
    lcd.print(str11);
    lcd.setCursor(6,1);
    lcd.print(str12);
    lcd.setCursor(11,1);
    lcd.print(str13);
    break;
    case 4:
    lcd.setCursor(0,0);
    lcd.print(str1);
    lcd.setCursor(0,1);
    lcd.print(str11);
    lcd.setCursor(4,1);
    lcd.print(str12);
    lcd.setCursor(8,1);
    lcd.print(str13);
    lcd.setCursor(12,1);
    lcd.print(str14);
    break;
  }
}











