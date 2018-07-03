/* 
	Editor: https://www.visualmicro.com/
			visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
			the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
			all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
			note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Arduino/Genuino Uno, Platform=avr, Package=arduino
*/

#define __AVR_ATmega328p__
#define __AVR_ATmega328P__
#define ARDUINO 10805
#define ARDUINO_MAIN
#define F_CPU 16000000L
#define __AVR__
#define F_CPU 16000000L
#define ARDUINO 10805
#define ARDUINO_AVR_UNO
#define ARDUINO_ARCH_AVR
void time_set(char days, char months, char years, char hours, char minutes, char seconds, char day_number);
void none();
void relay_control();
void drawout(char x, char y, char* str);
void watch_set_display();
void moveMenu(submenu *NewMenu);
void write24(int deviceaddress, unsigned int eeaddress, byte data );
byte read24(int deviceaddress, unsigned int eeaddress );
void button_active_func();
void buzzer_off();
void check_button(int interval);
//
//

#include "pins_arduino.h" 
#include "arduino.h"
#include "SimpleMenu.ino"