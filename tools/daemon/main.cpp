/* ---------------------------------------------------------------------------
*file:      GCDaemon
*ingroup:   tools
*author:    oudream , luliangbin
*date:      2013/10/29
*brief:     window only
*           GCDaemon.exe -n 5561;5562 -t 6000 -p a.exe;b.exe
*           UDP Port  5561  TimeOut 6000 ms
*modify:    2013/10/29 create
---------------------------------------------------------------------------*/

#include "common.cpp"

#define GM_BUFFER_SZIE 4096

unsigned int f_iTimeOut_receive = 3000;

void fn_outInfo(const string& sInfo)
{
    string sMsg = sInfo;
    sMsg.append("\n\n");
    ::cout << sMsg;
}

typedef struct {
    char code[10];
    char date[4];
} SampleInfo;

class Processer
{
public:
    Processer(string sProcessFilePath, int iWaitMillisecond, int iProcessType, int iLocalPort, string sHeartBuffer, int iTimeOut_receive, string sProcessParam,int iMode)
    {
        exeFilePath = sProcessFilePath;
        exePath = fn_extractFilePath(sProcessFilePath);
        exeFileName = fn_extractFileName(sProcessFilePath);
        processType = iProcessType;
        waitFirstMillisecond = iWaitMillisecond;

        localPort = iLocalPort;
        heartBuffer = sHeartBuffer;
        heartHexString = fn_toHexstring(sHeartBuffer.data(), sHeartBuffer.size(), false);
        timeOut_receive = iTimeOut_receive;

        isThreadWaitFinish  = false;
        startTimes = 0;
        hasFirstReceive = false;
        isThreadReceiveError = false;
        isThreadReceiveCancel = false;
        lastReceiveTime = 0;
        notReceiveLong = 0;
        startMode = iMode;
        runParam = sProcessParam;

        commandLine = sProcessFilePath;
        if (sProcessParam.size() > 0)
        {
            commandLine.push_back(' ');
            commandLine.append(sProcessParam);
        }

        memset(&pi, 0, sizeof(pi));
    }

    ~Processer()
    {
        releaseProcess();
    }

    void resetReceiveData()
    {
        //初始化线程函数中用到的全局参数
        hasFirstReceive = false;
        isThreadReceiveError = false;
        isThreadReceiveCancel = false;
        lastReceiveTime = 0;
        notReceiveLong = 0;
    }

    void outInfo(const string& sInfo)
    {
        fn_outInfo( exeFileName + " : " + sInfo );
    }

    static DWORD WINAPI fn_threadWaitProcess( LPVOID lpParam )
    {
        Processer * oProcesser = (Processer * ) lpParam;
        fn_outInfo( oProcesser->exeFileName + " : WaitProcess begin" );
        WaitForSingleObject( oProcesser->pi.hProcess, INFINITE );//检测进程是否停止
        fn_outInfo( oProcesser->exeFileName + fn_format( " : WaitProcess end : %d" , oProcesser->pi.dwProcessId ) );
        oProcesser->isThreadWaitFinish = true;
        return 0;
    }

