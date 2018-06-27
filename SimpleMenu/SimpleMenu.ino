/*
 * 30.05.2018 Для блока радиоуправляемого реле
 */
//--------------------------------------------------/                   ИНКЛУДЫ
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <RTC.h>
#include "nrf24.c"
#include "nrf24.h"
#include "nRF24L01.h"
#include "radioPinFunctions.c"
#include <Pointer.h>
//--------------------------------------------------/                   ДЕФАЙНЫ
#define light_sensor 8
#define one_wire    7
#define buzzer      6
#define relay       5
#define addMenu(name, next, prev, field) extern submenu next;extern submenu prev;submenu name = {(void*)&next, (void*)&prev, field}
#define button_delay 80
//--------------------------------------------------/                   КЛАССОВ ИНИЦИАЛИЗАЦИЯ
Pointer ask_keyboard;
Pointer button_pushed_time;
Pointer ask_light_sensor;
Pointer ds_start_conv;
Pointer ds_get_data;
Pointer draw_menu;
RTC time;
LiquidCrystal_I2C lcd(0x3F,16,2);
OneWire ds(one_wire);
//--------------------------------------------------/                   ПЕРЕМЕННЫЕ
char button_reset = 1;
char menu_scroll_buffer[16]="prev <    > next";
char menu_opened = 0;
struct submenu
{
  submenu *next;
  submenu *prev;
  const char field[4];
};
submenu *currentMenu;
char nrf_data_buffer[4];
char nrf_destination_buffer[5] = {0xD7,0xD7,0xD7,0xD7,0xD7}; //куда отправлять
char nrf_id_buffer[5] = {0xE7,0xE7,0xE7,0xE7,0xE7}; //адрес этого устройства
int analog_button_value = 0;
char buzzer_started = 0;
unsigned long buzzer_start_point = 0;
float ds_temp = 0.0;
byte ds_low_byte = 0;
byte ds_high_byte = 0;
addMenu(el1,el2,el3,"PCFn");
addMenu(el2,el3,el1,"DS18");
addMenu(el3,el1,el2,"FLSH");
//==================================================/                   ФУНКЦИИ
void moveMenu(submenu *NewMenu)
{
  currentMenu = NewMenu;
} 
//--------------------------------------------------/
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
//--------------------------------------------------/ 
void button_active_func()
{
  digitalWrite(buzzer,HIGH);
  buzzer_started = 1;
  button_reset = 0;
  buzzer_start_point=millis();
}
//--------------------------------------------------/
void buzzer_off()
{
  if ((millis()-buzzer_start_point > 100)&&(buzzer_started == 1))
  {
    buzzer_started = 0;
    digitalWrite(buzzer, LOW);
  }
}
//--------------------------------------------------/ 
void check_button(int interval)
{
//---Если пришло время опрашивать кнопки---//
  if (ask_keyboard.point(interval))
  {
    analog_button_value=analogRead(1);
//---ни одна кнопка не нажата---//
    if ((analog_button_value > 800) && (analog_button_value < 830))
    {
      if (button_pushed_time.point(button_delay/2))
      {
        analog_button_value=analogRead(1);
        if ((analog_button_value > 800) && (analog_button_value < 830))
        {
          button_reset=1;
        }
      }
    }
//---ни одна кнопка не нажата---//
//---нажатие кнопки SELECT---//
    else if ((analog_button_value > 750) && (analog_button_value < 790) && (button_reset==1))
    {
      if (button_pushed_time.point(button_delay))
      {
        analog_button_value=analogRead(1);
        if ((analog_button_value > 750) && (analog_button_value < 790))
        {
          if (menu_opened==0)
          {
            button_active_func();
            menu_opened=1;
          }
        }
      }
    }
//---нажатие кнопки SELECT---//
//---нажатие кнопки LEFT---//
    else if ((analog_button_value > 660) && (analog_button_value < 700) && (button_reset==1))
    {
      if (button_pushed_time.point(button_delay))
      {
        analog_button_value=analogRead(1);
        if ((analog_button_value > 660) && (analog_button_value < 700))
        {
          if (menu_opened==1)
          {
            button_active_func();
            moveMenu(currentMenu->prev);
          }
        }
      }
    }
//---нажатие кнопки LEFT---//
//---нажатие кнопки RIGHT---//
    else if ((analog_button_value > 500) && (analog_button_value < 530) && (button_reset==1))
    {
      if (button_pushed_time.point(button_delay))
      {
        analog_button_value=analogRead(1);
        if ((analog_button_value > 500) && (analog_button_value < 530))
        {
          if (menu_opened==1)
          {
            button_active_func();
            moveMenu(currentMenu->next);
          }
        }
      }
    }
//---нажатие кнопки RIGHT---//
//---нажатие кнопки CANCEL---//
    else if ((analog_button_value < 30) && (button_reset==1))
    {
      if (button_pushed_time.point(button_delay))
      {
        analog_button_value=analogRead(1);
        if (analog_button_value < 30)
        {
          if (menu_opened==1)
          {
            button_active_func();
            menu_opened=0;
            moveMenu(&el1);
          }
        }
      }
    }
//---нажатие кнопки CANCEL---//
  }
//---Если пришло время опрашивать кнопки---//
}
//==================================================/                     СЕТАП
void setup() 
{
  moveMenu(&el1);
  Serial.begin(9600);
  lcd.init();                     
  lcd.backlight();
  nrf24_init();     
  nrf24_config(2,4); //второй канал, длина пакета 4 байта
  nrf24_tx_address(nrf_destination_buffer);
  nrf24_rx_address(nrf_id_buffer);
  time.begin(RTC_DS3231);    //здесь инициализируется i2c шина (F_CPU/2)
  Wire.begin();
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  pinMode(light_sensor,INPUT);
  ds.reset();
  ds.write(0xCC);
  ds.write(0x4E);
  ds.write(0x28);
  ds.write(0x00);
  ds.write(0x7F); 
}
//==================================================/                   ЦИКЛ ЛУП
void loop() 
{
  if (menu_opened==0)
  {
    if(nrf24_dataReady())
    {
      nrf24_getData(nrf_data_buffer);        
    }
//--------------------------------------------------/
    if (ask_light_sensor.point(30))
    {
      if (digitalRead(light_sensor)==0)
      {
        lcd.setCursor(4,0);
        lcd.print("light");
        digitalWrite(relay, HIGH);
      }
      else
      {
        lcd.setCursor(4,0);
        lcd.print("dark ");
        digitalWrite(relay, LOW);
      }
    }
    if (ds_start_conv.point(500))
    {
      ds.reset();
      ds.write(0xCC);
      ds.write(0x44); 
    }
    if (ds_get_data.point(1500))
    {
      ds.reset();
      ds.write(0xCC);
      ds.write(0xBE); 
      ds_high_byte=ds.read();
      ds_low_byte=ds.read();
      ds_temp =  ((ds_low_byte << 8) | ds_high_byte) * 0.0625;
      lcd.clear();
      lcd.setCursor(4,1);
      lcd.print(ds_temp);
    }
  }
//--------------------------------------------------/
  else if (menu_opened==1)
  {
    if (draw_menu.point(50))
    {
      lcd.setCursor(0,0);
      lcd.print("MENU:device v1.1");
      lcd.setCursor(0,1);
      memcpy(menu_scroll_buffer+6,currentMenu->field,4);
      memcpy(menu_scroll_buffer,currentMenu->prev->field,4);
      memcpy(menu_scroll_buffer+12,currentMenu->next->field,4);  
      lcd.print(menu_scroll_buffer);
    }
  }
  check_button(button_delay/2);
//--------------------------------------------------/
  buzzer_off();
//--------------------------------------------------/
}//Скобка, закрывающая loop.
//==================================================/

