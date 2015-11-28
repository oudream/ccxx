
#include <ccxx.h>

using namespace std;

int main(int argc, char * argv[])
{
    cout << "begin test xml : ";

    CxApplication::init(argc, argv);

    cout << "end test xml !!!";

    return CxApplication::exec();
}

