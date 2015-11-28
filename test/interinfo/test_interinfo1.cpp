
#include <ccxx.h>

using namespace std;

int main(int argc, char * argv[])
{
    cout << "begin test interinfo : " << endl;

    CxApplication::init(argc, argv);

    cxWarning() << 111111 << " - " << 222222 << cxEndLine;

    cout << "end test interinfo !!!" << endl;

    return CxApplication::exec();
}
