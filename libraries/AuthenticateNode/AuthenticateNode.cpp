#include "AuthenticateNode.h"
#include "ClientDefinitions.h"
#include "Arduino.h"
#include "EEPROMx.h"

// start(): the top function called from node setup() method, handels authentication of node and returns a pointer to the new one-time key

AuthenticateNode::AuthenticateNode( ) { }

AuthenticateNode::AuthenticateNode( ClientDefinitions* clientPointer )
{
	test = clientPointer;
}

void AuthenticateNode::setClientDefinitions( ClientDefinitions* clientPointer )
{
	test = clientPointer;
}


char* AuthenticateNode::getOneTimeKey( )
{
	//char oneTimeKey[ KEY_LENGTH + 1 ];
//	char readString[ CLIENT_BUFFER_LENGTH + 1 ];
	char * readString;
	char * oneTimeKey;
	
	oneTimeKey = ( char * ) malloc( KEY_LENGTH + 1 );	
	memset( oneTimeKey, NULL, KEY_LENGTH + 1 );
	readString = ( char * ) malloc( CLIENT_BUFFER_LENGTH + 1 );
	Serial.print("Free ram: ");
	Serial.println(freeRam());
		
	while( !test->client.available( ) )
	{
		Serial.println( "Client not available." );
	}
	Serial.println( "Client available." );
	
	messageLength = test->receiveClientMessage( readString );
	Serial.print("Data received: ");
	Serial.println( readString );

	while( test->client.connected( ) || test->client.available( ) ) { }	// wait

	//Serial.println( "Cleint disconnected" );
  test->client.stop( );

  if( messageLength > 0 )
	{
		Serial.println( "processAuthenticationMessage here" );
    processAuthenticationMessage( oneTimeKey, readString );
		return oneTimeKey;
  }
	else
	{
		return NULL;
	}
}

// processAuthenticationMessage( ): checks if initialization was successful and if so gets and writes a new
// 																initializtion key into EEPROM and also stores a one-time key in RAM. If
//																initialization failed retry with the original key.

void AuthenticateNode::processAuthenticationMessage( char* oneTimeKey, char * readString )
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

int AuthenticateNode::freeRam ( ) {
  extern int __heap_start, *__brkval; 
  int v; 
  return ( int ) &v - ( __brkval == 0 ? ( int ) &__heap_start : ( int ) __brkval ); 
}
	
//AuthenticateNode AuthenticateNode;
	