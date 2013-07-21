#include <AuthenticateNode.h>
#include <ClientDefinitions.h>
#include <Ethernet.h>
#include <SPI.h>
#include <EEPROMx.h>

#ifndef LOCAL_SERVER_PORT
#define LOCAL_SERVER_PORT 8080
#endif

void setup( )
{

		
		//int mac = 10;
		//byte mac[] = { 
	  	//0x00, 0x11, 0x22, 0x33, 0x44, 0x56 };
	//Serial.begin(9600);
//		ClientDefinitionsClass test(mac,LOCAL_SERVER_PORT);

		AuthenticateNodeClass test;
                test.getOneTimeKey( );
}
  
void loop( ) { }
