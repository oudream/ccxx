#include <typeinfo>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <bitset>
#include <stack>
#include <queue>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <iso646.h>
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


#   undef UNICODE

#include <windows.h>

using namespace std;

int main()
{
    char data[100] = {
        1,2,3,4,5,6,7,8,9,1,
        1,2,3,4,5,6,7,8,9,1,
        1,2,3,4,5,6,7,8,9,1,
        1,2,3,4,5,6,7,8,9,1,
        1,2,3,4,5,6,7,8,9,1,

        1,2,3,4,5,6,7,8,9,1,
        1,2,3,4,5,6,7,8,9,1,
        1,2,3,4,5,6,7,8,9,1,
        1,2,3,4,5,6,7,8,9,1,
        1,2,3,4,5,6,7,8,9,1
    };

//    FILE *fp;
//    char buf[100];
//    if((fp=fopen("COM2","wr"))==NULL) puts("Can't open com3 \n");
//    fwrite (data , 1, sizeof(data), fp);
//    while(1)
//    {
//        if(fgets(buf,100,fp)!=0)
//            puts(buf);
//    }
//    fclose(fp);
//    return 0;



    string _portName = "COM1";

    HANDLE     m_descriptor = CreateFile(_portName.c_str(),
                     GENERIC_READ | GENERIC_WRITE,
                     0,                    // exclusive access
                     NULL,                 // no security attrs
                     OPEN_EXISTING,
                     FILE_FLAG_OVERLAPPED,
                     NULL);

    DCB m_currentDcb;
    DCB m_restoredDcb;

    DWORD confSize = sizeof(DCB);
    if (::GetCommState(m_descriptor, &m_restoredDcb) == 0) {
        cout << "GetCommState" << endl;
    }
//    ::memcpy(&m_currentDcb, &m_restoredDcb, confSize);

    memset(&m_currentDcb, 0, sizeof(m_currentDcb));
    m_currentDcb.BaudRate = 9600;
    m_currentDcb.ByteSize = 8;
//    m_currentDcb.XoffLim = 512;
//    m_currentDcb.XonLim = 2048;
//    m_currentDcb.fBinary = 1;
//    m_currentDcb.fTXContinueOnXoff = 1;

    if (::SetCommState(m_descriptor, &m_currentDcb) == 0) {
        cout << "GetCommState" << endl;
        return false;
    }

    COMMTIMEOUTS m_currentCommTimeouts;
    COMMTIMEOUTS m_restoredCommTimeouts;

    confSize = sizeof(COMMTIMEOUTS);
    if (::GetCommTimeouts(m_descriptor, &m_restoredCommTimeouts) == 0) {
        cout << "GetCommTimeouts Fail" << endl;
        return false;
    }
    ::memcpy(&m_currentCommTimeouts, &m_restoredCommTimeouts, confSize);

    // Set new port timeouts.
    ::memset(&m_currentCommTimeouts, 0, confSize);
    m_currentCommTimeouts.ReadIntervalTimeout = MAXDWORD;

//    if (::SetCommTimeouts(m_descriptor, &m_currentCommTimeouts) == 0) {
//        cout << "SetCommTimeouts Fail" << endl;
//        return false;
//    }

//    const DWORD flags = PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR;
//    ::PurgeComm(m_descriptor, flags);

    DWORD writeBytes = 0;

    OVERLAPPED m_writeOverlapped;
    ::memset(&m_writeOverlapped, 0, sizeof(OVERLAPPED));

    OVERLAPPED m_notifyOverlapped;
    ::memset(&m_notifyOverlapped, 0, sizeof(OVERLAPPED));
    m_notifyOverlapped.hEvent = ::CreateEvent(0, false, false, 0);

    DWORD m_currentMask = 0;

    COMSTAT cs;
    unsigned long dwError = 0;
    while (1)
    {
//        ::SetCommMask(m_descriptor, 133);
//        ::WaitCommEvent(m_descriptor, &m_currentMask, &m_notifyOverlapped);
//        ::SetCommMask(m_descriptor, 129);
//        ::WaitCommEvent(m_descriptor, &m_currentMask, &m_notifyOverlapped);
        if ( ::WriteFile(m_descriptor, data, sizeof(data), &writeBytes, &m_writeOverlapped) == FALSE )
        {
            cout << "WriteFile false 1" << endl;
            if(GetLastError() == ERROR_IO_PENDING)
            {
                WaitForSingleObject(m_writeOverlapped.hEvent, INFINITE);
                GetOverlappedResult(m_descriptor, &m_writeOverlapped, &writeBytes, TRUE);
            }
            else
            {
                ClearCommError(m_descriptor, &dwError, &cs);
            }
            cout << "WriteFile false 2" << endl;
        }
        else
        {
            if (writeBytes > 0)
//            ::FlushFileBuffers(m_descriptor);
            cout << "WriteFile true" << endl;
        }

        ::Sleep(1000);
    }

    ::CloseHandle(m_writeOverlapped.hEvent);

    ::CloseHandle(m_descriptor);

    cout << "Hello World!" << endl;
    return 0;
}


