#   undef UNICODE
#define _WIN32_WINNT 0x0501
#include <Windows.h>
#include <stdio.h>
#include <malloc.h>
#include <tchar.h>
#include <wchar.h>
#include <stdio.h>
#include <iostream>


#define BUFSIZE MAX_PATH

using namespace std;

int main41(int argc, TCHAR *argv[])
{
    char * fileNames[] = {
        "F:/WinUserOudream/desktop/test/a",
        "F:/WinUserOudream/desktop/test/b",
        "F:/WinUserOudream/desktop/test/c",
        "F:/WinUserOudream/desktop/test/d.lnk",
        "F:/WinUserOudream/desktop/test/a1.txt",
        "F:/WinUserOudream/desktop/test/a2.txt",
        "F:/WinUserOudream/desktop/test/a3.txt",
        "F:/WinUserOudream/desktop/test/a4.txt.lnk",
        "F:/WinUserOudream/desktop/test/a5.txt",
        "F:/WinUserOudream/desktop/test/a6.txt",
        "F:/WinUserOudream/desktop/test/a7.txt",
        "F:/WinUserOudream/desktop/test/a8.txt",
        "F:/WinUserOudream/desktop/test/a9.txt",
        "F:/WinUserOudream/desktop/test/a10.txt"
    };

    for (size_t i = 0; i < sizeof(fileNames) / sizeof(char *); ++i)
    {
        string sFilePath = fileNames[i];
        WIN32_FIND_DATA FindFileData;
        HANDLE hFind=::FindFirstFile(sFilePath.c_str(),&FindFileData);
        if(INVALID_HANDLE_VALUE == hFind) break;
        cout << sFilePath << "         ";
        cout << FindFileData.dwFileAttributes << " - " << FindFileData.dwReserved0  << " - " << FindFileData.dwReserved1 << endl;
        ::FindClose(hFind);
    }

//    return 0;
//    for (size_t i = 0; i < sizeof(fileNames) / sizeof(char *); ++i)
//    {
//        char * fileName = fileNames[i];
//        DWORD attr = GetFileAttributes(fileName);
////        if(attr == (DWORD)~0l)
//        if(attr == INVALID_FILE_ATTRIBUTES)
//            cout << "failse" << endl;
//        cout << fileName << endl;
//        cout << attr << endl;
//        cout << attr << endl;
//    }

}

