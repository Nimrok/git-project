#ifndef INTERFACE
#define INTERFACE

#include "Arduino.h"
#include "Pointer.h"

class Interface
	{
	private:
		char schema[20];
		char arrow_position;
		char arrow_floor;
		char opened_menu;
		char pre_arrow;
		char shift;
	public: 
	    Interface()
	    void construct_display()
		void check_button(int interval)
		void menu(char level, char element_num)
		void create(char global_num,const char* str1, char element_num,const char* str11,const char* str12,const char* str13,const char* str14)
		void set_arrow(char element_num, char arrow_position)
	};
	
#endif