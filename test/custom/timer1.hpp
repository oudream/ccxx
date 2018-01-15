//
// Created by oudream on 2017/5/27.
//

#include <ccxx/ccxx.h>

void fn_timer_timeout1(int iInterval)
{
    cxPrompt() << CxTime::currentSystemTimeString();
}

void stopTimer11()
{
    CxTimerManager::stopTimer(fn_timer_timeout1);
}

void startTimer11()
{
    CxTimerManager::startTimer(fn_timer_timeout1, 3000);
    CxApplication::registStopFunction(stopTimer11);
}
