#include "ClientDefinitions.h"
#include "Arduino.h"
#include <Ethernet.h>
#include <SPI.h>
#include <EEPROMx.h>

//EthernetServer server;

// ClientDefinitions Constructor class: initializes EthernetServer server via an "initialization list" and then
//																						the rest of the server/ethernet setup

byte mac[ ] = { 
  MAC0, MAC1, MAC2, MAC3, MAC4, MAC5 };

byte ip[ ] = { 
  IP0, IP1, IP2, IP3 };

ClientDefinitions::ClientDefinitions( char * initKey ) : 
server( LOCAL_SERVER_PORT )	// initialization list
{
	//char postData[ POST_DATA_LENGTH ];
	unsigned char authTryCount = 0;
  Serial.begin( 9600 );

	Serial.print( "Free ram: " );
	Serial.println( freeRam( ) );
	
	initEthernet( );
	server.begin( );
	setupServer( );
	setupClient( false, &authTryCount, initKey );
	//Serial.println( "Success." );
}

void ClientDefinitions::initEthernet( )
{
  Serial.println( "Initializing ethernet port..." );
  Ethernet.begin( mac );
	Serial.println( "Ethernet port initialized." );
}

void ClientDefinitions::setupServer( )
{
  Serial.print( "Starting server..." );
  server.begin( );
  Serial.print( "started at " );
  Serial.println( Ethernet.localIP( ) );
}

// setupClient(): initialize communication client between local node (arduino) and remote server

void ClientDefinitions::setupClient( boolean secondTry, unsigned char *authTryCount, char * initKey )
{
	char key[ KEY_LENGTH + 1 ];
	char postData[ POST_DATA_LENGTH + 1 ];
	
  Serial.println( "Starting client..." );
  // Build payload
  if ( secondTry == true )
	{
		// Use original initialization key if EEPROM key is invalid
    strncpy( key, initKey, KEY_LENGTH );
    *authTryCount = 1;
  }
	else
	{
		// first try the maintained EEPROM key
    Serial.print( "Reading EEPROM...\"" );
    EEPROMx.readFromEEPROM( key, KEY_LENGTH );
    Serial.print( key );
    Serial.println( "\"" );
  }

	Serial.print("Free ram: ");
	Serial.println(freeRam());
  
  sprintf(postData, "%s%0.2x%%3A%0.2x%%3A%0.2x%%3A%0.2x%%3A%0.2x%%3A%0.2x%s%s", KEY_MAC, mac[0], mac[1],
    mac[2], mac[3], mac[4], mac[5], KEY_INIT, key);
  
  if ( client.connect( ip, REMOTE_SERVER_PORT ) )
	{
    Serial.println( "Client connected." );
    Serial.print("postData: ");
		Serial.println( postData );
		Serial.print("postData length: ");
		Serial.println( strlen( postData ) );
    client.println( "POST /api/v1/online.txt HTTP/1.1" );
    client.println( "Content-Type: application/x-www-form-urlencoded; charset=UTF-8" ); 
    client.println( "Host: dev-environment:3000" );
    client.println( "Connection: close" );
    client.print( "Content-Length: " );
    client.println( strlen( postData ) );
    client.println( "" );
    client.print( postData );
  }
	else
	{
		Serial.println( "Unable to connect to server." );
		Serial.println( "DELAY" );
		Serial.print("Free ram: ");
		Serial.println(freeRam());
		delay( 1000 );
	}
}


// receiveClientMessage(): read from the client until no more data. Data stored in readString[]. readString[] passed by parent program.

int ClientDefinitions::receiveClientMessage( char* readString )
{
	bool lastCharWasCR = false;
	bool lastCharWasLF = false;
	int readStringIndex = 0;
	unsigned char crlfCount = 0;
	// empty readString
	memset( readString, NULL, CLIENT_BUFFER_LENGTH );
	
	while ( client.available( ) )
	{
    readFromClient( readString, &lastCharWasCR, &lastCharWasLF, &readStringIndex, &crlfCount );
		//Serial.print( "readFromClient result: " );
		//Serial.println( readString );
  }
	Serial.println( "Successfully read message." );
	
	Serial.print("Free ram: ");
	Serial.println(freeRam());
	
	return readStringIndex;	
}

// readFromClient(): read and append next character from client to readString[].

void ClientDefinitions::readFromClient( char* readString, bool *lastCharWasCR, bool *lastCharWasLF, int *readStringIndex, unsigned char *crlfCount )
{
  char c = client.read( );
  if ( ( *crlfCount ) >= 2 )
	{
    memset( readString + *readStringIndex, c, sizeof( c ) );
    ( *readStringIndex )++;
		//Serial.print( "readFromClient result: " );
		//Serial.println( readString );
  }
  
  if ( *lastCharWasLF )
	{
    if ( c == CR )
		{
      *lastCharWasCR = true;
      *lastCharWasLF = false;
    }
		else
		{
      *lastCharWasCR = false;
      *lastCharWasLF = false;
      if ( *crlfCount != 2 )
			{
        *crlfCount = 0;
      }
    }
  }
	else if ( *lastCharWasCR && c == LF )
	{
    ( *crlfCount )++;
    *lastCharWasCR = false;
    *lastCharWasLF = true;
  }
	else if ( c == CR )
	{
    *lastCharWasCR = true;
    *lastCharWasLF = false;
  }
}


int ClientDefinitions::freeRam ( ) {
  extern int __heap_start, *__brkval; 
  int v; 
  return ( int ) &v - ( __brkval == 0 ? ( int ) &__heap_start : ( int ) __brkval ); 
}

//ClientDefinitions ClientDefinitions;