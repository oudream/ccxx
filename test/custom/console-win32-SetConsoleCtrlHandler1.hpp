#include <windows.h>
#include <stdio.h>

#include <iostream>

int running;

BOOL WINAPI consoleHandler(DWORD signal) {

    if (signal == CTRL_C_EVENT)
        printf("Ctrl-C handled\n"); // do cleanup

    return TRUE;
}

int main()
{
    running = TRUE;
    if (!SetConsoleCtrlHandler(consoleHandler, TRUE)) {
        printf("\nERROR: Could not set control handler");
        return 1;
    }

    int iCount = 0;
    while (1) {
        /* do work */

        std::cout << ++ iCount << std::endl;

        Sleep(100);
    }


    return 0;
}
