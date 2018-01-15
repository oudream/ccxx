#include <ccxx/ccxx.h>

using namespace std;

volatile int f_k = 1;

volatile int f_count = 0;

class ThreadTestApplication1 : public CxDetachedThread
{
public:
    static void processThreadNofity(int, int, const void *, int iLength, void *, void *)
    {
        ++f_count;
        if (f_count % 500 == 0)
        {
            int iCount = f_count;
            CxInterinfoOut::outInfo( "processThreadNofity - data length : " + CxString::toString( iLength ) + " f_count : " + CxString::toString(iCount) + "\n" , "ThreadTestApplication1");
        }
    }

    void run()
    {
        sleep(1000);
        while (f_k)
        {
            vector<char> data;
            data.resize(4096);
            CxApplication::pushProcessCallBack(processThreadNofity, 0, 0, & data.front(), data.size(), 0, 0);
            CxApplication::signalMainThread();
            sleep(1);
        }
    }
};

int main(int argc,const char *argv[])
{
    cout << "begin" << endl;

    CxApplication::init(argc, argv);

    ThreadTestApplication1 td;
    td.start();

    int iResult = CxApplication::exec();

    f_k = 0;

    CxThread::sleep(100);

    return iResult;
}
