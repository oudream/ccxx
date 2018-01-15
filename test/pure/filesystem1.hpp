//
// Created by oudream on 2017/5/27.
//

#include <ccxx/ccxx.h>

using namespace std;

int testFileSystem1()
{
    string sFilePath = "F:/temp/ee";
    string sFilePath2 = "F:/temp/ee/a";
    string sFilePath3 = "F:/temp/ee/b";

    cxPrompt() << sFilePath;

    CxFileSystem::createDir(sFilePath);
    CxFileSystem::createDir(sFilePath2);
    CxFileSystem::createDir(sFilePath3);
    CxFileSystem::removeDir(sFilePath);

//    ucommon::dir d("F:\\Tools\\Designer\\mingw\\include");
    char buffer[4096];
//    int iR = d.read(buffer, 4096);
    printf("%s", buffer);
    return 0;
}