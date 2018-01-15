
#include <ccxx/ccxx.h>

using namespace std;

static long long int f_iLogSize1 = 0;
static long long int f_iLogSize2 = 0;

class ThreadTest1 : public CxJoinableThread {
public:
    inline void waitExit()
    {
        stateStatus = false;
        join();
    }

    bool stateStatus;

protected:
    void run()
    {
        while (stateStatus)
        {
            for (int i = 0; i < 1000; ++i)
            {
                vector<string> ss(i);
                string s = CxTime::currentSystemTimeString();
                for (int j = 0; j < i; ++j)
                {
                    ss[j] = s;
                }
                string sLog = CxString::join(ss, ' ');
                f_iLogSize1 += sLog.size();
                cxLog() << sLog;
            }
            CxThread::sleep(1000* 10);
        }
    }

private:

};
ThreadTest1 f_threadTest1;

class ThreadTest2 : public CxJoinableThread {
public:
    inline void waitExit()
    {
        stateStatus = false;
        join();
    }

    bool stateStatus;

protected:
    void run()
    {
        while (stateStatus)
        {
            for (int i = 0; i < 1000; ++i)
            {
				//msepoch_t dtNow = CxTime::currentSystemTime();
				vector<string> ss(1000);
                string s = CxTime::currentSystemTimeString();
                for (int j = 0; j < 1000; ++j)
                {
                    ss[j] = s;
                }
				//cxLog() << CxTime::milliSecondDifferToNow(dtNow);
				string sLog = CxString::join(ss, ' ');
                //cxLog() << CxTime::milliSecondDifferToNow(dtNow);
                f_iLogSize2 += sLog.size();
				//cxLog() << CxTime::milliSecondDifferToNow(dtNow);
                cxLog() << sLog;
				//cxLog() << CxTime::milliSecondDifferToNow(dtNow);
			}
            CxThread::sleep(1000 * 10);
        }
    }

private:

};
ThreadTest2 f_threadTest2;

void fn_timer_timeout1(int iInterval)
{
    cout << CxTime::currentSystemTimeString() << endl;
    cout << "testCxLog1 total size= " << f_iLogSize1 / 1024 << " KB" << endl;
    cout << "testCxLog2 total size= " << f_iLogSize2 / 1024 << " KB" << endl;
}

int main(int argc, const char * argv[])
{
    cout << "test cxLog Begin: " << endl;

    CxApplication::init(argc, argv);

    CxTimerManager::startTimer(fn_timer_timeout1, 2000);

    f_threadTest1.stateStatus = true;
    f_threadTest1.start();

    f_threadTest2.stateStatus = true;
    f_threadTest2.start();

    CxApplication::exec();

    f_threadTest1.waitExit();
    f_threadTest2.waitExit();

    cout << "test cxLog end." << endl;

    return 0;
}
