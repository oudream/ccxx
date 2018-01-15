
/***
* window message

#include <stdio.h>

#undef UNICODE
#include <Windows.h>

LRESULT CALLBACK MainWndProc ( HWND hwnd , UINT msg , WPARAM wParam, LPARAM lParam )
{
    FILE* fp=fopen("c:\\mainwndproc.txt","a+");
    fprintf(fp,"Received window message %d \n",msg);
    switch(msg)
    {
    case WM_QUERYENDSESSION:
        MessageBox(NULL,"Received WM_QUERYENDSESSION","MainWndProc",MB_OK);
        fprintf(fp,"Received WM_QUERYENDSESSION\n");
        fclose(fp);
        return TRUE;

    case WM_ENDSESSION:
        MessageBox(NULL,"Received WM_ENDSESSION","MainWndProc",MB_OK);
        fprintf(fp,"Received WM_ENDSESSION\n");
        fclose(fp);
    {
        if (wParam)
            MessageBox(hwnd, "ConsoleWaWindow", "WM_ENDSESSION at any TIME!!", MB_OK);
        else
            MessageBox(hwnd, "ConsoleWaWindow", "WM_ENDSESSION aNO!!", MB_OK);

    }
        return TRUE;

    case WM_DESTROY:
    {
        fprintf(fp,"WM_DESTROY received\n");
        PostQuitMessage(0);
    }
        break;

    case WM_CLOSE:
        fprintf(fp,"WM_CLOSE received\n");
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
        break;
    }
    fclose(fp);
    return TRUE;
}

void CreateInvisibleWindow()
{
    HWND hwnd;
    WNDCLASS wc={0};
    wc.lpfnWndProc=(WNDPROC)MainWndProc;
    wc.hInstance=GetModuleHandle(NULL);
    wc.hIcon=LoadIcon(GetModuleHandle(NULL), "TestWClass");
    wc.lpszClassName="TestWClass";
    RegisterClass(&wc);

    hwnd=CreateWindowEx(0,"TestWClass","TestWClass",WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,(HWND) NULL, (HMENU) NULL, GetModuleHandle(NULL), (LPVOID) NULL);
    if(!hwnd)
        printf("FAILED to create window!!!  %d\n",GetLastError());
}

DWORD WINAPI RunInvisibleWindowThread(LPVOID lpParam)
{
    MSG msg;
    CreateInvisibleWindow();
    printf(" Thread and window created ..whoa!!\n");
    while (GetMessage(&msg,(HWND) NULL , 0 , 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

int main(int argc,char* argv)
{

    DWORD tid;
    HANDLE hInvisiblethread=CreateThread(NULL, 0, RunInvisibleWindowThread, NULL, 0, &tid);

    while(TRUE)
    {
        printf(" Iam running ..whoa!!\n");
        Sleep(3000);
    }

    printf("Finished\n");
    CloseHandle(hInvisiblethread);
}

*/