    //接收
    static DWORD WINAPI fn_threadReceive( LPVOID lpParam )
    {
        Processer * oProcesser = (Processer * ) lpParam;
        //接收数据
        SOCKADDR_IN clientAddr;
        int nClientLen = sizeof(clientAddr);
        char            buf[GM_BUFFER_SZIE];    //接收数据缓冲区
        ZeroMemory(buf, GM_BUFFER_SZIE);

        int nTimeRpt = oProcesser->timeOut_receive * 3;
        msepoch_t dtLastReceive = fn_currentMsepoch();
        int iReceiveTotal = 0;
        int iReceiveSize = 0;
        string sReceive;

        while (!oProcesser->isThreadReceiveCancel)
        {
            iReceiveSize = ::recvfrom(oProcesser->socketId, buf, GM_BUFFER_SZIE, 0, (SOCKADDR*)&clientAddr, &nClientLen);
            if(iReceiveSize == SOCKET_ERROR)
            {
                fn_outInfo( oProcesser->exeFileName + " : " + fn_format("接收数据失败，失败原因: %d\n", WSAGetLastError() ));
                oProcesser->isThreadReceiveError = true;
                return 1;
            }
            else if (iReceiveSize > 0)
            {
                sReceive.append(string(buf, iReceiveSize));
                if (sReceive.size() >= oProcesser->heartBuffer.size())
                {
                    size_t found = sReceive.rfind(oProcesser->heartBuffer);
                    if (found != string::npos)
                    {
                        oProcesser->lastReceiveTime = fn_currentMsepoch();
                        oProcesser->notReceiveLong = 0;
                        oProcesser->hasFirstReceive = true;
                        iReceiveTotal += iReceiveSize;
                        if((oProcesser->lastReceiveTime - dtLastReceive) > nTimeRpt)
                        {
                            string sLastReceiveTime = fn_toString(oProcesser->lastReceiveTime);
                            string sMsg = oProcesser->exeFileName + " : " + fn_format( "LastReceiveTime : %s ; TimeSpan : %lld ; ReceiveTotal : %d" , sLastReceiveTime.c_str() , (oProcesser->lastReceiveTime-dtLastReceive)/1000 , iReceiveTotal);
                            fn_outInfo( sMsg );
                            iReceiveTotal = 0;
                            dtLastReceive = oProcesser->lastReceiveTime;
                        }

                        sReceive = sReceive.substr(found + oProcesser->heartBuffer.size());
                    }
                    else
                    {
                        sReceive.resize(oProcesser->heartBuffer.size()-1);
                    }
                }
            }
        }
        return 0;
    }

    bool createSocket()
    {
        resetReceiveData();
        //创建套接字
        socketId = ::socket(AF_INET, SOCK_DGRAM, 0);
        if (socketId == INVALID_SOCKET)
        {
            outInfo( fn_format( "创建套接字失败，失败原因 ： %d" , ::WSAGetLastError() ) );
            return false;
        }

        SOCKADDR_IN        servAddr;        //服务器地址
        //服务器地址
        servAddr.sin_family = AF_INET;
        servAddr.sin_port = ::htons((short)localPort);            //端口
        servAddr.sin_addr.s_addr = htonl(INADDR_ANY);    //IP

        //绑定
        if (bind(socketId, (SOCKADDR *)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        {
            outInfo( fn_format( "绑定失败，失败原因 : %d " , ::WSAGetLastError() ) );
            releaseSocket();
            return false;
        }

        DWORD dwThreadId2;
        socketThread = ::CreateThread(
                    NULL,              // default security attributes
                    0,                 // use default stack size
                    fn_threadReceive,          // thread function
                    this,             // argument to thread function
                    0,                 // use default creation flags
                    &dwThreadId2);   // returns the thread identifier
        return socketThread != NULL;
    }

    void releaseSocket()
    {
        if (socketThread)
        {
            ::CloseHandle(socketThread);
            socketThread = NULL;
        }
        if (socketId)
        {
            ::closesocket(socketId);
            socketId = 0;
        }
    }

    void restartSocket()
    {
        releaseSocket();
        createSocket();
    }

    bool createProcess()
    {
        DWORD dwCreationFlags = 0;
//        dwCreationFlags |= CREATE_NEW_CONSOLE;
        if(startMode==0)dwCreationFlags |= CREATE_NO_WINDOW;
        else if(startMode==1)dwCreationFlags |= CREATE_NEW_CONSOLE;

        STARTUPINFO si;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));
//        fn_killProcess(const_cast<char *>(exeFileName.data()));
        // 创建子进程，判断是否执行成功
        if(! ::CreateProcessA( NULL, const_cast<char *>(commandLine.data()),NULL,NULL,FALSE,dwCreationFlags,NULL,exePath.c_str(),&si,&pi) )
        {
            outInfo( fn_format( "创建进程失败 : %d" , ::GetLastError() ) );
//            system("pause"); //用于测试
            return 0;
        }

        startTimes++;
        //进程执行成功，打印进程信息
        string sMsg = fn_format("以下是子进程的信息 :\n进程ID pi.dwProcessID : %d\n线程ID pi.dwThreadID : %d\n启动次数 startTimes : %d" , pi.dwProcessId , pi.dwThreadId, startTimes);
        outInfo(sMsg);
        DWORD dwThreadId;
        isThreadWaitFinish = false;
        waitProcessThread = ::CreateThread(
                    NULL,              // default security attributes
                    0,                 // use default stack size
                    fn_threadWaitProcess,          // thread function
                    this,             // argument to thread function
                    0,                 // use default creation flags
                    &dwThreadId);   // returns the thread identifier
        bool bSuccess = waitProcessThread != NULL;
        if (bSuccess) startTime = fn_currentMsepoch();
        return bSuccess;
    }

