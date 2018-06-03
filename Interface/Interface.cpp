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
