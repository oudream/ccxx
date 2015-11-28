#include <windows.h>
#include <stdio.h>

#define THREADCOUNT 4

HANDLE ghGlobalWriteEvent;
HANDLE ghReadEvents[THREADCOUNT];

DWORD WINAPI ThreadProc(LPVOID);

void CreateEventsAndThreads(void)
{
    HANDLE hThread;
    DWORD i, dwThreadID;

    // Create a manual-reset event object. The master thread sets
    // this to nonsignaled when it writes to the shared buffer.

    ghGlobalWriteEvent = CreateEvent(
                NULL,               // default security attributes
                TRUE,               // manual-reset event
                TRUE,               // initial state is signaled
                TEXT("WriteEvent")  // object name
                );

    if (ghGlobalWriteEvent == NULL)
    {
        printf("CreateEvent failed (%d)\n", GetLastError());
        return;
    }
    else if ( GetLastError() == ERROR_ALREADY_EXISTS )
    {
        printf("Named event already exists.\n");
        return;
    }

    // Create multiple threads and an auto-reset event object
    // for each thread. Each thread sets its event object to
    // signaled when it is not reading from the shared buffer.

    for(i = 0; i < THREADCOUNT; i++)
    {
        // Create the auto-reset event
        ghReadEvents[i] = CreateEvent(
                    NULL,     // no security attributes
                    FALSE,    // auto-reset event
                    TRUE,     // initial state is signaled
                    NULL);    // object not named

        if (ghReadEvents[i] == NULL)
        {
            printf("CreateEvent failed (%d)\n", GetLastError());
            return;
        }

        hThread = CreateThread(NULL,
                               0,
                               ThreadProc,
                               &ghReadEvents[i],  // pass event handle
                               0,
                               &dwThreadID);

        if (hThread == NULL)
        {
            printf("CreateThread failed (%d)\n", GetLastError());
            return;
        }
    }
}

void WriteToBuffer(VOID)
{
    DWORD dwWaitResult, i;

    // Reset ghGlobalWriteEvent to nonsignaled, to block readers

    if (! ResetEvent(ghGlobalWriteEvent) )
    {
        printf("ResetEvent failed (%d)\n", GetLastError());
        return;
    }

    // Wait for all reading threads to finish reading

    dwWaitResult = WaitForMultipleObjects(
                THREADCOUNT,   // number of handles in array
                ghReadEvents,  // array of read-event handles
                TRUE,          // wait until all are signaled
                INFINITE);     // indefinite wait

    switch (dwWaitResult)
    {
    // All read-event objects were signaled
    case WAIT_OBJECT_0:
        // TODO: Write to the shared buffer
        printf("Main thread writing to the shared buffer...\n");
        break;

        // An error occurred
    default:
        printf("Wait error: %d\n", GetLastError());
        ExitProcess(0);
    }

    // Set ghGlobalWriteEvent to signaled

    if (! SetEvent(ghGlobalWriteEvent) )
    {
        printf("SetEvent failed (%d)\n", GetLastError());
        ExitProcess(0);
    }

    // Set all read events to signaled
    for(i = 0; i < THREADCOUNT; i++)
        if (! SetEvent(ghReadEvents[i]) )
        {
            printf("SetEvent failed (%d)\n", GetLastError());
            return;
        }
        else
        {
            printf("SetEvent success (%d)\n", i);
        }
}

void CloseEvents()
{
    int i;

    for( i=0; i < THREADCOUNT; i++ )
        CloseHandle(ghReadEvents[i]);

    CloseHandle(ghGlobalWriteEvent);
}

int main()
{
    int i;

    // TODO: Create the shared buffer

    // Create the events and THREADCOUNT threads to read from the buffer

    CreateEventsAndThreads();

    // Write to the buffer three times, just for test purposes

    for(i=0; i < 3; i++)
        WriteToBuffer();

    // Close the events

    CloseEvents();
}

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
    DWORD dwWaitResult;
    HANDLE hEvents[2];

    hEvents[0] = *(HANDLE*)lpParam;  // thread's read event
    hEvents[1] = ghGlobalWriteEvent; // global write event

    dwWaitResult = WaitForMultipleObjects(
                2,            // number of handles in array
                hEvents,      // array of event handles
                TRUE,         // wait till all are signaled
                INFINITE);    // indefinite wait

    switch (dwWaitResult)
    {
    // Both event objects were signaled
    case WAIT_OBJECT_0:
        // TODO: Read from the shared buffer
        printf("Thread %d reading from buffer...\n",
               GetCurrentThreadId());
        break;

        // An error occurred
    default:
        printf("Wait error: %d\n", GetLastError());
        ExitThread(0);
    }

    // Set the read event to signaled

    if (! SetEvent(hEvents[0]) )
    {
        printf("SetEvent failed (%d)\n", GetLastError());
        ExitThread(0);
    }

    return 1;
}

