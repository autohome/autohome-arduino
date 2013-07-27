#include <AuthenticateNode.h>
#include <ClientDefinitions.h>
#include <Ethernet.h>
#include <SPI.h>
#include <EEPROMx.h>

#ifndef ORIG_INIT
#define ORIG_INIT "8e66e3673d8a7eaf9fbae04b2059ec67bb0bfae7d45a072d93144a624d4e9314"
#endif

void setup( )
{
  char * oneTimeKey;
	delay( 1000 );
	ClientDefinitions clientCommunication = ClientDefinitions( ORIG_INIT );
//	clientCommunication = ClientDefinitions( ORIG_INIT );

	AuthenticateNode test;
	test = AuthenticateNode( &clientCommunication );
	oneTimeKey = test.getOneTimeKey( );
	if( oneTimeKey != NULL )
	{
		Serial.print( "oneTimeKey: " );
    Serial.println( oneTimeKey );
		Serial.println( "End of setup()." );
	}
	else
	{
		Serial.println( "oneTimeKey = NULL" );
	}
}
  
void loop( ) { }
