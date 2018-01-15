
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

int main1(int argc, const char * argv[])
{
    CxApplication::init(argc, argv);

    //startTimer11();

    CxApplication::exec();

    return 0;
}

std::string ginput()
{
	std::string msg;
	std::cin >> msg;
	return msg;
}

int main2(int argc, const char*argv[])
{
	//应用初始化
	CxApplication::init(argc, argv);

	cxPrompt() << CxAppEnv::applicationTargetName();

	while (1) {
		string s1 = ginput();
		if (s1.find("exit") != string::npos) exit(0);
		std::cout << s1 << std::endl;
	}

	CxApplication::exec();

	return 0;
}

int fn_interinfo_in_cmd(const std::string & sCommand, const std::map<std::string, std::string> & sParams, const msepoch_t & dtIn, int iSource, int eInType, int iTag)
{
	if (sCommand == "exit")
	{
		CxApplication::exit();
		return TRUE;
	}
	return FALSE;
}


void fn_test(int, int, const void *, int, void *, void *)
{
	cxDebug() << "hello";
}

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

void fn_timer_timeout11(int iInterval)
{
	return;

	if (SetConsoleCtrlHandler(
		(PHANDLER_ROUTINE)ConsoleHandler,TRUE)==FALSE)
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

int main(int argc, const char * argv[])
{
	CxApplication::init(argc, argv);

	CxApplication::pushProcessCallBack(fn_test);

	CxInterinfoIn::addObserver(fn_interinfo_in_cmd);

	CxTimerManager::startTimer(fn_timer_timeout11, 5000);

	CxApplication::exec();

	return 0;
}

