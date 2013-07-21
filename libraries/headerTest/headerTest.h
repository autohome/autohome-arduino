#ifndef headerTest_h
#define headerTest_h

void test(){
	Serial.begin(9600);
	char text[8] = {'t','e','s','t','i','n','g'};
	String str = "text above?";
	Serial.println( text );
	Serial.println(str);
	Serial.println("any text?");
}


#endif