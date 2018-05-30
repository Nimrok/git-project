/*
* ----------------------------------------------------------------------------
* “THE COFFEEWARE LICENSE” (Revision 1):
* <ihsan@kehribar.me> wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a coffee in return.
* -----------------------------------------------------------------------------
* Please define your platform spesific functions in this file ...
* -----------------------------------------------------------------------------
*/

#include <avr/io.h>

/* ------------------------------------------------------------------------- */
void nrf24_setupPins()
{
	DDRB |= (1<<1);  // CE output
	DDRB |= (1<<2);  // CSN output
	DDRB |= (1<<3);  // MOSI output
	DDRB |= (1<<5);  // SCK output
	DDRB &= ~(1<<4); // MISO input
}
/* ------------------------------------------------------------------------- */
void nrf24_ce_digitalWrite(char state)
{
    if(state)
    {
		PORTB |= (1<<1);
    }
    else
    {
		PORTB &= ~(1<<1);
    }
}
/* ------------------------------------------------------------------------- */
void nrf24_csn_digitalWrite(char state)
{
    if(state)
    {
		PORTB |= (1<<2);
    }
    else
    {
		PORTB &= ~(1<<2);
    }
}
/* ------------------------------------------------------------------------- */
void nrf24_sck_digitalWrite(char state)
{
    if(state)
    {
		PORTB |= (1<<5);
    }
    else
    {
		PORTB &= ~(1<<5);
    }
}
/* ------------------------------------------------------------------------- */
void nrf24_mosi_digitalWrite(char state)
{
    if(state)
    {
		PORTB |= (1<<3);
    }
    else
    {
		PORTB &= ~(1<<3);
    }
}
/* ------------------------------------------------------------------------- */
char nrf24_miso_digitalRead()
{
	return PINB & (1<<4);
}
/* ------------------------------------------------------------------------- */
