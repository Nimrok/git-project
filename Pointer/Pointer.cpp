#include "Pointer.h"
#include "Arduino.h"

Pointer::Pointer() // конструктор класса
	{
		count_point = 0;
		first_use = 1;
	}
		 
char Pointer::point(unsigned int timer) 
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
