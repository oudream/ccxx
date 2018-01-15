#include <ccxx/ccxx.h>

#include "test_vdi_lib.h"

using namespace std;


int main(int argc, const char *argv[])
{
    CxApplication::init(argc, argv);

    TestVdi::init(argc, argv);

    return CxApplication::exec();
}
