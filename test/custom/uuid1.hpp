//
// Created by oudream on 2017/5/27.
//

#include <ccxx/ccxx.h>
#include <ccxx/cxuuid.h>

int testUuid11()
{
    string sGuid = CxUuid::createGuidString();
    cout << sGuid << endl;

    GUID guid = CxUuid::guidFromString(sGuid);

    cout << CxUuid::guidToString(guid) << endl;

    cout << "end..." << endl;

	return TRUE;
}