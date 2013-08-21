#include <TCPSocket.h>
#include <Ethernet.h>
#include <SPI.h>
#include <EEPROMx.h>
#include <Wiegand.h>

//char * packageDoorData( unsigned long rfidID );
void sendRfidPackage( unsigned long rfidID, TCPSocket socket );
void setupReader( );
int freeRam ( );

#ifndef ORIG_INIT
#define ORIG_INIT "12bc15089d985ebe0ff3e803170f94e68975325b9e2b60e1ed894a76b6ceb225"
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

#ifndef PIN_DATA0
#define PIN_DATA0 2
#endif

#ifndef PIN_DATA1
#define PIN_DATA1 3
#endif

#ifndef PIN_LED
#define PIN_LED 4
#endif

#ifndef PIN_BEEP
#define PIN_BEEP 5
#endif


void setup( )
{
	Serial.begin( 9600 );
	Serial.print( "Free ram at start: " );
	Serial.println( freeRam( ) );
	delay( 1000 );
	setupReader( );
}
  
void loop( )
{
	static unsigned long rfidID;
	static TCPSocket socket = TCPSocket( ORIG_INIT, mac );

	
	if( wg.available( ) )
  {
		Serial.print( "Free ram: " );
		Serial.println( freeRam( ) );
		rfidID = wg.getCode( );
    //Serial.println( rfidID );
		//Serial.println( packageDoorData( rfidID ) );
		sendRfidPackage( rfidID, socket );
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

int processClientMessage( char* message, TCPSocket socket )
{
  int statusInt;
  char * status;
  
  status = ( char * ) malloc( STATUS_LENGTH + 1 );
  memset( status, NULL, STATUS_LENGTH + 1 );
  
  // Get status
  strncpy ( status, strstr ( message, PARM_STATUS ) + strlen ( PARM_STATUS ), STATUS_LENGTH );
  
  statusInt = atoi( status );
  
  // Get new OTK
  socket.setOneTimeKey( strstr ( &message[ 0 ], PARM_NEW_ONE_TIME_KEY ) + strlen( PARM_NEW_ONE_TIME_KEY ) );
  
  if( statusInt == 0 && status[0] == '0' )
  {
    free( status );
    
    // TODO: Output pins!
    Serial.println("SUCCESS!");
    digitalWrite( PIN_LED, LOW );
    // Sleep for default of 5 seconds (unless otherwise specified)
    delay( 5000 );
    digitalWrite( PIN_LED, HIGH );
  }
  else
  {
    Serial.println("FAILURE!");
    free( status );
    if( statusInt == 0 )
    {
      statusInt = -1;
    }
  }
  
  return statusInt;
}
void sendRfidPackage( unsigned long rfidID, TCPSocket socket )
{
	char * data;
        int msgLength = 0;
        char* received;
	
	data = ( char * ) malloc( strlen( RFID_ID ) + RFID_ID_VALUE_LENGTH + 1 );
  memset( data, NULL, strlen( RFID_ID ) + RFID_ID_VALUE_LENGTH + 1 );
	
	sprintf( data, "%s%lu", RFID_ID, rfidID );
	Serial.println( data );
	
	socket.sendClientMessage( RFID_URI, data );
        free( data );
        received = ( char * ) malloc( sizeof( char ) * ( CLIENT_BUFFER_LENGTH + 1 ) );
        socket.receiveClientMessage( received );
        Serial.println( received );
        processClientMessage( received, socket );
	
	free( received );
}

void setupReader( )
{
  wg.begin( );
  
  pinMode( PIN_DATA0, INPUT );
  pinMode( PIN_DATA1, INPUT );
  pinMode( PIN_LED, OUTPUT );
  pinMode( PIN_BEEP, OUTPUT );
  
  digitalWrite( PIN_LED, HIGH );
  digitalWrite( PIN_BEEP, HIGH );
}

int freeRam ( ) {
  extern int __heap_start, *__brkval; 
  int v; 
  return ( int ) &v - ( __brkval == 0 ? ( int ) &__heap_start : ( int ) __brkval ); 
}
