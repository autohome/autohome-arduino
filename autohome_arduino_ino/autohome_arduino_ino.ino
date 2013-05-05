/*
  Web client
 
  This is an adaptation of:
  
   created 18 Dec 2009
   by David A. Mellis
   modified 9 Apr 2012
   by Tom Igoe, based on work by Adrian McEwen
  
  for connection and communication to a local server.
 
 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
IPAddress server(192,168,18,114);  // numeric IP for Google (no DNS)
//char server[] = "dev-environment";    // name address for Google (using DNS)

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192,168,0,177);

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
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

  String PostData = "initialization_key=c41c9c6a98d8cae08a32d82a8d069e1a2b5f9290f87aa620cee6d2695498cf1a&mac_address=00%3A11%3A22%3A33%3A44%3A56";

  // if you get a connection, report back via serial:
  if (client.connect(server, 3000)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.println("POST /api/v1/online HTTP/1.1");
    
    client.println("Content-Type: application/x-www-form-urlencoded; charset=UTF-8"); 
    client.println("Host: dev-environment:3000");
    client.println("Connection: close");
    client.print("Content-Length: ");
    client.println(PostData.length());
    client.println();
    client.println(PostData);    
  } 
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

void loop()
{
  // if there are incoming bytes available 
  // from the server, read them and print them:
  if (client.available()) {                        // PARSE DATA TO RECEIVE RESULT AND STORE IN MEMORY
    char c = client.read();
    Serial.print(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    // do nothing forevermore:
    while(true);
  }
}
