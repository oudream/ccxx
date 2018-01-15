#include "test_vdi_lib.h"

#include "test_modbus_lib.h"

using namespace std;

typedef int (VXD_API_CALL * fn_vxd_init_t) (int argc, const char * argv[]);

int TestVdi::init(int argc, const char **argv)
{
    cxPrompt() << "i am vdi";
    const string sModbusLibName = "libtest_dll_modbus.dll";
    CxDll dllLib(sModbusLibName.c_str());
    if(dllLib.hadLoad())
    {
        fn_vxd_init_t fn_vxd_init = (fn_vxd_init_t)dllLib.find("vxd_modbus_init");
        if (fn_vxd_init)
        {
            fn_vxd_init(argc, argv);
        }
        else
        {
            cxPrompt() << "lib: " << sModbusLibName << ", load fail!";
        }
    }

    return 0;
}

int TestVdi::start()
{
    return 0;
}

void TestVdi::stop()
{

}
