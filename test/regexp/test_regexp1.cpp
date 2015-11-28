
#include <ccxx/ccxx.h>

using namespace std;

int f_argc = 0;
char * * f_argv = NULL;

int main(int argc, char * argv[])
{
    f_argc = argc;
    f_argv = argv;

    cout << "begin test xxx : " << endl;

    CxApplication::init(argc, argv);

    cout << "end test xxx !!!" << endl;

    return CxApplication::exec();
}
