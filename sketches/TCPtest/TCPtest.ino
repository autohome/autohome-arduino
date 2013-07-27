#include <TCPSocket.h>
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
	TCPSocket clientCommunication = TCPSocket( ORIG_INIT );
}
  
void loop( ) { }
