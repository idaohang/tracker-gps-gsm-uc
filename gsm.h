/****************************************************************************

 File			:	gsm.h
 Version		:	0.01 (2011.04.11)
 
 Author			:	Bartosz Chmura
 Copyright		:	(c) 2011 by Bartosz Chmura
 License		:	GNU GPL v3

-----------------------------------------------------------------------------
 
 Description	:	biblioteka do obsługi GSM
					obsługiwane ukłądy:
						* SIM900(x); SIMCOM
 
-----------------------------------------------------------------------------
 
 Changelog		:	0.01 - initial version
 
-----------------------------------------------------------------------------
 
 ToDo			:	* nothing
					
 
****************************************************************************/

#ifndef GSM_H_INCLUDED
#define GSM_H_INCLUDED

//#include "at.h"


// znak CTRL+Z - Substitute (SUB)
#define CTRL_Z				"\x1a"

#define GSM_BUFF_SIZE 		60
#define GSM_CMD_BUFF_SIZE 	80




/* inicjacja modułu GSM, konfiguracja I/O
 */
void gsmInit(void);


/* czekaj na połączenie z siecią GSM
 * 		nawiązuje komunikację po UART (autobounding)
 * 		wyłącza echo w GSM
 * 		czeka na połączenie z BTS
 */ 
void gsmConnectToBts(void);



/* wyślij komendę AT, czekaj na odpowiedź i porównaj ją z tą oczekiwaną 
 * 
 * użycie:
 *		gsmSendAtCmdWaitResp("AT+CPIN?", "READY", 1, 3);
 *
 * dane wejściowe:
 * 		AT_cmd_string		- komenda do wysłania
 * 		response_string		- oczekiwana odpowiedź, może być fragment
 * 		no_of_attempts		- ilość prób, >=1
 * 		wait_delay			- ile ms które mamy czekać na odpowiedź; delay = 100ms * wait_delay,  >=1

 */
uint8_t gsmSendAtCmdWaitResp(uint8_t const *, uint8_t const *, uint8_t, uint8_t);

/*
 */
void gsmSendAtCmdNoResp(uint8_t const *, uint8_t);


/* czekaj na znak zachęty (command prompt)
 * jeżeli on wystąpi możemy zacząć nadawać treść SMS'a lub poprzez GPRS
 */
inline void gsmWaitForCmdPrompt(void);

/*
 */
uint8_t gsmGprsInit(uint8_t const *);

/*
 */
uint8_t gsmGprsOpenSocket(uint8_t const *, uint8_t const *, uint8_t const *);

/*
 */
uint8_t gsmGprsSendData(uint8_t const *);


// włączenie/wyłącznie wyjść z uC do modułu GSM
inline void gsmRtsOn(void);
inline void gsmRtsOff(void);
inline void gsmDtrOn(void);
inline void gsmDtrOff(void);
inline void gsmPwrkeyOn(void);
inline void gsmPwrkeyOff(void);
inline void gsmResetOn(void);
inline void gsmResetOff(void);



/* zatrzymuje odbieranie danych z GSM
 * GSM dalej działa i może wysyłać dane, ale przerwanie które je odbiera jest wyłączone
 */
inline void gsmRxcieDisable(void);

/* wznawia odbieranie danych z GSM po użyciu gsmRxcieDisable()
 */
inline void gsmRxcieEnable(void);




/* pole bitowe
 * flagi informujące o stanie GSM
 */
struct GSM_FLAGS {
		
	// status: włączony(1) / wyłączony (0)
	//uint8_t status : 1;
	//uint8_t gprsRegistered : 1;
	//uint8_t gprsAtReady : 1;
	
	
};


// definicja struktury na flagi do obsługi GPS
volatile struct GSM_FLAGS gsmFlags;



/* do przechowywania wyniku z gsmSendAtCmdWaitResp()
 * otrzymuje dane z 'enum AT_RESP_ENUM '
 */ 
uint8_t gsmResponse;


// bufor na dane odebrane z GSM
volatile uint8_t gsmBuff[GSM_BUFF_SIZE];
// index dla bufora
volatile uint8_t gsmBuffIndex;

// bufor do 'składania' komend AT do wysłania 
uint8_t gsmCmdBuff[GSM_CMD_BUFF_SIZE];
// index
volatile uint8_t gsmCmdBuffIndex;



enum AT_RESP_ENUM 
{
  AT_RESP_ERR_NO_RESP = -1,   // nic nie odebrano
  AT_RESP_ERR_DIF_RESP = 0,   // odebrana odpowiedź jest inna niż oczekiwana
  AT_RESP_OK = 1,             // odebrana odpowiedź jest taka jak oczekiwana

  AT_RESP_LAST_ITEM
};







#endif // GSM_H_INCLUDED
