#ifndef authenticateNode_h
#define authenticateNode_h

#include "Arduino.h"

// authenticateNode.h: declares parameters associated with node authentication

/*********************************************************
 * BEGIN Authorization Key Definitions                   *
 *********************************************************/

#ifndef KEY_LENGTH
#define KEY_LENGTH 64		// both initialization and one-time key
#endif

#ifndef STATUS_LENGTH
#define STATUS_LENGTH 1
#endif

#ifndef KEY_INIT
#define KEY_INIT "&initialization_key="
#endif

#ifndef KEY_ONETIME
#define KEY_ONETIME "&one_time_key="
#endif

#ifndef KEY_MAC
#define KEY_MAC "mac_address="
#endif

#ifndef PARM_STATUS
#define PARM_STATUS "status:"
#endif

#ifndef PARM_NEW_ONE_TIME_KEY
#define PARM_NEW_ONE_TIME_KEY "newonetime:"
#endif

#ifndef PARM_NEW_INIT_KEY
#define PARM_NEW_INIT_KEY "newinit:"
#endif
/*
char status[STATUS_LENGTH+1];
char oneTimeKey[KEY_LENGTH+1];
char newInitKey[KEY_LENGTH+1];
*/
/*********************************************************
 *   END Authorization Key Definitions                   *
 *********************************************************/

#ifndef CLIENT_BUFFER_LENGTH
#define CLIENT_BUFFER_LENGTH 200
#endif

class AuthenticateNodeClass
{
	public:
		char* start();
	
	private:
		char readString[CLIENT_BUFFER_LENGTH+1];
		int messageLength;
		char status[STATUS_LENGTH+1];
		char oneTimeKey[KEY_LENGTH+1];
		char newInitKey[KEY_LENGTH+1];
		void processAuthenticationMessage( char* oneTimeKey );	// aka authorizeNode()
		int processAuthenticationResult();
	
};

extern AuthenticateNodeClass AuthenticateNode;

#endif