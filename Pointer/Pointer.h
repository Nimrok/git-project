#ifndef POINTER
#define POINTER

#include "Arduino.h"

class Pointer // ��� ������
	{
	private: // ������������ ������� private
	    unsigned long count_point;
		char first_use;
	public: // ������������ ������� public
	    Pointer(); // ����������� ������
	    char point(unsigned int timer);
	}; // ����� ���������� ������ Pointer
	
#endif