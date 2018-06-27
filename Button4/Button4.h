#ifndef BUTTON4
#define BUTTON4

#include "Arduino.h"

class Button4 // им¤ класса
	{
	private: // спецификатор доступа private
	    unsigned long count_point;
		char first_use;
	public: // спецификатор доступа public
	    Button4(); // конструктор класса
	    char point(unsigned int timer);
	}; // конец объ¤влени¤ класса Button4
	
#endif