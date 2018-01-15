
#include <ccxx/ccxx.h>

#include <ccxx/cxdatabase_sqlite.h>
#include <ccxx/cxdatabase_odbc.h>

using namespace std;

static string f_sName = "testpromutex1";
static string f_sFileName = "f:\\000.txt";

static int iCount = 0;

CxProcMutex * f_oProcMutext = NULL;

int fn_test_procMutex()
{
    if (f_oProcMutext)
    {
//        CxProcMutex mProcMutex(f_sName);
//        CxProcMutexScope lock(mProcMutex);
        CxProcMutexScope lock(f_oProcMutext);

        string sWrite = CxApplication::applicationTargetName() + " - " + CxTime::currentDayString() + "\r\n\r\n";
        FILE * pFile;
        pFile = fopen (f_sFileName.c_str(), "ab+");
        if (pFile==NULL) return -1;
        size_t iWrote = fwrite (const_cast<char *>(sWrite.data()) , 1, sWrite.size(), pFile);
        fflush(pFile);
        fclose (pFile);

        return iWrote;
    }
    else
    {
        return -101;
    }
}

void fn_timer_timeout(int iInterval)
{
    int iWrote = fn_test_procMutex();

    cxPrompt() << CxTime::currentMsepochString();
    cxPrompt() << "fn_test_procMutex.result=" << iWrote << ++iCount;

    int iPing = CxNetwork::ping("10.31.58.5");
    cxPrompt() << "ping : " << iPing;
}

int main(int argc, const char*argv[])
{
    //应用初始化
    CxApplication::init(argc, argv);

    CxProcMutex mProcMutex(f_sName);
    f_oProcMutext = &mProcMutex;

    CxDatabase * oDb = CxDatabaseManager::getDefaultDb();
    if (oDb)
    {
        cxPrompt() << "Database.connectSource:" << oDb->connectSource();
        cxPrompt() << "Database.isOpen:" << oDb->isOpen();
    }
    else
    {
        cxPrompt() << "No DefaultDb!";
    }

    CxTimerManager::startTimer(fn_timer_timeout, 30);

    int iResult = CxApplication::exec();

    f_oProcMutext = NULL;

    return iResult;
}