    void releaseProcess()
    {
        if (0 == pi.hProcess)
            return;
        ::TerminateProcess(pi.hProcess, 0);
        ::WaitForSingleObject( pi.hProcess, INFINITE );//检测线程是否停止
        ::CloseHandle(waitProcessThread);
        ::CloseHandle(pi.hProcess);
        ::CloseHandle(pi.hThread);
        waitProcessThread = 0;
        pi.hProcess = 0;
        pi.hThread = 0;
        string sMsg = fn_format("子进程已经退出 : isThreadWaitFinish == %d, hasFirstReceive == %d, notReceiveLong == %d", (int)isThreadWaitFinish, (int)hasFirstReceive, notReceiveLong);
        outInfo( sMsg );
    }

    void rerunProcess()
    {
        releaseProcess();
        hasFirstReceive = false;
        notReceiveLong = 0;
        createProcess();
    }

public:
    string exeFilePath;
    string exePath;
    string exeFileName;
    int processType;
    int waitFirstMillisecond;

    int localPort;
    string heartHexString;
    string heartBuffer;
    unsigned int timeOut_receive;

    int startTimes;
    msepoch_t startTime;

    //进程
    PROCESS_INFORMATION pi; //进程信息
    HANDLE waitProcessThread;
    volatile bool isThreadWaitFinish;

    //套接字
    SOCKET socketId;
    HANDLE socketThread;

    //接收数据
    volatile bool hasFirstReceive;
    volatile bool isThreadReceiveError;
    volatile bool isThreadReceiveCancel;
    msepoch_t lastReceiveTime;
    volatile unsigned int notReceiveLong;

    string runParam;
    string commandLine;
    int    startMode; //启动模式
};

vector<Processer*> f_oProcesses;

