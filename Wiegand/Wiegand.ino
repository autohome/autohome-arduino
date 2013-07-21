#include <Wiegand.h>
#include <SPI.h>
#include <Ethernet.h>
#include <stdarg.h>
#include <WString.h>
#include <avr/pgmspace.h>
#include <EEPROMx.h>

WIEGAND wg;

#define ADMINCARD 2756273

#define PIN_DATA0  2
#define PIN_DATA1  3
#define PIN_LED    4
#define PIN_BUZZER 5

#define CR 13
#define LF 10

#ifndef ORIG_INIT
  #define ORIG_INIT "e77fa8afd1e3857ec42ec83618c86ec2ef830495e2a3f5101bbc3ea3beba240e"
#endif

#define KEY_INIT "&initialization_key="
#define KEY_ONETIME "&one_time_key="
#define KEY_MAC "mac_address="

#define KEY_LENGTH 64

#define PARM_STATUS "status:"
#define PARM_NEW_ONE_TIME_KEY "newonetime:"
#define PARM_NEW_INIT_KEY "newinit:"

byte mac[] = {  0x00, 0x11, 0x22, 0x33, 0x44, 0x56 };
byte ip[] = {192, 168, 18, 114};
#define serverport 3000

int readStringIndex = 0;
char *readString; // Buffer to read in client results

char postDataInit[123];
char *postDataAuth;

char key[KEY_LENGTH];

EthernetServer server(8080);
EthernetClient client;

boolean lastCharWasCR = false;
boolean lastCharWasLF = false;
int crlfCount = 0;
int authTryCount = 0;

void setupPins() {
  pinMode(PIN_DATA0, INPUT);
  pinMode(PIN_DATA1, INPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  
  digitalWrite(PIN_LED, HIGH);
  digitalWrite(PIN_BUZZER, HIGH);
}

void setupReader() {
  wg.begin();
}

void setupServer() {
  Serial.print("Starting server...");
  server.begin();
  Serial.print("started at ");
  Serial.println(Ethernet.localIP());
}

void setupClient( boolean secondTry ) {
  Serial.println("Starting client...");
  // Build payload
  if ( secondTry == true ) {
    strncpy(key, ORIG_INIT, KEY_LENGTH);
    authTryCount = 1;
  } else {
    Serial.print("Reading EEPROM...\"");
    EEPROMx.readFromEEPROM(key, KEY_LENGTH);
    Serial.print(key);
    Serial.println("\"");
  }
  
  sprintf(postDataInit, "%s%0.2x%%3A%0.2x%%3A%0.2x%%3A%0.2x%%3A%0.2x%%3A%0.2x%s%s", KEY_MAC, mac[0], mac[1],
    mac[2], mac[3], mac[4], mac[5], KEY_INIT, key);
  
  if (client.connect(ip, serverport)) {
    Serial.println("Client connected.");
    client.println("POST /api/v1/online.txt HTTP/1.1");
    client.println("Content-Type: application/x-www-form-urlencoded; charset=UTF-8"); 
    client.println("Host: dev-environment:3000");
    client.println("Connection: close");
    client.print("Content-Length: ");
    client.println(strlen(postDataInit));
    client.println("");
    client.print(postDataInit);
  }
}

void readFromClient() {
  char c = client.read();
  if ( crlfCount >= 2 ) {
    readString[readStringIndex] = c;
    readStringIndex++;
  }
  
  if ( lastCharWasLF ) {
    if ( c == CR ) {
      lastCharWasCR = true;
      lastCharWasLF = false;
    } else {
      lastCharWasCR = false;
      lastCharWasLF = false;
      if ( crlfCount != 2 ) {
        crlfCount = 0;
      }
    }
  } else if ( lastCharWasCR && c == LF ) {
    crlfCount++;
    lastCharWasCR = false;
    lastCharWasLF = true;
  } else if ( c == CR ) {
    lastCharWasCR = true;
    lastCharWasLF = false;
  }
}

void authorizeNode() {
  char* jsonString = (char*)malloc(sizeof(char)*strlen(readString));

	if ( jsonString != NULL ) {
		
  	memcpy(jsonString, readString, strlen(readString));
	  // Is this needed here? Causing problems?
	  free(readString);
  
	  char* temp = strstr ( jsonString, PARM_STATUS ); // Get start of status
	  char* temp2;
	  if ( temp != NULL ) {
	    temp += strlen( PARM_STATUS );
	    memcpy ( temp2, temp, 1 );
	    if ( strcmp( temp2, "0" ) == 0 ) {
		  	Serial.println("Suceeded in verifying key.");
	      free( temp2 );
	      // Success! Gather the new init key and store it, then grab the new one-time-key
	      temp = strstr ( jsonString, PARM_NEW_INIT_KEY );
	      if ( temp != NULL ) {
	        temp += strlen( PARM_NEW_INIT_KEY );
	        temp2 = (char*)malloc( sizeof(char) * KEY_LENGTH );
	        memcpy ( temp2, temp, KEY_LENGTH );
	        Serial.print ( "New initialization key is ");
                Serial.println ( temp2 );
	        EEPROMx.writeToEEPROM( temp2 );
					Serial.println( "Wrote to EEPROM." );
	        free( temp2 );
	      }
      
	      // Grab one-time key
	      temp = strstr ( jsonString, PARM_NEW_ONE_TIME_KEY );
	      if ( temp != NULL ) {
	        temp += strlen ( PARM_NEW_ONE_TIME_KEY );
	        memcpy ( temp2, temp, KEY_LENGTH );
	      }
	    } else {
	      // Authentication failed. Try once more, but with the original key
	      if ( authTryCount == 0 ) {
	        setupClient(true);
	      } else {
					Serial.println("Failed to authenticate both keys.");
				}
	    }
	  }
	  free(temp2);
	  free(temp);
	  free(jsonString);
	  readStringIndex = 0;
	} else {
		Serial.println("jsonString malloc failed.");
	}
}

void initEthernet() {
  Serial.println("Initializing ethernet port...");
  Ethernet.begin(mac);
}

void setup() {
  Serial.begin(9600);
  setupPins();
  setupReader();
  
  readString = (char*)malloc(sizeof(char)*255);
  memset(readString, 0, 255);
  
  initEthernet();
  setupClient(false);
  setupServer();
}

void loop() {
  
  if (client.available()) {                        // PARSE DATA TO RECEIVE RESULT AND STORE IN MEMORY
    readFromClient();
	Serial.println("Cleint Read");
  }
  
  if (!client.connected() && !client.available()) {
	Serial.println("Cleint not connected");
    client.stop();
    if( readStringIndex > 0 ) {
      authorizeNode();
    }
    
  }
  
  Serial.println("1");
  if(wg.available())
  {
    Serial.println(wg.getCode());
  }
  
}
