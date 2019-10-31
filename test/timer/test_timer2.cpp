#include <ccxx/ccxx.h>

int iCount = 0;

class TimerPrintFunction
{
public:
    static void timer_timeOut(int iInterval)
    {
        if (iInterval == 1000)
        {
            cout << "TimerPrintFunction" << iInterval << endl;
        }
        else if (iInterval == 3000)
        {
            cout << "TimerPrintFunction" << iInterval << endl;
        }
    }

    static void start()
    {
        CxTimerManager::startTimer(timer_timeOut, s_interval_1);
        CxTimerManager::startTimer(timer_timeOut, s_interval_3);
    }

    static void stop()
    {
        CxTimerManager::stopTimer(timer_timeOut, s_interval_1);
        CxTimerManager::stopTimer(timer_timeOut, s_interval_3);
    }

private:
    static int s_interval_1;
    static int s_interval_3;

};

int TimerPrintFunction::s_interval_1 = 3000;
int TimerPrintFunction::s_interval_3 = 3000;
