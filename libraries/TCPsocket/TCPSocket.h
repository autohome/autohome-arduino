#ifndef TCPSocket_h
#define TCPSocket_h

#include "Arduino.h"
#include <Ethernet.h>
#include <SPI.h>


/*********************************************************
 * BEGIN Port Definitions                                *
 *********************************************************/
#ifndef LOCAL_SERVER_PORT
#define LOCAL_SERVER_PORT 8080
#endif

#ifndef REMOTE_SERVER_PORT
#define REMOTE_SERVER_PORT 80
#endif
/*********************************************************
 *   END Port Definitions                                *
 *********************************************************/

/*********************************************************
 * BEGIN Authorization Key Definitions                   *
 *********************************************************/

#ifndef KEY_LENGTH
#define KEY_LENGTH 64		// both initialization and one-time key
#endif

#ifndef STATUS_LENGTH
#define STATUS_LENGTH 1
#endif

#ifndef KEY_INIT
#define KEY_INIT "&I=" // "&initialization_key="
#endif

#ifndef KEY_ONETIME
#define KEY_ONETIME "&O=" // "&one_time_key="
#endif

#ifndef KEY_MAC
#define KEY_MAC "M=" // "mac_address="
#endif

#ifndef PARM_STATUS
#define PARM_STATUS "status:"
#endif

#ifndef PARM_NEW_ONE_TIME_KEY
#define PARM_NEW_ONE_TIME_KEY "newonetime:"
#endif

#ifndef PARM_NEW_INIT_KEY
#define PARM_NEW_INIT_KEY "newinit:"
#endif

#ifndef POST_DATA_KEY_POSITION
#define POST_DATA_KEY_POSITION 2+5*5+2*2+1 //32
#endif

#ifndef POST_DATA_LENGTH
#define POST_DATA_LENGTH 97 //POST_DATA_KEY_POSITION + KEY_LENGTH + 1 // 96 123
#endif


/*********************************************************
 *   END MAC Address Byte Definitions                    *
 *********************************************************/

/*********************************************************
 * BEGIN IP Address Definitions                          *
 *********************************************************/
#ifndef IP0
#define IP0 192
#endif

#ifndef IP1
#define IP1 168
#endif

#ifndef IP2
#define IP2 18
#endif

#ifndef IP3
#define IP3 213 //131 is me, 114 is brian's dev environment
#endif
// 
extern byte ip[ ];// = { 
//   IP0, IP1, IP2, IP3 };


/*********************************************************
 *   END IP Address Definitions                          *
 *********************************************************/

const char CR = 13;
const char LF = 10;

#ifndef CLIENT_BUFFER_LENGTH
#define CLIENT_BUFFER_LENGTH 200
#endif


// TCPSocket.h: declares parameters associated with client communiction



class TCPSocket
{	
	public:
		TCPSocket( char * initKey, byte macReceived[ ] );
		int receiveClientMessage( char* readString );
		int sendClientMessage( char * uri, char * data );
		int setOneTimeKey( char * key );
		
	private:
		byte * mac;
		EthernetServer server;
		char * oneTimeKey;
	  EthernetClient client;
		//int readStringIndex;
		//unsigned char crlfCount;
		void setupServer( );
		void initEthernet( );
		void readFromClient( char* readString, bool *lastCharWasCR, bool *lastCharWasLF, int *readStringIndex, unsigned char *crlfCount );
		void setupClient( boolean secondTry, char * initKey );
		//char * initializeOneTimeKey( );
		int processAuthenticationMessage( char * readString );
		int freeRam ();

};

//extern TCPSocket TCPSocket;

#endif