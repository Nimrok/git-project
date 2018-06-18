/*
* ----------------------------------------------------------------------------
* “THE COFFEEWARE LICENSE” (Revision 1):
* <ihsan@kehribar.me> wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a coffee in return.
* -----------------------------------------------------------------------------
* This library is based on this library: 
*   https://github.com/aaronds/arduino-nrf24l01
* Which is based on this library: 
*   http://www.tinkerer.eu/AVRLib/nRF24L01
* -----------------------------------------------------------------------------
*/
#ifndef NRF24
#define NRF24

#include "nRF24L01.h"
#include <stdint.h>

#define LOW 0
#define HIGH 1

#define nrf24_ADDR_LEN 5
#define nrf24_CONFIG ((1<<EN_CRC)|(0<<CRCO))

#define NRF24_TRANSMISSON_OK 0
#define NRF24_MESSAGE_LOST   1

/* adjustment functions */
void    nrf24_init();
void    nrf24_rx_address(char* adr);
void    nrf24_tx_address(char* adr);
void    nrf24_config(char channel, char pay_length);

/* state check functions */
char nrf24_dataReady();
char nrf24_isSending();
char nrf24_getStatus();
char nrf24_rxFifoEmpty();

/* core TX / RX functions */
void    nrf24_send(char* value);
void    nrf24_getData(char* data);

/* use in dynamic length mode */
char nrf24_payloadLength();

/* post transmission analysis */
char nrf24_lastMessageStatus();
char nrf24_retransmissionCount();

/* Returns the payload length */
char nrf24_payload_length();

/* power management */
void    nrf24_powerUpRx();
void    nrf24_powerUpTx();
void    nrf24_powerDown();

/* low level interface ... */
char spi_transfer(char tx);
void    nrf24_transmitSync(char* dataout,char len);
void    nrf24_transferSync(char* dataout,char* datain,char len);
void    nrf24_configRegister(char reg, char value);
void    nrf24_readRegister(char reg, char* value, char len);
void    nrf24_writeRegister(char reg, char* value, char len);

/* -------------------------------------------------------------------------- 
  You should implement the platform spesific functions in your code 
 * -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- 
 * In this function you should do the following things:
 *    - Set MISO pin input
 *    - Set MOSI pin output
 *    - Set SCK pin output
 *    - Set CSN pin output
 *    - Set CE pin output     
 * -------------------------------------------------------------------------- */
extern void nrf24_setupPins();

/* -------------------------------------------------------------------------- 
 * nrf24 CE pin control function
 *    - state:1 => Pin HIGH
 *    - state:0 => Pin LOW     
 * -------------------------------------------------------------------------- */
extern void nrf24_ce_digitalWrite(char state);

/* -------------------------------------------------------------------------- 
 * nrf24 CE pin control function
 *    - state:1 => Pin HIGH
 *    - state:0 => Pin LOW     
 * -------------------------------------------------------------------------- */
extern void nrf24_csn_digitalWrite(char state);

/* -------------------------------------------------------------------------- 
 * nrf24 SCK pin control function
 *    - state:1 => Pin HIGH
 *    - state:0 => Pin LOW     
 * -------------------------------------------------------------------------- */
extern void nrf24_sck_digitalWrite(char state);

/* -------------------------------------------------------------------------- 
 * nrf24 MOSI pin control function
 *    - state:1 => Pin HIGH
 *    - state:0 => Pin LOW     
 * -------------------------------------------------------------------------- */
extern void nrf24_mosi_digitalWrite(char state);

/* -------------------------------------------------------------------------- 
 * nrf24 MISO pin read function
 * - returns: Non-zero if the pin is high 
 * -------------------------------------------------------------------------- */
extern char nrf24_miso_digitalRead();

#endif
