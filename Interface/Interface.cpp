#include "Interface.h"
#include "Arduino.h"
#include "Pointer.h"

Interface::Interface() // конструктор класса
	{

	}
void Interface::construct_display()
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
void Interface::create(char global_num,const char* str1, char element_num,const char* str11,const char* str12,const char* str13,const char* str14)
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
void Interface::set_arrow(char element_num, char arrow_position)
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
void Interface::menu(char level, char element_num)
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
void Interface::check_button(int interval)
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
