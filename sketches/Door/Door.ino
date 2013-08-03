#include <TCPSocket.h>
#include <Ethernet.h>
#include <SPI.h>
#include <EEPROMx.h>
#include <Wiegand.h>

#ifndef ORIG_INIT
#define ORIG_INIT "bd6b4bfea9930541b33ced8f2f7674ceb084836ae81d8452de7c00135bbe3632"
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

byte mac[ ] = { MAC0, MAC1, MAC2, MAC3, MAC4, MAC5 };

WIEGAND wg;

void setupReader( )
{
  wg.begin();
}

void setup( )
{
	delay( 1000 );
	TCPSocket clientCommunication = TCPSocket( ORIG_INIT, mac );

	setupReader( );
}
  
void loop( )
{
	if(wg.available())
  {
    Serial.println(wg.getCode());
  }
}
