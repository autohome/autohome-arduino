#include <TCPSocket.h>
#include <Ethernet.h>
#include <SPI.h>
#include <EEPROMx.h>
#include <Wiegand.h>

#ifndef ORIG_INIT
#define ORIG_INIT "8e66e3673d8a7eaf9fbae04b2059ec67bb0bfae7d45a072d93144a624d4e9314"
#endif

WIEGAND wg;

void setupReader( )
{
  wg.begin();
}

void setup( )
{
	delay( 1000 );
	TCPSocket clientCommunication = TCPSocket( ORIG_INIT );

	setupReader( );
}
  
void loop( )
{
	if(wg.available())
  {
    Serial.println(wg.getCode());
  }
}
