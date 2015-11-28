
#include <ccxx.h>

using namespace std;

void do_interinfo_in_line( const std::string& sInfo, const msepoch_t & dtIn, int iSource = 0, int eInType = 0, int iTag = 0 )
{
    cxPromptOut(sInfo);
}

void run1()
{
    int i = 0;
    while (i < 1000)
    {
//        if (i % 100 == 0)
//        {
//            cxPrompt() << i;
//        }
        if (i % 100 == 0)
        {
            cin.peek()
        }
        CxThread::sleep(10);
        ++i;
    }

    cxPromptOut("end test interinfo !!!");
}

int main(int argc, char * argv[])
{
    CxConsoleInterinfo::addObserver(do_interinfo_in_line);

//    cout << "begin test interinfo : " << endl;

    cxPromptOut("begin test interinfo !!!");

    CxApplication::registStartFunction(run1);

    CxApplication::init(argc, argv);

    return CxApplication::exec();
}

