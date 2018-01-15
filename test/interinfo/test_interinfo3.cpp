
#include <ccxx/ccxx.h>

using namespace std;

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
            cin.peek();
        }
        CxThread::sleep(10);
        ++i;
    }

    cxPromptOut("end test interinfo !!!");
}

int main(int argc,const char * argv[])
{
    cxPromptOut("begin test interinfo !!!");

    CxApplication::registStartFunction(run1);

    CxApplication::init(argc, argv);

    return CxApplication::exec();
}

