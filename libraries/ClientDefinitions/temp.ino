/*********************************************************
 * BEGIN MAC Address Byte Definitions                    *
 *********************************************************/
#ifndef MAC

#define MAC

#define MAC0 0x00

#define MAC1 0x11

#define MAC2 0x22

#define MAC3 0x33

#define MAC4 0x44

#define MAC5 0x56

byte mac[] = { 
  MAC0, MAC1, MAC2, MAC3, MAC4, MAC5 };

#endif

/*********************************************************
 *   END MAC Address Byte Definitions                    *
 *********************************************************/

/*********************************************************
 * BEGIN IP Address Definitions                          *
 *********************************************************/
#ifndef IP

#define IP

#define IP0 192

#define IP1 168

#define IP2 18

#define IP3 131 //131 is me, 114 is brian's dev environment

byte ip[] = { 
  IP0, IP1, IP2, IP3 };

#endif
/*********************************************************
 *   END IP Address Definitions                          *
 *********************************************************/

/*********************************************************
 * BEGIN Server/Port Definitions                         *
 *********************************************************/
#ifndef LOCAL_SERVER_PORT
#define LOCAL_SERVER_PORT 8080
#endif

#ifndef REMOTE_SERVER_PORT
#define REMOTE_SERVER_PORT 3000
#endif

/*********************************************************
 *   END Server/Port Definitions                         *
 *********************************************************/

/*********************************************************
 * BEGIN Client Message Definitions                      *
 *********************************************************/

#ifndef CLIENT_BUFFER_LENGTH
#define CLIENT_BUFFER_LENGTH 200
#endif

#ifndef READSTRING
#define READSTRING
char readString[CLIENT_BUFFER_LENGTH];
#endif

char *readString_pointer = &readString[0];

const char CR = 13;
const char LF = 10;

int readStringIndex;
int authTryCount;
int crlfCount;

char postDataInit[123];
char* postDataAuth;

boolean lastCharWasCR = false;
boolean lastCharWasLF = false;

/*********************************************************
 * END Client Message Definitions                        *
 *********************************************************/