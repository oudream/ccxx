
#include <ccxx/ccxx.h>

using namespace std;


class FastcgiDealThread : public CxJoinableThread
{
public:
    FastcgiDealThread()
    {
        _isStarted = false;
    }
    ~FastcgiDealThread()
    {
    }

    inline void stop() { _isStarted = false; join(); }

protected:
    void run()
    {
        _isStarted = true;
        while (_isStarted)
        {
            cxPrompt() << "this thread say: ";
            cxPrompt() << CxTime::currentMsepochString();
            CxThread::sleep(1000);
        }
    }

public:
    volatile bool _isStarted;


};

int main(int argc, const char * argv[])
{
    CxApplication::init(argc, argv);

    FastcgiDealThread mThread;
    int i = 0;
    while (1)
    {
        ::Sleep(1000);
        i++;
        if (i==11)
        {
            cxPrompt() << "break!!!";
            break;
        }
        if (i==1)
        {
            mThread.start();
        }
        if (i==10)
        {
            mThread.stop();
            std::cout << "mThread.exit!!!";
            cxPrompt() << "mThread.exit!!!";
        }
    }

    return 0;
}

