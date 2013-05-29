#include <EEPROMx.h>
#include <SPI.h>
#include <Wiegand.h>
#include <Ethernet.h>

/*********************************************************
 * BEGIN I/O Pin Definitions                             *
 *********************************************************/
#ifndef PIN_DATA0
#define PIN_DATA0 2
#endif

#ifndef PIN_DATA1
#define PIN_DATA1 3
#endif

#ifndef PIN_LED
#define PIN_LED 4
#endif

#ifndef PIN_BUZZER
#define PIN_BUZZER 5
#endif
/*********************************************************
 *   END I/O Pin Definitions                             *
 *********************************************************/

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
#define IP3 114
#endif
/*********************************************************
 *   END IP Address Definitions                          *
 *********************************************************/

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

/*********************************************************
 * BEGIN Authorization Key Definitions                   *
 *********************************************************/
#ifndef ORIG_INIT
#define ORIG_INIT "c3119956a208973124b0a7e780998c33b5261800674bcbe95c978d1bf336eed6"
#endif

#define KEY_LENGTH 64
/*********************************************************
 *   END Authorization Key Definitions                   *
 *********************************************************/

#define KEY_INIT "&initialization_key="
#define KEY_ONETIME "&one_time_key="
#define KEY_MAC "mac_address="

const char CR = 13;
const char LF = 10;

WIEGAND wg;
EthernetServer server( LOCAL_SERVER_PORT );
EthernetClient client;

byte mac[] = { 
  MAC0, MAC1, MAC2, MAC3, MAC4, MAC5 };
byte ip[] = { 
  IP0, IP1, IP2, IP3 };
  
int authTryCount = 0;
char postDataInit[123];
char* postDataAuth;

void setUpPins( )
{
  // Configure pin I/O
  pinMode(PIN_DATA0, INPUT);
  pinMode(PIN_DATA1, INPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  // Configure digital outputs
  digitalWrite(PIN_LED, HIGH);
  digitalWrite(PIN_BUZZER, HIGH);
}

void setUpReader( )
{
  // Start reader
  wg.begin( );
}

void setUpEthernet( )
{
  Ethernet.begin( mac );
  Serial.println( Ethernet.localIP( ) );
}

void setUpServer( )
{
  server.begin( );
}

void setUpClient( boolean secondTry ) {
  Serial.println("Starting client...");
  // Build payload
  char* key = (char*)malloc(sizeof(char) * KEY_LENGTH);
  if ( secondTry == true ) {
    key = ORIG_INIT;
    authTryCount = 1;
  } else {
    Serial.print("Reading EEPROM...\"");
    EEPROMx.readFromEEPROM( key, KEY_LENGTH );
    Serial.print(key);
    Serial.println("\"");
  }
  
  sprintf(postDataInit, "%s%0.2x%%3A%0.2x%%3A%0.2x%%3A%0.2x%%3A%0.2x%%3A%0.2x%s%s", KEY_MAC, mac[0], mac[1],
    mac[2], mac[3], mac[4], mac[5], KEY_INIT, key);
  
  free(key);
  
  if (client.connect(ip, REMOTE_SERVER_PORT)) {
    client.println("POST /api/v1/online.txt HTTP/1.1");
    client.println("Content-Type: application/x-www-form-urlencoded; charset=UTF-8"); 
    client.println("Host: dev-environment:3000");
    client.println("Connection: close");
    client.print("Content-Length: ");
    client.println(strlen(postDataInit));
    client.println("");
    client.print(postDataInit);
  }
}

void readFromClient( )
{
}

void authorizeNode( )
{
  
}

void setup( )
{
  Serial.begin( 9600 );
  setUpPins( );
  setUpReader( );
  setUpEthernet( );
  setUpServer( );
  setUpClient( false );
  /*
  char* readIt = "1234567890123456789012345678901234567890123456789012345678901234";
   char* writeIt = (char*) malloc ( sizeof ( char ) * KEY_LENGTH );
   
   EEPROMx.writeToEEPROM( readIt );
   EEPROMx.readFromEEPROM( writeIt, KEY_LENGTH );
   
   
   Serial.println ( writeIt );
   //*/
}

void loop( )
{
  if ( client.available( ) )
  {
    readFromClient( );
  }
  
  if ( !client.connected( ) )
  {
    client.stop( );
    if ( readStringIndex > 0 )
    {
      authorizeNode( );
    }
  }
  if ( wg.available( ) )
  {
    Serial.println ( wg.getCode( ) );
  }
}

