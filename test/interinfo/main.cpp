
#include <ccxx.h>

using namespace std;

int main(int argc, char * argv[])
{
    cout << "begin test interinfo : ";

    CxApplication::init(argc, argv);

    cout << "end test interinfo !!!";

    return CxApplication::exec();
}

