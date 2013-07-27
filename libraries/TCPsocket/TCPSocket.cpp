#include "TCPSocket.h"
#include "Arduino.h"
#include <Ethernet.h>
#include <SPI.h>
#include <EEPROMx.h>

//EthernetServer server;

// TCPSocket Constructor class: initializes EthernetServer server via an "initialization list" and then
//																						the rest of the server/ethernet setup

byte mac[ ] = { 
  MAC0, MAC1, MAC2, MAC3, MAC4, MAC5 };

byte ip[ ] = { 
  IP0, IP1, IP2, IP3 };

TCPSocket::TCPSocket( char * initKey ) : 
server( LOCAL_SERVER_PORT )	// initialization list
{
	char * readString;
	int messageLength;
	
	oneTimeKey = ( char * ) malloc( KEY_LENGTH + 1 );	
	readString = ( char * ) malloc( CLIENT_BUFFER_LENGTH + 1 );
	
	memset( oneTimeKey, NULL, KEY_LENGTH + 1 );
	
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
	
	while( !client.available( ) )
	{
		Serial.println( "Client not available." );
	}
	Serial.println( "Client available." );
	
	messageLength = receiveClientMessage( readString );
	Serial.print("Data received: ");
	Serial.println( readString );

	while( client.connected( ) || client.available( ) ) { }	// wait

	//Serial.println( "Cleint disconnected" );
  client.stop( );

  if( messageLength > 0 )
	{
		Serial.println( "processAuthenticationMessage here" );
    processAuthenticationMessage( readString );
  }
}

void TCPSocket::initEthernet( )
{
  Serial.println( "Initializing ethernet port..." );
  Ethernet.begin( mac );
	Serial.println( "Ethernet port initialized." );
}

void TCPSocket::setupServer( )
{
  Serial.print( "Starting server..." );
  server.begin( );
  Serial.print( "started at " );
  Serial.println( Ethernet.localIP( ) );
}

// setupClient(): initialize communication client between local node (arduino) and remote server

void TCPSocket::setupClient( boolean secondTry, unsigned char *authTryCount, char * initKey )
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

int TCPSocket::receiveClientMessage( char* readString )
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

void TCPSocket::readFromClient( char* readString, bool *lastCharWasCR, bool *lastCharWasLF, int *readStringIndex, unsigned char *crlfCount )
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


// processAuthenticationMessage( ): checks if initialization was successful and if so gets and writes a new
// 																initializtion key into EEPROM and also stores a one-time key in RAM. If
//																initialization failed retry with the original key.

void TCPSocket::processAuthenticationMessage( char * readString )
{
	//char newInitKey[ KEY_LENGTH + 1 ];
	char * status;
	char * newInitKey;
	
	newInitKey = ( char * ) malloc( KEY_LENGTH + 1 );
	status = ( char * ) malloc( STATUS_LENGTH + 1 );
	
	memset( status, NULL, STATUS_LENGTH + 1 );
	memset( newInitKey, NULL, KEY_LENGTH + 1 );
		
	// Get status
	strncpy ( status, strstr ( readString, PARM_STATUS ) + strlen( PARM_STATUS ), STATUS_LENGTH ); // Get status value
		
		Serial.print ( "All text following status: " );
		Serial.println ( status );
		
		// If successfully verified key
		if( atoi( status ) == 0 )
		{
			Serial.println( "Suceeded in verifying key." );
			
			// Get new initialization key
			strncpy ( newInitKey, strstr ( &readString[ 0 ], PARM_NEW_INIT_KEY ) + strlen( PARM_NEW_INIT_KEY ), KEY_LENGTH ); // Get start of new init key
			
			// Write the key to EEPROM
			if ( newInitKey != NULL )
			{
				Serial.print ( "New initialization key is ");
		          Serial.println ( newInitKey );
		    EEPROMx.writeToEEPROM( newInitKey );
				Serial.println( "Wrote to EEPROM." );
			}
			
			// de-allocated memory for initialization key after storing to EEPROM
			free( newInitKey );		
		
			// Get new one-time key
			strncpy ( oneTimeKey, strstr ( &readString[ 0 ], PARM_NEW_ONE_TIME_KEY ) + strlen( PARM_NEW_ONE_TIME_KEY ), KEY_LENGTH ); // Get start of new init key
			
			if ( oneTimeKey != NULL )
			{
				Serial.print ( "New one-time key is ");
		          Serial.println ( oneTimeKey );
			}
		}
}

int TCPSocket::freeRam ( ) {
  extern int __heap_start, *__brkval; 
  int v; 
  return ( int ) &v - ( __brkval == 0 ? ( int ) &__heap_start : ( int ) __brkval ); 
}

//TCPSocket TCPSocket;