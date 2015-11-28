#include <ccxx/ccxx.h>

using namespace std;

class TimerPrintObject : public ICxTimerNotify
{
public:
    TimerPrintObject()
    {
        timer1.init(this, 10);
        timer1.start();
        timer5.init(this, 20);
        timer5.start();
    }

protected:
    void timer_timeOut(const CxTimer* oTimer)
    {
        if (oTimer == &timer1)
        {
            cout << "TimerPrintObject" << timer1.interval() << endl;
            timer5.start();
            timer1.stop();
        }
        else if (oTimer == &timer5)
        {
            cout << "TimerPrintObject" << timer5.interval() << endl;
            timer5.stop();
            timer1.start();
        }

        static int iTimes = 2;
        try
        {
//            int i = 1 / --iTimes;
//            cout << i << endl;
//            iResult = CxApplication::exec();
        }
        catch (...)
        {
            cout << "CxApplication::exec() except" << endl;
        }
    }

private:
    CxTimer timer1;
    CxTimer timer5;

};

