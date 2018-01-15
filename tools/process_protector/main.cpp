#include <ccxx/cxapplication.h>
#include <ccxx/cxtime.h>

#define GM_SERVER_PORT 5015

#define GM_BUFFER_SZIE 4096

using namespace std;

#undef UNICODE

#include <windows.h>
#include <tchar.h>

using namespace std;

PROCESS_INFORMATION pi; //进程信息：


bool f_bMyThreadFun1 = false;
DWORD WINAPI MyThreadFun1( LPVOID lpParam )
{
    cout << "MyThreadFun1 begin" << endl;
    WaitForSingleObject( pi.hProcess, INFINITE );//检测进程是否停止
    cout << "MyThreadFun1 end" << endl;
    f_bMyThreadFun1 = true;
    return 0;
}

SOCKET            f_sk;                //套接字


bool f_bMyThreadFun2Error = false;
bool f_bMyThreadFun2Cancel = false;
msepoch_t f_tFirstRec = 0;
msepoch_t f_tLastRec = 0;
DWORD WINAPI MyThreadFun2( LPVOID lpParam )
{
    //接收数据
    SOCKADDR_IN clientAddr;
    int nClientLen = sizeof(clientAddr);
    char            buf[GM_BUFFER_SZIE];    //接收数据缓冲区
    ZeroMemory(buf, GM_BUFFER_SZIE);
    while (!f_bMyThreadFun2Cancel)
    {
        int iRecLen = recvfrom(f_sk, buf, GM_BUFFER_SZIE, 0, (SOCKADDR*)&clientAddr, &nClientLen);
        if(iRecLen == SOCKET_ERROR)
        {
            printf("接收数据失败，失败原因: %d\n", WSAGetLastError());
//            closesocket(f_sk);    //关闭套接字
//            WSACleanup();    //释放套接字资源
            f_bMyThreadFun2Error = true;
            return 1;
        }
        else
        {
            if (f_tFirstRec == 0) f_tFirstRec = CxTime::currentMsepoch();
            f_tLastRec = CxTime::currentMsepoch();
            cout << "接收数据长度：" << iRecLen << endl;
        }
    }
    return 0;
}


int main(int argc, const char *argv[])
{
    CxApplication::init(argc, argv);

    STARTUPINFO si;

    //    PROCESS_INFORMATION pi; //进程信息：

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    string sProgressFilePath = CxApplication::applicationPath() + "/ProtocolConvertor.exe";

    cout << "ProgressFilePath : " << sProgressFilePath << endl;

    //    if (  )

    do{
        f_bMyThreadFun1 = false;

        // 创建子进程，判断是否执行成功
        if(!CreateProcessA( NULL, const_cast<char *>(sProgressFilePath.data()),NULL,NULL,FALSE,0,NULL,NULL,&si,&pi))
        {
            cout << "创建进程失败.." << GetLastError() << endl;
            system("pause"); //用于测试
            return 0;
        }
        //进程执行成功，打印进程信息
        cout << "以下是子进程的信息：" << endl;
        cout << "进程ID pi.dwProcessID: " << pi.dwProcessId << endl;
        cout << "线程ID pi.dwThreadID : " << pi.dwThreadId << endl;

        HANDLE hThread1;
        DWORD dwThreadId1;
        hThread1 = CreateThread(
                    NULL,              // default security attributes
                    0,                 // use default stack size
                    MyThreadFun1,          // thread function
                    NULL,             // argument to thread function
                    0,                 // use default creation flags
                    &dwThreadId1);   // returns the thread identifier


        WSADATA            wsd;            //WSADATA变量
        SOCKADDR_IN        servAddr;        //服务器地址

        //初始化套结字动态库
        if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
        {
            cout << "初始化套接字动态库失败!" << endl;
            CloseHandle(hThread1);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            return 0;
        }

        //创建套接字
        f_sk = socket(AF_INET, SOCK_DGRAM, 0);
        if (f_sk == INVALID_SOCKET)
        {
            cout << "创建套接字失败，失败原因：" << WSAGetLastError() << endl;
            WSACleanup();//释放套接字资源
            CloseHandle(hThread1);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            return 0;
        }

        //服务器地址
        servAddr.sin_family = AF_INET;
        servAddr.sin_port = htons((short)GM_SERVER_PORT);            //端口
        servAddr.sin_addr.s_addr = htonl(INADDR_ANY);    //IP

        //绑定
        if (bind(f_sk, (SOCKADDR *)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        {
            cout << "绑定失败，失败原因:" << WSAGetLastError() << endl;
            closesocket(f_sk);    //关闭套接字
            WSACleanup();    //释放套接字资源
            CloseHandle(hThread1);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            return 0;
        }


        //初始化线程函数中用到的全局参数
        f_bMyThreadFun2Error = false;
        f_bMyThreadFun2Cancel = false;
        f_tFirstRec = 0;
        f_tLastRec = 0;
        HANDLE hThread2;
        DWORD dwThreadId2;
        hThread2 = CreateThread(
                    NULL,              // default security attributes
                    0,                 // use default stack size
                    MyThreadFun2,          // thread function
                    NULL,             // argument to thread function
                    0,                 // use default creation flags
                    &dwThreadId2);   // returns the thread identifier

//        //接收数据
//        SOCKADDR_IN clientAddr;
//        int nClientLen = sizeof(clientAddr);
//        char            buf[GM_BUFFER_SZIE];    //接收数据缓冲区
//        ZeroMemory(buf, BUF_SZIE);
//        if(recvfrom(sk, buf, BUF_SZIE, 0, (SOCKADDR*)&clientAddr, &nClientLen) == SOCKET_ERROR)
//        {
//            printf("接收数据失败，失败原因: %d\n", WSAGetLastError());
//            closesocket(sk);    //关闭套接字
//            WSACleanup();    //释放套接字资源
//            return 1;
//        }
//        printf("%s\n", buf);//输出


        while (1)
        {
            if ( f_bMyThreadFun1 )
                break;

            //大于3秒没收到数据重启进程
            if (f_tFirstRec != 0 && (CxTime::currentMsepoch() - f_tLastRec > 3000))
            {
                closesocket(f_sk);   //关闭套接字
                f_bMyThreadFun2Cancel = true;

                TerminateProcess(pi.hProcess, 0);

                WaitForSingleObject( pi.hProcess, INFINITE );//检测线程是否停止
                WaitForSingleObject( hThread2, INFINITE );//检测线程是否停止
                break;
            }

            //socket发生错误重启进程
            if (f_bMyThreadFun2Error)
                break;

            Sleep(10);
        }

        closesocket(f_sk);   //关闭套接字
        WSACleanup();        //释放套接字资源
        CloseHandle(hThread1);
        CloseHandle(hThread2);

        // 等待知道子进程退出...
        //        WaitForSingleObject( pi.hProcess, INFINITE );//检测进程是否停止
        //WaitForSingleObject()函数检查对象的状态，如果是未确定的则等待至超时
        //子进程退出
        cout << "子进程已经退出..." << endl;
        //关闭进程和句柄
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        //system("pause");//执行完毕后等待
    }while(true);//如果进程推出就再次执行方法
    exit(0);
    return 0;
}
