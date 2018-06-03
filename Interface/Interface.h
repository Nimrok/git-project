#ifndef INTERFACE
#define INTERFACE

#include "Arduino.h"

class Interface // им€ класса
	{
	private: // спецификатор доступа private
	    unsigned long count_point;
		char first_use;
	public: // спецификатор доступа public
	    Interface(); // конструктор класса
	    char point(unsigned int timer);
	}; // конец объ€влени€ класса Pointer
	
#endif