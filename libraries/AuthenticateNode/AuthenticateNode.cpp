#include "AuthenticateNode.h"
#include "ClientDefinitions.h"
#include "Arduino.h"
#include "EEPROMx.h"

// start(): the top function called from node setup() method, handels authentication of node and returns a pointer to the new one-time key

char* AuthenticateNodeClass::getOneTimeKey( )
{
	ClientDefinitionsClass test;
	char oneTimeKey[ KEY_LENGTH + 1 ];
	
	while( !test.client.available( ) )
	{
		Serial.println( "Client not available." );
	}
	
	messageLength = test.receiveClientMessage( readString );

	while( test.client.connected( ) || test.client.available( ) ) { }	//wait

	Serial.println( "Cleint disconnected" );
  test.client.stop( );

  if( messageLength > 0 )
	{
		Serial.println( "processAuthenticationMessage here" );
    processAuthenticationMessage( oneTimeKey );
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

void AuthenticateNodeClass::processAuthenticationMessage( char* oneTimeKey )
{
	char newInitKey[ KEY_LENGTH + 1 ];
	
	Serial.println( readString );
	
	// Get status
	strncpy ( status, strstr ( &readString[ 0 ], PARM_STATUS ) + strlen( PARM_STATUS ), STATUS_LENGTH ); // Get start of status
	
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
		
		// Get new one-time key
		strncpy ( oneTimeKey, strstr ( &readString[ 0 ], PARM_NEW_ONE_TIME_KEY ) + strlen( PARM_NEW_ONE_TIME_KEY ), KEY_LENGTH ); // Get start of new init key
		
		if ( oneTimeKey != NULL )
		{
			Serial.print ( "New one-time key is ");
	          Serial.println ( oneTimeKey );
		}
	}
}	
	
AuthenticateNodeClass AuthenticateNode;
	