#include <EEPROMx.h>
#include <SPI.h>
#include <Ethernet.h>
#include <ClientDefinitions.h>
#include <AuthenticateNode.h>

#ifndef LOCAL_SERVER_PORT
#define LOCAL_SERVER_PORT 8080
#endif

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


void setupPins() {
  pinMode(PIN_DATA0, INPUT);
  pinMode(PIN_DATA1, INPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  
  digitalWrite(PIN_LED, HIGH);
  digitalWrite(PIN_BUZZER, HIGH);
}


void setup() {
  Serial.begin(9600);
	delay(2000);
  setupPins();
  ClientDefinitions.

	AuthenticateNode.start();
}


void loop() {
  
	Serial.println("Done.");
	delay(10000);  
}
