#include <TCPSocket.h>
#include <Ethernet.h>
#include <SPI.h>
#include <EEPROMx.h>
#include <Wiegand.h>

//char * packageDoorData( unsigned long rfidID );
// void sendRfidPackage( unsigned long rfidID, TCPSocket socket );
void setupReader( );

#ifndef ORIG_INIT
#define ORIG_INIT "a33e39051021df7b3e12e84b3d0312c9182020257cdaa5d7307c1b58603508ea"
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

#ifndef RFID_ID
#define RFID_ID "&rfid_id="
#endif

#ifndef RFID_ID_VALUE_LENGTH
#define RFID_ID_VALUE_LENGTH 10
#endif

#ifndef RFID_URI
#define RFID_URI "/api/v1/auth/rfid.txt"
#endif


void setup( )
{
	delay( 1000 );
	setupReader( );
}
  
void loop( )
{
	static unsigned long rfidID;
	static TCPSocket socket = TCPSocket( ORIG_INIT, mac );
	
	if( wg.available( ) )
  {
		rfidID = wg.getCode( );
    //Serial.println( rfidID );
		//Serial.println( packageDoorData( rfidID ) );
		//sendRfidPackage( rfidID, socket );
  }
}

// char * packageDoorData(): writes the rfidID into a char[] structured for acceptance by autohome server
// 
// char * packageDoorData( unsigned long rfidID )
// {
// 	char * data;
// 	data = ( char * ) malloc( strlen( RFID_ID ) + RFID_ID_VALUE_LENGTH + 1 );
// 	
// 	sprintf( data, "%s%lu", RFID_ID, rfidID );
// 	
// 	return data;
// }
// 
// 
void sendRfidPackage( unsigned long rfidID, TCPSocket socket )
{
	char * data;
	
	data = ( char * ) malloc( strlen( RFID_ID ) + RFID_ID_VALUE_LENGTH + 1 );
  memset( data, NULL, strlen( RFID_ID ) + RFID_ID_VALUE_LENGTH + 1 );
	
	sprintf( data, "%s%lu", RFID_ID, rfidID );
	Serial.println( data );
	
	socket.sendClientMessage( RFID_URI, data );
	
	free( data );
}

void setupReader( )
{
  wg.begin( );
}