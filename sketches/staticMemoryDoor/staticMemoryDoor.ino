#include <EEPROMx.h>
#include <SPI.h>
#include <Wiegand.h>
#include <Ethernet.h>

/*********************************************************
 * BEGIN I/O Pin Definitions                             *
 *********************************************************/
#ifndef PIN_DATA0
#define PIN_DATA0 2
#endif

#ifndef PIN_DATA1
#define PIN_DATA1 3
#endif

#ifndef PIN_LED
#define PIN_LED 4
#endif

#ifndef PIN_BUZZER
#define PIN_BUZZER 5
#endif
/*********************************************************
 *   END I/O Pin Definitions                             *
 *********************************************************/

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
/*********************************************************
 *   END MAC Address Byte Definitions                    *
 *********************************************************/

/*********************************************************
 * BEGIN IP Address Definitions                          *
 *********************************************************/
#ifndef IP0
#define IP0 192
#endif

#ifndef IP1
#define IP1 168
#endif

#ifndef IP2
#define IP2 18
#endif

#ifndef IP3
#define IP3 131 //131 is me, 114 is brian's dev environment
#endif
/*********************************************************
 *   END IP Address Definitions                          *
 *********************************************************/

/*********************************************************
 * BEGIN Port Definitions                                *
 *********************************************************/
#ifndef LOCAL_SERVER_PORT
#define LOCAL_SERVER_PORT 8080
#endif

#ifndef REMOTE_SERVER_PORT
#define REMOTE_SERVER_PORT 3000
#endif
/*********************************************************
 *   END Port Definitions                                *
 *********************************************************/

/*********************************************************
 * BEGIN Authorization Key Definitions                   *
 *********************************************************/
#ifndef ORIG_INIT
#define ORIG_INIT "cced0c7dd8502a7c89b91e8062febed254b3cdd48cea7f8e04c904cf58905395"
#endif

#define KEY_LENGTH 64		// both initialization and one-time key
#define STATUS_LENGTH 1
/*********************************************************
 *   END Authorization Key Definitions                   *
 *********************************************************/

#define KEY_INIT "&initialization_key="
#define KEY_ONETIME "&one_time_key="
#define KEY_MAC "mac_address="

const char CR = 13;
const char LF = 10;

WIEGAND wg;
EthernetServer server( LOCAL_SERVER_PORT );
EthernetClient client;

byte mac[] = { 
  MAC0, MAC1, MAC2, MAC3, MAC4, MAC5 };
byte ip[] = { 
  IP0, IP1, IP2, IP3 };
  
char postDataInit[123];
char* postDataAuth;

char key[KEY_LENGTH];

boolean lastCharWasCR = false;
boolean lastCharWasLF = false;
int crlfCount;
int authTryCount;

#define PARM_STATUS "status:"
#define PARM_NEW_ONE_TIME_KEY "newonetime:"
#define PARM_NEW_INIT_KEY "newinit:"

#define CLIENT_BUFFER_LENGTH 200
char readString[CLIENT_BUFFER_LENGTH];
char *readString_pointer = &readString[0];
char status[STATUS_LENGTH+1];
char oneTimeKey[KEY_LENGTH+1];
char newInitKey[KEY_LENGTH+1];
//char *temp_pointer = &temp[0];
int readStringIndex;


/*********************************************************
 * BEGIN Function Definitions       							       *
 *********************************************************/

// setupPins(): initialize pin setting

void setupPins() {
  pinMode(PIN_DATA0, INPUT);
  pinMode(PIN_DATA1, INPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  
  digitalWrite(PIN_LED, HIGH);
  digitalWrite(PIN_BUZZER, HIGH);
}

// setupReader(): initialize RFID reader

void setupReader() {
  wg.begin();
}

// setupServer(): initialize node (arduio) as a server

void setupServer() {
  Serial.print("Starting server...");
  server.begin();
  Serial.print("started at ");
  Serial.println(Ethernet.localIP());
}

// setupClient(): initialize communication client between local node (arduino) and remote server

void setupClient( boolean secondTry ) {
  Serial.println("Starting client...");
  // Build payload
  if ( secondTry == true ) {
		// Use original initialization key if EEPROM key is invalid
    strncpy(key, ORIG_INIT, KEY_LENGTH);
    authTryCount = 1;
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
		Serial.print("postDataInit: ");
		Serial.println( postDataInit );
		Serial.print("postDataInit length: ");
		Serial.println( strlen( postDataInit ) );
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

// readFromClient(): read and append next character from client to readString[]. 

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

// authorizeNode(): checks if initialization was successful and if so gets and writes a new
// 									initializtion key into EEPROM and also stores a one-time key in RAM. If
//									initialization failed retry with the original key.

void authorizeNode() {
	Serial.println( readString );
	
	// Get status
	strncpy ( status, strstr ( readString_pointer, PARM_STATUS ) + strlen( PARM_STATUS ), STATUS_LENGTH ); // Get start of status
	
	Serial.print ( "All text following status: " );
	Serial.println ( status );
	
	// If successfully verified key
	if( atoi(status) == 0 ) {
		Serial.println("Suceeded in verifying key.");
		
		// Get new initialization key
		strncpy ( newInitKey, strstr ( readString_pointer, PARM_NEW_INIT_KEY ) + strlen( PARM_NEW_INIT_KEY ), KEY_LENGTH ); // Get start of new init key
		
		// Write the key to EEPROM
		if ( newInitKey != NULL ) {
			Serial.print ( "New initialization key is ");
	          Serial.println ( newInitKey );
	    EEPROMx.writeToEEPROM( newInitKey );
			Serial.println( "Wrote to EEPROM." );
		}
		
		// Get new one-time key
		strncpy ( oneTimeKey, strstr ( readString_pointer, PARM_NEW_ONE_TIME_KEY ) + strlen( PARM_NEW_ONE_TIME_KEY ), KEY_LENGTH ); // Get start of new init key
		
		if ( oneTimeKey != NULL ) {
			Serial.print ( "New one-time key is ");
	          Serial.println ( oneTimeKey );
		}
		
		// Empty readString and reset index pointer
		memset(readString_pointer,'0',CLIENT_BUFFER_LENGTH);
		readStringIndex = 0;
	}
}


void initEthernet() {
  Serial.println("Initializing ethernet port...");
  Ethernet.begin(mac);
}

/*********************************************************
 * END Function Definitions       								       *
 *********************************************************/



void setup() {
  Serial.begin(9600);
	delay(2000);
  setupPins();
  setupReader();

	readStringIndex = 0;
	authTryCount = 0;
	crlfCount = 0; 
  
  initEthernet();
  setupClient(false);	// (false) in execution
  setupServer();
}



void loop() {
  
  if (client.available()) {                        // PARSE DATA TO RECEIVE RESULT AND STORE IN MEMORY
    readFromClient();
	//Serial.println("Cleint Read");
  }
  
  if (!client.connected() && !client.available()) {
	Serial.println("Cleint not connected");
    client.stop();
    if( readStringIndex > 0 ) {
      authorizeNode();
    }
		delay(5000);
  }
  
  Serial.println("1");
  if(wg.available())
  {
    Serial.println(wg.getCode());
  }
  
}
