#ifndef POINTER
#define POINTER

#include "Arduino.h"

class Pointer // им€ класса
	{
	private: // спецификатор доступа private
	    unsigned long count_point;
		char first_use;
	public: // спецификатор доступа public
	    Pointer(); // конструктор класса
	    char point(unsigned int timer);
	}; // конец объ€влени€ класса Pointer
	
#endif