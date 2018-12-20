#include <ccxx/ccxx.h>

//#include <ucommon/file.h>
//#include <ucommon/fsys.h>

using namespace std;

/*
#include "appenv1.hpp"
#include "filesystem1.hpp"
#include "time1.hpp"
#include "timer1.hpp"
#include "string1.hpp"
*/

BOOL WINAPI ConsoleHandler(DWORD CEvent)
{
	char mesg[128];

	switch(CEvent)
	{
		case CTRL_C_EVENT:
			MessageBox(NULL,
					   "CTRL+C received!","CEvent",MB_OK);
			break;
		case CTRL_BREAK_EVENT:
			MessageBox(NULL,
					   "CTRL+BREAK received!","CEvent",MB_OK);
			break;
		case CTRL_CLOSE_EVENT:
			MessageBox(NULL,
					   "Program being closed!","CEvent",MB_OK);
			break;
		case CTRL_LOGOFF_EVENT:
			MessageBox(NULL,
					   "User is logging off!","CEvent",MB_OK);
			break;
		case CTRL_SHUTDOWN_EVENT:
			MessageBox(NULL,
					   "User is logging off!","CEvent",MB_OK);
			break;

	}
	return TRUE;
}

void testConsoleHandler1(int iInterval)
{
	if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler,TRUE)==FALSE)
	{
		// unable to install handler...
		// display message to the user
		printf("Unable to install handler!\n");
		return;
	}

    cout << "fn_timer_timeout11 -- 1 " << endl;

	return;

	raise(SIGINT);
    return;

	ungetc('a', stdin);
	ungetc('\r', stdin);
	ungetc('\n', stdin);

    cout << "fn_timer_timeout11 -- 2 " << endl;
}

