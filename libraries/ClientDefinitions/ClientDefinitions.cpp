#include "ClientDefinitions.h"
#include "Arduino.h"
#include <Ethernet.h>
#include <SPI.h>
#include <EEPROMx.h>

//EthernetServer server;

// ClientDefinitionsClass Constructor class: initializes EthernetServer server via an "initialization list" and then
//																						the rest of the server/ethernet setup

byte mac[] = { 
  MAC0, MAC1, MAC2, MAC3, MAC4, MAC5 };

byte ip[] = { 
  IP0, IP1, IP2, IP3 };

ClientDefinitionsClass::ClientDefinitionsClass() : 
server( LOCAL_SERVER_PORT )	// initialization list
{
	char postDataInit[123];
	int authTryCount = 0;
  Serial.begin(9600);
	
	initEthernet();
	server.begin();
	setupServer();
	setupClient(true, &authTryCount);
	Serial.println("Success.");
}

void ClientDefinitionsClass::initEthernet() {
  Serial.println("Initializing ethernet port...");
  Ethernet.begin(mac);
}

void ClientDefinitionsClass::setupServer() {
  Serial.print("Starting server...");
  server.begin();
  Serial.print("started at ");
  Serial.println(Ethernet.localIP());
}

// setupClient(): initialize communication client between local node (arduino) and remote server

void ClientDefinitionsClass::setupClient( boolean secondTry, int *authTryCount ) {
	char key[KEY_LENGTH+1];
	char postDataInit[123];
	
  Serial.println("Starting client...");
  // Build payload
  if ( secondTry == true ) {
		// Use original initialization key if EEPROM key is invalid
    strncpy(key, ORIG_INIT, KEY_LENGTH);
    *authTryCount = 1;
  } else {
		// first try the maintained EEPROM key
    Serial.print("Reading EEPROM...\"");
    EEPROMx.readFromEEPROM(key, KEY_LENGTH);
    Serial.print(key);
    Serial.println("\"");
  }
  
  sprintf(postDataInit, "%s%0.2x%%3A%0.2x%%3A%0.2x%%3A%0.2x%%3A%0.2x%%3A%0.2x%s%s", KEY_MAC, mac[0], mac[1],
    mac[2], mac[3], mac[4], mac[5], KEY_INIT, key);
  
  if (client.connect(ip, REMOTE_SERVER_PORT)) {
    Serial.println("Client connected.");
    client.println("POST /api/v1/online.txt HTTP/1.1");
    client.println("Content-Type: application/x-www-form-urlencoded; charset=UTF-8"); 
    client.println("Host: dev-environment:3000");
    client.println("Connection: close");
    client.print("Content-Length: ");
    client.println(strlen(postDataInit));
    client.println("");
    client.print(postDataInit);
  } else {
		Serial.println("Unable to connect to server.");
	}
}


// receiveClientMessage(): read from the client until no more data. Data stored in readString[]. readString[] passed by parent program.

int ClientDefinitionsClass::receiveClientMessage(char* readString) {
	bool lastCharWasCR = false;
	bool lastCharWasLF = false;
	readStringIndex = 0;
	crlfCount = 0;
	// empty readString
	memset( &readString, '0', sizeof( readString ) );
	
	while (client.available()) {
    readFromClient(readString, &lastCharWasCR, &lastCharWasLF);
		Serial.println("Cleint Read");
  }
	Serial.println("Read Complete.");
	
	return readStringIndex;	
}

// readFromClient(): read and append next character from client to readString[].

void ClientDefinitionsClass::readFromClient(char* readString, bool *lastCharWasCR, bool *lastCharWasLF) {
  char c = client.read();
  if ( crlfCount >= 2 ) {
    readString[readStringIndex] = c;
    readStringIndex++;
  }
  
  if ( *lastCharWasLF ) {
    if ( c == CR ) {
      *lastCharWasCR = true;
      *lastCharWasLF = false;
    } else {
      *lastCharWasCR = false;
      *lastCharWasLF = false;
      if ( crlfCount != 2 ) {
        crlfCount = 0;
      }
    }
  } else if ( *lastCharWasCR && c == LF ) {
    crlfCount++;
    *lastCharWasCR = false;
    *lastCharWasLF = true;
  } else if ( c == CR ) {
    *lastCharWasCR = true;
    *lastCharWasLF = false;
  }
}


//ClientDefinitionsClass ClientDefinitions;