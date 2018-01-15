
#include <ccxx/ccxx.h>

#include <ccxx/cxuuid.h>

using namespace std;

int f_argc = 0;
const char * * f_argv = NULL;

int main(int argc, const char * argv[])
{
    f_argc = argc;
    f_argv = argv;

    cout << "begin test xxx : " << endl;

    CxApplication::init(argc, argv);

    cout << "end test xxx !!!" << endl;

    for (size_t i = 0; i < 100; ++i)
    {
        cout << CxUuid::createGuidString() << endl;
    }

    for (size_t i = 0; i < 100; ++i)
    {
        cout << CxUuid::createRandString() << endl;
    }

    return 0;

    int iResult = CxApplication::exec();

    return iResult;
}
