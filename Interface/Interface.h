#ifndef INTERFACE
#define INTERFACE

#include "Arduino.h"

class Interface // ��� ������
	{
	private: // ������������ ������� private
	    unsigned long count_point;
		char first_use;
	public: // ������������ ������� public
	    Interface(); // ����������� ������
	    char point(unsigned int timer);
	}; // ����� ���������� ������ Pointer
	
#endif