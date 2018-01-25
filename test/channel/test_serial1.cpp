
//stl
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

//clib
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

#include <windows.h>

using namespace std;

static long long f_iSerialReceivedTotal = 0;

void HandleASuccessfulRead(char *lpBuf, DWORD dwRead)
{
    f_iSerialReceivedTotal += dwRead;
    cout << "seral read: " << f_iSerialReceivedTotal << " bytes." << endl;
}

int main(int argc, char *argv[])
{
    cout << "test seral begin:" << endl;

    HANDLE dev;

    string sDev = "\\\\.\\COM1";
    dev = CreateFile(sDev.c_str(),
                     GENERIC_READ | GENERIC_WRITE,
                     0,                    // exclusive access
                     NULL,                 // no security attrs
                     OPEN_EXISTING,
                     FILE_FLAG_OVERLAPPED,
                     NULL);

    DCB current;
    DCB *attr = &current;

    GetCommState(dev, attr);

//    attr->DCBlength = sizeof(DCB);
//    attr->BaudRate = 56000;
//    attr->ByteSize = 8;
//    attr->XoffLim = 512;
//    attr->XonLim = 2048;
//    attr->fBinary = 1;
//    attr->fTXContinueOnXoff = 1;


    attr->DCBlength = sizeof(DCB);
    attr->BaudRate = 56000;
//    attr->ByteSize = 8;
//    attr->Parity = NOPARITY;
//    attr->StopBits = ONESTOPBIT;

//    attr->fBinary = TRUE;
//    attr->fParity = FALSE;
//    attr->fDsrSensitivity = FALSE;
//    attr->fNull = FALSE;
//    attr->fAbortOnError = FALSE;
//    attr->fOutxCtsFlow = false; // Disable CTS monitoring
//    attr->fOutxDsrFlow = false; // Disable DSR monitoring
//    attr->fDtrControl = DTR_CONTROL_ENABLE; // Enable DTR control (for self-powered interfaces)
//    attr->fOutX = false; // Disable XON/XOFF for transmission
//    attr->fInX = false; // Disable XON/XOFF for receiving
//    attr->fRtsControl = RTS_CONTROL_ENABLE; // Enable RTS to go high
//    attr->fParity = (attr->Parity != NOPARITY);
    
    
    
    
    
//    SetCommState(dev, attr);
//
//    COMMTIMEOUTS CommTimeouts;
//    CommTimeouts.ReadIntervalTimeout = MAXDWORD;
//    CommTimeouts.ReadTotalTimeoutConstant = 0;
//    CommTimeouts.ReadTotalTimeoutMultiplier = 0;
//    CommTimeouts.WriteTotalTimeoutConstant = 0;
//    CommTimeouts.WriteTotalTimeoutMultiplier = 0;
//    ::SetCommTimeouts(dev, &CommTimeouts);

    COMMTIMEOUTS timeouts;
//    timeouts.ReadIntervalTimeout = 20;
//    timeouts.ReadTotalTimeoutMultiplier = 10;
//    timeouts.ReadTotalTimeoutConstant = 100;
//    timeouts.WriteTotalTimeoutMultiplier = 10;
//    timeouts.WriteTotalTimeoutConstant = 100;
    timeouts.ReadIntervalTimeout = 100;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.WriteTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 0;
    if (!SetCommTimeouts(dev, &timeouts)){}
        // Error setting time-outs.

    DWORD dwRead;
    BOOL fWaitingOnRead = FALSE;
    OVERLAPPED osReader = {0};

#define READ_BUF_SIZE 1024 * 4
    char lpBuf[READ_BUF_SIZE];

// Create the overlapped event. Must be closed before exiting
// to avoid a handle leak.
    osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    if (osReader.hEvent == NULL)
    {
        // Error creating overlapped event; abort.
        return 0;
    }

    if (!fWaitingOnRead)
    {
        // Issue read operation.
        while (1)
        {
            if (!ReadFile(dev, lpBuf, READ_BUF_SIZE, &dwRead, &osReader))
            {
                if (GetLastError() != ERROR_IO_PENDING)
                {}     // read not delayed?
                    // Error in communications; report it.
                else
                    fWaitingOnRead = TRUE;
            }
            else
            {
                if (dwRead>0)
                // read completed immediately
                HandleASuccessfulRead(lpBuf, dwRead);
            }

#define READ_TIMEOUT      2000      // milliseconds

            DWORD dwRes;

            if (fWaitingOnRead) {
                dwRes = WaitForSingleObject(osReader.hEvent, READ_TIMEOUT);
                switch(dwRes)
                {
                    // Read completed.
                    case WAIT_OBJECT_0:
                        if (!GetOverlappedResult(dev, &osReader, &dwRead, FALSE)){}
                            // Error in communications; report it.
                            else
                        // Read completed successfully.
                        HandleASuccessfulRead(lpBuf, dwRead);

                        //  Reset flag so that another opertion can be issued.
                        fWaitingOnRead = FALSE;
                        break;

                    case WAIT_TIMEOUT:
                        // Operation isn't complete yet. fWaitingOnRead flag isn't
                        // changed since I'll loop back around, and I don't want
                        // to issue another read until the first one finishes.
                        //
                        // This is a good time to do some background work.
                        break;

                    default:
                        // Error in the WaitForSingleObject; abort.
                        // This indicates a problem with the OVERLAPPED structure's
                        // event handle.
                        break;
                }
            }
        }
    }

    cout << "end." << endl;

    CloseHandle(dev);

    return TRUE;
}
