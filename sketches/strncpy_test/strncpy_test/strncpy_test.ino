void setup() {
  Serial.begin( 9600 );
  char * readString;
  char c = 't';
  readString = ( char * ) malloc( 255 + 1 );
  int * readStringIndex;
  
  //Serial.print("sizeof( readString ) = " );
  //Serial.println(strlen( readString ));
  memset( readString, NULL, 255 + 1 );
  
  *readStringIndex = 0;
  //strncpy( readString + *readStringIndex, &c, sizeof( c ) );
  memset( readString + *readStringIndex, c, sizeof( c ) );
  
  c = 'y';
  *readStringIndex = 1;
  memset( readString + *readStringIndex, c, sizeof( c ) );

  c = 'l';  
  *readStringIndex = 2;
  memset( readString + *readStringIndex, c, sizeof( c ) );
  

  Serial.println( readString );
}

void loop() {
  // put your main code here, to run repeatedly: 
  
}
