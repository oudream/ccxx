#include "test_modbus_lib.h"

#include <ccxx/ccxx.h>

using namespace std;

int VXD_API_CALL vxd_modbus_init(int argc,const char **argv)
{
    cxPrompt() << "i am modbus : " << CxAppEnv::applicationTargetName();
    return 0;
}