//#pf [process file]是应用程序文件，可填写相对路径或绝对路径
//#pp [process param]进程的参数
//#pt [process type]是应用程序类型32位中最低位表示是否核心程序，如果是核心，此程序一旦over就必须全部应用程序重新启动
//#pw [process wait]是应用程序首次启动等待多长时间再启动下一个 --{默认为启动进程后不等待}
//#np [network port]是网络监听端口号列表，监听 --{默认为空，不启动网络判断}
//#nt [network timeout]是网络超时，没有收到数据的超时 --{默认为"5000"}
//#nh [network heart]是网络心跳报文 --{默认为"a55aa55a"}
//#pm [process mode]进程的模式
void fn_prepareProcessInfo2()
{
    vector<string> sLines;
    fn_fileLoad(f_sApplicationConfigFilePath, sLines, "\r\n");
    vector<vector<string> > sInfoeses;
    sInfoeses = fn_split(sLines, '[', '#');
    vector<string> sCmdFulls;
    vector<int> iPortes;
    for (size_t i = 0; i < sInfoeses.size(); ++i)
    {
        const vector<string> & sInfoes = sInfoeses.at(i);
        map<string, string> sProcessInfos = fn_splitToMap(sInfoes, '=');
        //pf=aaa/aaa.exe,pt=1,pw=3000,np=5566,nt=6600,nh=a55aa55a

        string sExeFilePath;
        int iProcessType;
        int iWaitFirstMillisecond;
        int iLocalPort;
        string sHeartBuffer;
        int iTimeOut_receive;

        string sPf = sProcessInfos["pf"];
        if (sPf.empty())
        {
            cout << "config index : " << i << " 是空路径" << endl;
            continue;
        }
        if (sPf.find(':') == string::npos)
        {
            sExeFilePath = fn_mergeFilePath(f_sApplicationPath, sPf);
        }
        else
        {
            sExeFilePath = sPf;
        }


        string sPp = sProcessInfos["pp"];
        if (sPp.size() > 255)
        {
            cout << "config index : " << i << " 进程的运行参数过长不合法" << endl;
            continue;
        }


        string sCommandLine = sExeFilePath;
        if (sPp.size() > 0)
        {
            sCommandLine.push_back(' ');
            sCommandLine.append(sPp);
        }
        sCommandLine = fn_toLower(sCommandLine);
        if (find(sCmdFulls.begin(), sCmdFulls.end(), sCommandLine) != sCmdFulls.end())
        {
            cout << "config index : " << i << " 是重复的命令行" << endl;
            continue;
        }
        sCmdFulls.push_back(sCommandLine);


        string sPt = sProcessInfos["pt"];
        iProcessType = atoi(sPt.c_str());


        string sPw = sProcessInfos["pw"];
        iWaitFirstMillisecond = atoi(sPw.c_str());
        if (! (iWaitFirstMillisecond >= 0 && iWaitFirstMillisecond < 60 * 1000 * 30))
        {
            cout << "config index : " << i << " 首次启动等待多长时间的数值不合法" << endl;
            continue;
        }


        string sNp = sProcessInfos["np"];
        iLocalPort = atoi(sNp.c_str());

        if (! (iLocalPort >= 0 && iLocalPort < USHRT_MAX))
        {
            cout << "config index : " << i << " 端口不合法" << endl;
            continue;
        }
        if (iLocalPort > 0 && find(iPortes.begin(), iPortes.end(), iLocalPort) != iPortes.end())
        {
            cout << "config index : " << i << " 是重复的端口" << endl;
            continue;
        }
        iPortes.push_back(iLocalPort);


        string sNt = sProcessInfos["nt"];
        iTimeOut_receive = atoi(sNt.c_str());
        if (! (iTimeOut_receive >= 0 && iTimeOut_receive < 86400*1000))
        {
            cout << "config index : " << i << " 网络超时的数值不合法" << endl;
            continue;
        }


        string sNh = sProcessInfos["nh"];
        sHeartBuffer = sNh;
        if (sHeartBuffer.size() > 0)
        {
            if (! (fn_isValidHexCharater(sHeartBuffer) && (sHeartBuffer.size() % 2 == 0) && (sHeartBuffer.size() < 255) ) )
            {
                cout << "config index : " << i << " 心跳报文不合法" << endl;
                continue;
            }
        }
        vector<char> hBuf = fn_fromHexstring(sHeartBuffer);
        string sHBuf = string(hBuf.data(), hBuf.size());


        string sMode = sProcessInfos["pm"];
        int iMode = atoi(sMode.data());
        if (! (iMode >= 0 && iMode < 5))
        {
            cout << "config index : " << i << " 窗口模式不合理" << endl;
            continue;
        }


        Processer * oProcesser = new Processer(sExeFilePath, iWaitFirstMillisecond, iProcessType, iLocalPort, sHBuf, iTimeOut_receive,sPp,iMode);
        f_oProcesses.push_back(oProcesser);
        fn_outInfo( fn_format( "Append Process FilePath : %s \nLocal Port : %d\nHeart String : %d\n", sExeFilePath.c_str(), iLocalPort, sHeartBuffer.c_str() ) );
    }
}

void fn_handler (int param)
{
  std::cout << "recv signal : interactive attention : " << param << std::endl;
  std::cout << "but the application is must execute , can not exit!!!" << param << std::endl;

  for (size_t i = 0; i < f_oProcesses.size(); ++i)
  {
      Processer * oProcesser = f_oProcesses.at(i);
      oProcesser->releaseProcess();
  }
}

