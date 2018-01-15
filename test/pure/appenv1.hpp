//
// Created by oudream on 2017/5/27.
//

#include <ccxx/ccxx.h>

int testAppEnv11(int argc, const char * argv[])
{
    map<string, string> sConfigs;
    sConfigs[string("LocalIpAddress")] = "127.0.0.1";
    sConfigs[string("LocalPort")] = "5555";
    sConfigs[string("RemoteIpAddress")] = "127.0.0.1";
    sConfigs[string("RemotePort")] = "5556";
    sConfigs[string("PsmSentSourceId")] = "0x00000901";
    CxAppEnv::CommandLineArguments cls(argc, argv);
    cls.push("GeneralAccess", CxString::join(sConfigs, CxGlobal::middleChar, CxGlobal::splitChar));
    for (int i = 0; i < cls.argc; ++i)
    {
        cout << string(cls.argv[i]) << endl;
    }
	return TRUE;
}