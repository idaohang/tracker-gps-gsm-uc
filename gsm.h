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
 
 Changelog		:	0.1 - initial version
 
-----------------------------------------------------------------------------
 
 ToDo			:	* nothing
					
 
****************************************************************************/

#ifndef __GSM_H_INCLUDED
#define __GSM_H_INCLUDED

#define GSM_LIB_VERSION 010	// library version X.YY (e.g. 1.00)




// znak CTRL+Z - Substitute (SUB)
#define CTRL_Z				"\x1a"

#define GSM_RX_BUFF_SIZE 	60
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
 * 		no_of_attempts		- ilość prób; >=1
 * 		wait_max_delay		- ile maksymalnie ms mamy czekać na odpowiedź; delay = 100ms * wait_max_delay;  >=1
 */
enum AT_RESP_ENUM gsmSendAtCmdWaitResp(uint8_t const *, uint8_t const *, uint8_t, uint8_t);

/*
 */
void gsmSendAtCmdNoResp(uint8_t const *, uint8_t);


/* czekaj na znak zachęty (command prompt)
 * jeżeli on wystąpi możemy zacząć nadawać treść SMS'a lub poprzez GPRS
 */
inline void gsmWaitForCmdPrompt(void);




enum RX_STATE_ENUM gsmIsRxFinished(void);



/*
 */
enum GPRS_INIT_ENUM gsmGprsInit(uint8_t const *);

/*
 */
enum GPRS_SOCKET_ENUM gsmGprsOpenSocket(uint8_t const *, uint8_t const *, uint8_t const *);

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




//////////////////////////////////////////////////////////////////
// enums


enum AT_RESP_ENUM 
{
	AT_RESP_ERR_NO_RESP = -1,		// nic nie odebrano
	AT_RESP_ERR_DIF_RESP = 0,		// odebrana odpowiedź jest inna niż oczekiwana
	AT_RESP_OK = 1,					// odebrana odpowiedź jest taka jak oczekiwana
	AT_RESP_LAST_ITEM
};

enum RX_STATE_ENUM 
{
	RX_NOT_FINISHED = 0,			// nie zakończono odbioru
	RX_FINISHED = 1,				// zakończono odbiór, coś odebrano
	RX_LAST_ITEM
};

enum GPRS_INIT_ENUM 
{
	GPRS_INIT_NOT_OK = 0,			// nie połączono
	GPRS_INIT_OK = 1,				// połączono z siecią GPRS
	GPRS_INIT_LAST_ITEM
};

enum GPRS_SOCKET_ENUM 
{
	GPRS_SOCKET_NOT_OPEN = 0,		// nie otwarty
	GPRS_SOCKET_OPEN = 1,			// otwarty pomyślnie
	GPRS_SOCKET_LAST_ITEM
};




//////////////////////////////////////////////////////////////////
// struktury etc.



struct GSM_DATA {
	
	uint8_t ipAddress[16+1];			// otrzymany adres IP
	enum AT_RESP_ENUM response;			// do przechowywania wyniku z gsmSendAtCmdWaitResp
};


/* pole bitowe
 * flagi informujące o stanie GSM
 */
struct GSM_FLAGS {
		
	// status: włączony(1) / wyłączony (0)
	//uint8_t status : 1;
	//uint8_t gprsRegistered : 1;
	//uint8_t gprsAtReady : 1;
	
	
};


// definicja struktury na dane dla GSM
volatile struct GSM_DATA gsm;


// definicja struktury na flagi do obsługi GSM
volatile struct GSM_FLAGS gsmFlags;




//////////////////////////////////////////////////////////////////
// bufory



// bufor na dane odebrane z GSM
volatile uint8_t gsmRxBuff[GSM_RX_BUFF_SIZE];
// index dla bufora odiorczego
volatile uint8_t gsmRxBuffIdx;


// bufor do 'składania' komend AT do wysłania 
uint8_t gsmCmdBuff[GSM_CMD_BUFF_SIZE];
// index dla bufora na komendy
uint8_t gsmCmdBuffIdx;







#endif // __GSM_H_INCLUDED
