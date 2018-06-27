#include "Button4.h"
#include "Arduino.h"

Button4::Button4() // конструктор класса
	{

	}
		 
char Button4::point(unsigned int timer) 
{

}
Pointer button_push;
void button_read(int frequency, char analogPin)
{
	if (button_push.point(frequency))
	{
		button_value = analogRead(analogPin);
		
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
      }
    }
    else if ((value > 660) && (value < 700))
    { //кнопка left
      if (btn_delay.point(300))
      {
        digitalWrite(buzzer,HIGH);
        buzz_flag = 1;
        buzz_start=millis();
      }
    }
    else if ((value > 500) && (value < 530))
    { //кнопка right
      if (btn_delay.point(300))
      {
        digitalWrite(buzzer,HIGH);
        buzz_flag = 1;
        buzz_start=millis();
      }
    }
    else if (value < 30)
    { //кнопка cancel
      if (btn_delay.point(300))
      {
        digitalWrite(buzzer,HIGH);
        buzz_flag = 1;   
        lcd.clear();
        buzz_start=millis();
      }
    }
  }
}