//main
int main(int argc, char *argv[])
{
    cout << "begin daemon:" << endl;

    fn_void_int_t prev_handler = signal (SIGINT, fn_handler);
    if (prev_handler == SIG_ERR)
    {
        std::cout << "error : signal (SIGINT , *)" << std::endl;
    }

    fn_application_init(argc, argv);

    fn_prepareProcessInfo2();

    if (f_oProcesses.empty())
    {
        fn_outInfo( "没有要守护的应用程序，检查参数及其正确性!!!" );
        ::system("pause");
        return 0;
    }

    fn_outInfo(string("daemon start time : ") + fn_toString(fn_currentMsepoch()));

    WSADATA            wsd;            //WSADATA变量
    //初始化套结字动态库
    if (::WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        fn_outInfo("初始化套接字动态库失败!!!");
        ::system("pause");
        return 0;
    }

    for (size_t i = 0; i < f_oProcesses.size(); ++i)
    {
        Processer * oProcesser = f_oProcesses.at(i);
        int ret = fn_killProcess(const_cast<char *>(oProcesser->exeFileName.data()));

        fn_outInfo( string("kill [") +fn_toString(ret)+"] 程序 :" + oProcesser->exeFileName );

    }

    for (size_t i = 0; i < f_oProcesses.size(); ++i)
    {
        Processer * oProcesser = f_oProcesses.at(i);
        oProcesser->createProcess();
        if (oProcesser->localPort > 0 && oProcesser->localPort < USHRT_MAX)
        {
            oProcesser->createSocket();
        }
        if (oProcesser->waitFirstMillisecond > 0)
        {
            ::Sleep(oProcesser->waitFirstMillisecond);
            fn_outInfo( string("程序 : ") + oProcesser->exeFileName + "首次启动等待多长时间再启动下一个！！！" );
        }
    }

    msepoch_t dtPreNow = fn_currentMsepoch();
    unsigned int iMsgLong = 0;
    while (1)
    {
        msepoch_t dtNow = fn_currentMsepoch();
        unsigned int dwSpan = dtNow - dtPreNow;
        string sMsg;
        for (size_t i = 0; i < f_oProcesses.size(); ++i)
        {
            Processer * oProcesser = f_oProcesses.at(i);
            //1)exe已经退出; 2)收数据超时
//            if ( oProcesser->isThreadWaitFinish || ( oProcesser->hasFirstReceive && oProcesser->notReceiveLong > oProcesser->timeOut_receive ) )
            if ( oProcesser->isThreadWaitFinish || ( oProcesser->heartBuffer.size()>0 && oProcesser->notReceiveLong > oProcesser->timeOut_receive ) )
            {
                oProcesser->rerunProcess();
                continue;
            }
            else
            {
                unsigned int iLong = oProcesser->notReceiveLong + dwSpan;
                oProcesser->notReceiveLong = iLong;
            }

            //socket发生错误重启进程
            if ( oProcesser->isThreadReceiveError )
            {
                oProcesser->restartSocket();
                continue;
            }

            if (iMsgLong > 1000 * 60)
            {
                sMsg += fn_format("第%d个 程序名称[ %s ] - 监听端口[ %d ] - 心跳报文[ %s ] \n", i+1, oProcesser->exeFileName.c_str(), oProcesser->localPort, oProcesser->heartHexString.c_str());
                sMsg += fn_format("1) 运行时长 : %d 分钟； 启动 : %d 次； 是否收到心跳 : %d \n", (int)((dtNow - oProcesser->startTime) / (1000 * 60)), oProcesser->startTimes , (int)oProcesser->hasFirstReceive);
                sMsg += fn_format("2) 内部信息 : isThreadWaitFinish == %d, hasFirstReceive == %d, notReceiveLong == %d \n\n", (int)oProcesser->isThreadWaitFinish, (int)oProcesser->hasFirstReceive, oProcesser->notReceiveLong);
            }
        }
        if (iMsgLong > 1000 * 60)
        {
            sMsg = fn_format("守护%d个应用程序，以下是各应用程序的信息：\n\n", f_oProcesses.size()) + sMsg;
            fn_outInfo(sMsg);
            iMsgLong = 0;
        }
        iMsgLong += dwSpan;
        dtPreNow = dtNow;
        ::Sleep(100);
    }

    ::WSACleanup();        //释放套接字资源

    ::exit(0);

    return 0;
}

