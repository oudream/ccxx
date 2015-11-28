#include <typeinfo>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <bitset>
#include <stack>
#include <queue>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <iso646.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include <wctype.h>

#include <windows.h>

using namespace std;

#define f_cCacheSize (1024 * 256)

#define GM_TEST


const char f_sourceString[] = "Node-path: 软件事业部";
const char f_targetString[] = "Node-path: ";

const size_t f_sourceSize = sizeof(f_sourceString) - 1;
const size_t f_targetSize = sizeof(f_targetString) - 1;

const char f_sourceFile[] = "e:/bigFile/sourceFile.big";
const char f_targetFile[] = "e:/bigFile/targetFile.big";


string f_sInput = "";

bool f_bInreplacing = false;


void WriteFormatted ( const char * format, ... )
{
  va_list args;
  va_start (args, format);
  vprintf (format, args);
  va_end (args);
}


DWORD WINAPI MyThreadFun1( LPVOID lpParam )
{
    FILE * pFile;
    pFile = fopen ("c:/record.txt", "wb");
    char buffer[256] = {};
    for (size_t i = 0; i < 300; ++i)
    {
        sprintf (buffer, "now : %d \n", clock());
        fwrite (buffer , sizeof(char), strlen(buffer), pFile);
        fflush (pFile);
        ::Sleep(10);
    }
    fclose (pFile);
    return 0;
}

int testFile11(int argc, char *argv[])
{
    cout << "bigFile program begin :" << endl;

    HANDLE hThread1;
    DWORD dwThreadId1;
    hThread1 = CreateThread(
                NULL,              // default security attributes
                0,                 // use default stack size
                MyThreadFun1,          // thread function
                NULL,             // argument to thread function
                0,                 // use default creation flags
                &dwThreadId1
                );   // returns the thread identifier}

    int buffer[300];

    for (size_t i = 0; i < 300; ++i)
    {
        buffer[i] = i;
        cout << clock() << endl;
        ::Sleep(11);
    }

    CloseHandle(hThread1);

    cout << "bigFile program end !" << endl;

    return 0;
}
