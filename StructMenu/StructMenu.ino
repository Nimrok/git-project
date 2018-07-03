/*
 * 30.05.2018 Для блока радиоуправляемого реле
 */
//--------------------------------------------------/                   ИНКЛУДЫ
#include <avr/wdt.h>
#include <LiquidCrystal_I2C.h>
#include <RTC.h>
#include <Pointer.h>
//--------------------------------------------------/                   ДЕФАЙНЫ
#define buzzer      6     //управление активной пищалкой
#define relay       5     //управление реле
#define addVar(name, next, prev, symbol, var) extern subvar next;extern subvar prev;subvar name = {(subvar*)&next, (subvar*)&prev, symbol, (void*)&var}
#define addMenu(name, next, prev, field, func) extern submenu next;extern submenu prev;submenu name = {(submenu*)&next, (submenu*)&prev, field, (void*)&func}
#define button_delay 80   //время повторного опроса кнопки, для избежания дребезга
//--------------------------------------------------/                   КЛАССОВ ИНИЦИАЛИЗАЦИЯ
Pointer ask_keyboard;         //точка отсчета опроса клавиатуры
Pointer button_pushed_time;   //т/о повторного опроса кнопки
Pointer draw_menu;            //т/о отрисовки экрана меню
Pointer draw_workspace;       //т/о отрисовки рабочего экрана
Pointer draw_element_menu;    //т/о отрисовки экрана элемента меню
RTC time;
LiquidCrystal_I2C lcd(0x3F,16,2);
//--------------------------------------------------/                   ПЕРЕМЕННЫЕ
struct subvar
{
  subvar *next;
  subvar *prev;
  const char symbol;
  void *var;
};
subvar *currentVar;
//--------------------------------------------------/
char button_released = 1;
char bottom_string_buffer[16] = "     <    >     ";
char menu_opened = 0;
struct submenu
{
  submenu *next;
  submenu *prev;
  const char field[4];
  void (*func)();
};
submenu *currentMenu;
//--------------------------------------------------/
char relay_state = 0;
int analog_button_value = 0;
char buzzer_started = 0;
unsigned long buzzer_start_point = 0;
char days=0;
char months=0;
char years=0;
char hours=0;
char minutes=0;
char seconds=0;
char day_number=0;
addVar(Y_var,M_var,d_var,'Y',years);
addVar(M_var,D_var,Y_var,'M',months);
addVar(D_var,h_var,M_var,'D',days);
addVar(h_var,m_var,D_var,'h',hours);
addVar(m_var,s_var,h_var,'m',minutes);
addVar(s_var,d_var,m_var,'s',seconds);
addVar(d_var,Y_var,s_var,'d',day_number);
addMenu(el1,el2,el3,"Wtch",watch_set_display);
addMenu(el2,el3,el1,"Rele",watch_set_display);
addMenu(el3,el1,el2,"FLSH",watch_set_display);
//==================================================/                   ФУНКЦИИ
void time_set(char days, char months, char years, char hours, char minutes, char seconds, char day_number)
{
  time.settime(days,months,years,hours,minutes,seconds,day_number);
}
//--------------------------------------------------/
void watch_set_display()
{
  lcd.setCursor(0,0);
  lcd.print("Real time set   ");
  lcd.setCursor(0,1);
  bottom_string_buffer[6] = currentVar->symbol;  
  bottom_string_buffer[0] = currentVar->prev->symbol;
  bottom_string_buffer[12] = currentVar->next->symbol;  
  lcd.print(bottom_string_buffer);
  char value = (char)&currentVar->var;
  lcd.setCursor(8,1);
  lcd.print(value,DEC);
}
//--------------------------------------------------/
void moveMenu(submenu *NewMenu)
{
  currentMenu = NewMenu;
} 
//--------------------------------------------------/
void moveVar(subvar *NewVar)
{
  currentVar = NewVar;
} 
//--------------------------------------------------/ 
void button_active_func()
{
  digitalWrite(buzzer,HIGH);
  buzzer_started = 1;
  button_released = 0;
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
          button_released=1;
        }
      }
    }
//---ни одна кнопка не нажата---//
//---нажатие кнопки SELECT---//
    else if ((analog_button_value > 750) && (analog_button_value < 790) && (button_released==1))
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
            lcd.clear();
          }
          else if (menu_opened==1)
          {
            button_active_func();
            menu_opened=2;
            lcd.clear();
            strcpy(bottom_string_buffer,"     <    >     ");
          }
        }
      }
    }
//---нажатие кнопки SELECT---//
//---нажатие кнопки LEFT---//
    else if ((analog_button_value > 660) && (analog_button_value < 700) && (button_released==1))
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
          else if (menu_opened==2)
          {
            button_active_func();
            moveVar(currentVar->prev);
          }
        }
      }
    }
//---нажатие кнопки LEFT---//
//---нажатие кнопки RIGHT---//
    else if ((analog_button_value > 500) && (analog_button_value < 530) && (button_released==1))
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
          else if (menu_opened==2)
          {
            button_active_func();
            moveVar(currentVar->next);
          }
        }
      }
    }
//---нажатие кнопки RIGHT---//
//---нажатие кнопки CANCEL---//
    else if ((analog_button_value < 30) && (button_released==1))
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
            lcd.clear();
          }
          else if (menu_opened==2)
          {
            button_active_func();
            menu_opened=1;
            lcd.clear();
          }
        }
      }
    }
//---нажатие кнопки CANCEL---//
  }
//---Если пришло время опрашивать кнопки---//
}
//==================================================/                   СЕТАП
void setup() 
{
  wdt_disable();
  moveMenu(&el1);
  moveVar(&Y_var);
  Serial.begin(9600);
  lcd.init();                     
  lcd.backlight();
  time.begin(RTC_DS3231);    //здесь инициализируется i2c шина (F_CPU/2)
  Wire.begin();
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  time.gettime();
  days=time.day;
  months=time.month;
  years=time.year;
  hours=time.Hours;
  minutes=time.minutes;
  seconds=time.seconds;
  day_number=time.weekday;
  wdt_enable(WDTO_2S);
}
//==================================================/                   ЦИКЛ ЛУП
void loop() 
{
  wdt_reset();
//--------------------------------------------------/
  if (menu_opened==0)
  {
    if (draw_workspace.point(50))
    {
      lcd.setCursor(0,0);
      lcd.print(time.gettime("d-m, H:i:s"));
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
      memcpy(bottom_string_buffer+6,currentMenu->field,4);
      memcpy(bottom_string_buffer,currentMenu->prev->field,4);
      memcpy(bottom_string_buffer+12,currentMenu->next->field,4);  
      lcd.print(bottom_string_buffer);
    }
  }
//--------------------------------------------------/
  else if (menu_opened==2)
  {
    if (draw_element_menu.point(50))
    {
      currentMenu->func();
    }
  }
//--------------------------------------------------/
  check_button(button_delay/2);
//--------------------------------------------------/
  buzzer_off();
//--------------------------------------------------/
}//Скобка, закрывающая loop.
//==================================================/

