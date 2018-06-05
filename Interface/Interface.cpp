#include "Interface.h"
#include "Arduino.h"

Interface::Interface() // конструктор класса
	{
		count_point = 0;
		first_use = 1;
	}
		 
char Interface::point(unsigned int timer) 
{
	if (first_use)
	{
		count_point = millis();
		first_use = 0;
	}
	if (millis() - count_point > timer)
	{
		count_point = millis();
		return 1;
	}
	else
	{
		return 0;
	}
}

menu1.create0(0,str1,3,str11,str12,str13);

menu11.create1(1,str1,2,str11,str12);
menu12.create1(2,str1,1,str1);
menu13.create1(3,str1,4,str11,str12,str13,str14);

menu111.create2(4,str1,1,str1);
menu112.create2(5,str1,1,str1);

menu121.create2(6,str1,1,str1);

menu131.create2(7,str1,1,str1);
menu132.create2(8,str1,1,str1);
menu133.create2(9,str1,1,str1);
menu134.create2(10,str1,1,str1);

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





