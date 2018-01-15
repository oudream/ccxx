//
// Created by oudream on 2017/5/27.
//

#include <ccxx/ccxx.h>

int testSkerJson11()
{
    cxPrompt() << __FILE__;
    string sConfigFilePath = CxFileSystem::mergeFilePath(CxFileSystem::mergeFilePath(CxFileSystem::parentPath(__FILE__), "resource"), "SvrGlobal.json");
    CxSkverJson tJson(sConfigFilePath);
    tJson.setValue("self", "ServerId", 999);
    tJson.save();
	return TRUE;
}