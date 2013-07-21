#ifndef ClientDefinitions_h
#define ClientDefinitions_h

#include "Arduino.h"
#include <Ethernet.h>
#include <SPI.h>

#ifndef ORIG_INIT
#define ORIG_INIT "09cff4df1aa9052c635b9c4dc53fa3c90029aa1e1e9c531849cbd231f52bcb9d"
#endif


/*********************************************************
 * BEGIN Port Definitions                                *
 *********************************************************/
#ifndef LOCAL_SERVER_PORT
#define LOCAL_SERVER_PORT 8080
#endif

#ifndef REMOTE_SERVER_PORT
#define REMOTE_SERVER_PORT 3000
#endif
/*********************************************************
 *   END Port Definitions                                *
 *********************************************************/

#ifndef KEY_INIT
#define KEY_INIT "&initialization_key="
#endif

#ifndef KEY_LENGTH
#define KEY_LENGTH 64		// both initialization and one-time key
#endif

#ifndef KEY_MAC
#define KEY_MAC "mac_address="
#endif

/*********************************************************
 * BEGIN MAC Address Byte Definitions                    *
 *********************************************************/
#ifndef MAC0
#define MAC0 0x00
#endif

#ifndef MAC1
#define MAC1 0x11
#endif

#ifndef MAC2
#define MAC2 0x22
#endif

#ifndef MAC3
#define MAC3 0x33
#endif

#ifndef MAC4
#define MAC4 0x44
#endif

#ifndef MAC5
#define MAC5 0x56
#endif

extern byte mac[ ];// = { 
//   MAC0, MAC1, MAC2, MAC3, MAC4, MAC5 };


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
#define IP3 131 //131 is me, 114 is brian's dev environment
#endif
// 
extern byte ip[ ];// = { 
//   IP0, IP1, IP2, IP3 };


/*********************************************************
 *   END IP Address Definitions                          *
 *********************************************************/

const char CR = 13;
const char LF = 10;



// ClientDefinitions.h: declares parameters associated with client communiction



class ClientDefinitionsClass
{	
	public:
		ClientDefinitionsClass( );
		void setupServer( );
		int receiveClientMessage( char* readString );
	  EthernetClient client;
		
	private:
		EthernetServer server;
		int readStringIndex;
		int crlfCount;
		void initEthernet( );
		void readFromClient( char* readString, bool *lastCharWasCR, bool *lastCharWasLF );
		void setupClient( boolean secondTry, int *authTryCount );

};

//extern ClientDefinitionsClass ClientDefinitions;

#endif