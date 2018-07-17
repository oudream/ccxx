
#include <ccxx/ccxx.h>

using namespace std;

CxDatabase::CursorBase * f_oCursor1 = NULL;
CxDatabase::CursorBase * f_oCursor2 = NULL;

void timerCursor1(int iInterval)
{
    static int iIndex = 0;
    ++ iIndex;
    msepoch_t dtNow = CxTime::currentMsepoch();
    CxDatabase * oDb = CxDatabaseManager::getDefaultDb();
    if (oDb == NULL)
    {
        cxPrompt() << "can not open database!";
        return;
    }

    if (f_oCursor1 == NULL)
    {
        string sSql = "select * from ha_appconfig";
        f_oCursor1 = oDb->cursorLoad(sSql, 10);
    }

    if (f_oCursor2 == NULL)
    {
        string sSq2 = "select * from omc_neconfig";
        f_oCursor2 = oDb->cursorLoad(sSq2, 10);
    }

    if (! oDb->cursorIsEnd(f_oCursor1))
    {
        vector<std::vector<std::string> > rows;
        oDb->cursorPut(f_oCursor1, rows);
        if (rows.size() > 0)
        {
            vector<string> row = CxEncoding::utf8ToGb2312(rows[0]);
            cxPrompt() << "cursor1 cursorPut row count : " << rows.size()
                       << " , print line0: ";
            cxPrompt() << row;
        }
    }
    if (oDb->cursorIsEnd(f_oCursor1))
    {
        oDb->cursorClose(f_oCursor1);
        f_oCursor1 = NULL;
    }

    if (! oDb->cursorIsEnd(f_oCursor2))
    {
        vector<std::vector<std::string> > rows;
        oDb->cursorPut(f_oCursor2, rows);
        if (rows.size() > 0)
        {
            vector<string> row = CxEncoding::utf8ToGb2312(rows[0]);
            cxPrompt() << "cursor2 cursorPut row count : " << rows.size()
                       << " , print line0: ";
            cxPrompt() << row;
        }
    }
    if (oDb->cursorIsEnd(f_oCursor2))
    {
        oDb->cursorClose(f_oCursor2);
        f_oCursor2 = NULL;
    }

    cxPrompt() << CxTime::milliSecondDifferToNow(dtNow);
    cxPrompt() << " --- --- --- --- ---";

    if (iIndex % 100 == 0)
    {
        cxPrompt() << "DB has cursor count : " << oDb->cursorGetOnlineAll().size();
        cxPrompt() << " --- --- --- --- ---";
    }
}

void testCursor1()
{
    cxPrompt() << "TEST Cursor1 start : " << CxTime::currentSystemTimeString();
    CxTimerManager::startTimer(timerCursor1, 200);
}



