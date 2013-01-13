/*
  Web client
 
 This sketch connects to a website (http://www.google.com)
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 
 created 18 Dec 2009
 modified 9 Apr 2012
 by David A. Mellis
 
 */

#include <SPI.h>
#include <Ethernet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define BUILD 1 // Update this with every commit, m'kay?

// Sleep delay
#ifndef DELAY
  #define DELAY 60000
#endif
// Server to connect to
#ifndef HOST1
  #define HOST1 66
#endif
#ifndef HOST2
  #define HOST2 96
#endif
#ifndef HOST3
  #define HOST3 223
#endif
#ifndef HOST4
  #define HOST4 75
#endif
// Port to connect to
#ifndef PORT
  #define PORT 80
#endif
// MAC address
#ifndef MAC1
  #define MAC1 0x00
#endif
#ifndef MAC2
  #define MAC2 0x11
#endif
#ifndef MAC3
  #define MAC3 0x22
#endif
#ifndef MAC4
  #define MAC4 0x33
#endif
#ifndef MAC5
  #define MAC5 0x44
#endif
#ifndef MAC6
  #define MAC6 0x56
#endif

byte mac[] = {  MAC1, MAC2, MAC3, MAC4, MAC5, MAC6  };
IPAddress server( HOST1, HOST2, HOST3, HOST4 );


// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
  Serial.print("My address:");
  Serial.println(Ethernet.localIP());
  
  // give the Ethernet shield a second to initialize:
  delay(1000);
}

void loop()
{
  int reading = 1;
  Serial.println( "Connecting..." );
  // if you get a connection, report back via serial:
  if ( client.connect( server, PORT ) ) {
    Serial.println("connected");
    // Make a HTTP request:
    client.println("GET / HTTP/1.1");
    client.println();
  } 
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
    reading = 0;
  }
  // if there are incoming bytes available 
  // from the server, read them and print them:
  if( reading == 1 ) Serial.println( "Reading..." );
  while( reading == 1 && client.available( ) )
  {
    char c = client.read( );
    Serial.print( c );
    
    if( !client.connected( ) )
    {
      reading = 0;
    }
  }
  // Stop the client, then reconnect later.
  reading = 0;
  client.stop( );

  Serial.println("Sleeping...");
  delay( DELAY );
}

