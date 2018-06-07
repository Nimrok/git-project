
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
Pointer constr_delay;
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
//                0 1 2 3   4 5 6 7  8 9.0.1 .2.3.4.5 .6.7.8.9
char schema[20]={2,2,2,2  ,2,2,2,2 ,2,2,2,2 ,2,2,2,2 ,2,2,2,2};
char arrow_position = 0;
char arrow_floor = 0;
char opened_menu = 0;
char pre_arrow = 0;
char shift;
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
  menu(1,4);
  menu(2,1);
  menu(2,2);
  menu(2,3);
  menu(2,4);
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
  check_button(100);
//--------------------------------------------------/
  if ((millis()-buzz_start > 100)&&(buzz_flag == 1))
  {
    buzz_flag = 0;
    digitalWrite(buzzer, LOW);
  }
//--------------------------------------------------/
  if (constr_delay.point(50))
  {
    construct_display();
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
void menu(char level, char element_num)
{
  static char element = 0;
  if (level==1)
  {
    for(char i =0;i<element_num;i++)
    {
      schema[i]=0;
    }
  }
  else if (level==2)
  {
    for(char i =0;i<element_num;i++)
    {
      if (element==0)
      {
        schema[i+4]=0;
      }
      else if (element==1)
      {
        schema[i+8]=0;
      }
      else if (element==2)
      {
        schema[i+12]=0;
      }
      else if (element==3)
      {
        schema[i+16]=0;
      }
    }
    element++;
  }
}  
void check_button(int interval)
{
  if (button.point(interval))
  {
    value=analogRead(1);
    if ((value > 800) && (value < 830))
    {
      //НЕ НАЖАТА НИ ОДНА КНОПКА
    }
    else if ((value > 750) && (value < 790))
    { //кнопка select
      if (btn_delay.point(300))
      {
        digitalWrite(buzzer,HIGH);
        lcd.clear();
        buzz_flag = 1;
        buzz_start=millis();
        if (arrow_floor==2)
        {
          schema[arrow_position+shift]=1;
          arrow_floor=3;
          arrow_position=0;
        }
        if (arrow_floor==1)
        {
          pre_arrow=arrow_position;
          schema[arrow_position]=1;
          shift=4+arrow_position*4;
          arrow_floor=2;
          arrow_position=0;
        }
        if (arrow_floor==0)
        {
          arrow_floor=1;
          opened_menu=1;
        }
      }
    }
    else if ((value > 660) && (value < 700))
    { //кнопка left
      if (btn_delay.point(300))
      {
        digitalWrite(buzzer,HIGH);
        buzz_flag = 1;
        buzz_start=millis();
        if (opened_menu==1)
        {
          arrow_position--;
          if (arrow_position<0)
          {
            arrow_position=3;
          }
          if (arrow_floor==1)
          {
            while(schema[arrow_position]==2)
            {
              arrow_position--;
            }         
          }
          else if (arrow_floor==2)
          {
            while(schema[arrow_position+shift]==2)
            {
              arrow_position--;
            }          
          }
        }
      }
    }
    else if ((value > 500) && (value < 530))
    { //кнопка right
      if (btn_delay.point(300))
      {
        digitalWrite(buzzer,HIGH);
        buzz_flag = 1;
        buzz_start=millis();
        if (opened_menu==1)
        {
          arrow_position++;
          if (arrow_position>3)
          {
            arrow_position=0;
          }
          if (arrow_floor==1)
          {
            if (schema[arrow_position]==2)
            {
              arrow_position=0;
            }
          }
          else if (arrow_floor==2)
          {
            if (schema[arrow_position+shift]==2)
            {
              arrow_position=0;
            }
          }
        }
      }
    }
    else if (value < 30)
    { //кнопка cancel
      if (btn_delay.point(300))
      {
        digitalWrite(buzzer,HIGH);
        buzz_flag = 1;
        arrow_floor--;
        if (arrow_floor<0)
        {
          arrow_floor=0;
        }
        if (arrow_floor==1)
        {
          arrow_position=pre_arrow;
        }
        if (arrow_floor==0)
        {
          pre_arrow=0;
          arrow_position=0;
          opened_menu = 0;
        }
        lcd.clear();
        buzz_start=millis();
      }
    }
  }
}
void construct_display()
{
  if (arrow_floor==0)
  {
    create(0,"___Work field___",1,"data","","","");
  }
  else if (arrow_floor==1)
  {
    create(0,"_____Menu1______",4,"El1","El2","El3","El4");
    set_arrow(4,arrow_position);
  }
  else if (arrow_floor==2)
  {
    switch (pre_arrow)
    {
      case 0:
      create(0,"_______EL1______",1,"E11","","","");        //менять количество элементов нужно в сетэрроу, в криейте, в меню
      set_arrow(1,arrow_position);                          
      break;
      case 1:
      create(0,"_______EL2______",2,"E21","E22","","");
      set_arrow(2,arrow_position);
      break;
      case 2:
      create(0,"_______EL3______",3,"E31","E32","E33","");
      set_arrow(3,arrow_position);
      break;
      case 3:
      create(0,"_______EL4______",4,"E41","E42","E43","E44");
      set_arrow(4,arrow_position);
      break;
    }
  }
}




