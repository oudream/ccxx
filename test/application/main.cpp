#include <cxapplication.h>
#include <cxfile.h>
#include <cxstring.h>
#include <cxapplication.h>
#include <cxthread.h>
#include <cxsocket.h>

using namespace std;

class Conditional
{
public:
    HANDLE events[2];
    CRITICAL_SECTION mlock;

    Conditional()
    {
        InitializeCriticalSection(&mlock);
        events[0] = CreateEvent(NULL, FALSE, FALSE, NULL);
        events[1] = CreateEvent(NULL, TRUE, FALSE, NULL);
    }

    ~Conditional()
    {
        DeleteCriticalSection(&mlock);
        CloseHandle(events[0]);
        CloseHandle(events[1]);
    }

    bool wait(unsigned long ot)
    {
//        ::ResetEvent(events[0]);
        int result = WaitForMultipleObjects(2, events, FALSE, ot);
        if (result == WAIT_OBJECT_0 + 0)
        {
            cout << "WaitForMultipleObjects : WAIT_OBJECT_0 + 0" << endl;
        }
        /*
        else
        {
            cout << "WaitForMultipleObjects : " << result << endl;
        }*/
    }

    void signal1()
    {
        ::SetEvent(events[0]);
    }

    void signal2()
    {
        ::SetEvent(events[1]);
    }
};

CxConditional conditional;

static queue<string> f_msg;

void fn_push(string sMsg)
{
    CxConditional::ScopeLock lock(&conditional);
    f_msg.push(sMsg);
}

string fn_pop()
{
    string r;
    {
        CxConditional::ScopeLock lock(&conditional);
        if (! f_msg.empty())
        {
            r = f_msg.back();
            f_msg.pop();
        }
    }
    return r;
}

class SignalThread : public CxDetachedThread
{
public:
   void run(void)
   {
       int i = 0;
       char sMsg[5];
       while (1)
       {
           sleep(200);
           sprintf(sMsg, "%d", i);
           fn_push(sMsg);
           conditional.signal();
//           conditional.broadcast();
           cout << "broadcast" << endl;
       }
   }
};

int main(int argc, char *argv[])
{
    CxApplication::init(argc, argv);

    SignalThread st;
    st.start();
//    conditional.signal();

    cout << "signal" << endl;

    while (1)
    {
        conditional.wait(60000);
        cout << "wait 60000" << endl;
    }


    return 0;
}
