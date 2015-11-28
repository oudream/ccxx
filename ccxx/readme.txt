要做的：
7)return code
6)ICS SHELL
5)GM_INVALID_RETURE 只用在 debug 中
4)static const string xxx = xxx //初始化的先后问题
3)Tcpserver的新连接加入时通知select
2)Tcpserver的select发送
1）Application::signalMainThread 无效的处理



/***
 * 概念
1) needtodo : 须要做的
2) musttodo : 必须做的
3）besttodo : 需要优化
4）attention : 注意
5）risk : 风险所在
 */


/***
* 简称
g 共创 远光
inter center => giic ( rtdb )
inter p         girc
*/




/***
 * SIGNAL SIGNAL SIGNAL
 C语言标准定义了6个信号。都定义在signal.h头文件中[1]。

 SIGABRT - 异常中止。
 SIGFPE - 浮点异常。
 SIGILL - 无效指令。
 SIGINT - 交互的用户按键请求，默认情况下，这会导致进程终止。
 SIGSEGV - 无效内存访问。
 SIGTERM - 程序的中止请求。
 */




/***
 * 配置
 #file : app.config.ini
 [ProgramConfigs]
 #Type_IO_Udp =       0x04000104 == 67109124
 #停止TcpClient、TcpServer、UDP、Icmp、Serial的日志
 #LogDisables=67109120,;67109122,;67109123,;67109124,;67109125,;67109126,;
 #停止Protocol、Terminal、Business的日志
 #LogDisables=67109377,;67109633,;67109889
 #停止UDP的通道ID为2和3的日志
 #LogDisables=67109124,2;67109124,3;

 #file : xxx.ini
 [xxx.Protocol.Channel]
 #发送时是否Push到发送队列
 IsSendQueue=0


 #dbs的定时心跳
 DdsHeartJumpInterval=5000
 DdsHeartJumpSql=select count(*) from t_mj_rt_evt
 */



/***
 * ICS SHELL 外部命令

    命令结构：
    对象 动作/功能 参数
    对象 动作/功能 参数名 ：都小写，可带分割符　'_' 如：channel_udp
    对象 参数名 ：如果操作的是对象的属性可用 '.' 如：terminal.channel.time
    参数：参数名1=参数值1 参数名2=参数值2 （用 key=value 多个参数中间用空格 0x20 ' '，支持转义符'\ '表示参数值中的空格）
         如：channel -report type=udp id>10

 system -exit : Application::exit

 system -save_config : Application::saveConfig()

 terminal -report all : report terminal information
 terminal -report name=xxx : report terminal information

 msobject -report all : report msobject information

 measure -report :

 file send [fileName] : send file from directory [app_data/temp/fileName] to every client

 yx send : send a test yx to every client

 yc send : send a test yc to every client

 message send [filePath\message_text] : send message command from directory [app_data/temp/fileName] to every client

 channel enable packet.out : endable (CMD)command line packet output

 channel disable packet.out : disable (CMD)command line packet output

 interinfo enable\disable all\terminalName : enable\disable all\terminal(protocol and business and channel)

 timer -message xxx -file xxx -interval:: start\stop timer timer to send message, if [app_data/temp/fileName] exist then send message from file

 bus -upate file=c:\xxx.ini

 log -disable all -enable type=0x04000100 source=0x01830101 reason=11

 interinfo -disable all -enable platform=cmd type=0x04000100 source=0x01830101 reason=11

 rank=1


 */


/***
 *
 C标准中空白字符有：空格0x20（‘ ’）、换页0x12（‘\f’）、换行0x10（‘\n’）、回车0x0D（‘\r’）、水平制表符0x09（‘\t’）、垂直制表符0x11（‘\v’）六个

 */



/***
 * 变量前辍

变量种类                             前缀         字节数	示例
 bool : BOOL BOOLEAN                b	1	boolean bSuccess
 char : int8 CHAR                   ch	1	char cKey
 unsigned char : uchar byte BYTE    by	1	byte byFlag
 short : int16 short int            n	2
 unsigned short	: ushort WORD word  w	2
 int : int32 Integer                i	4
 unsigned int : uint uint32         dw	4
 long long : Int64                  l	8
 unsigned long long                 lw	4
 float                              f	4
 double                             d	8
 string                             s	N
 enum                               e	4
 function                           fn	4
 pointer                            p	4
 date time datetime                 dt	8
 object                             o	4
 Class instance                     m	N
 */



/***
 * 全局命名
 变量种类              前缀要求           示例
 全局宏                GM_              GM_DLL_DIRECTION_PORT
 全局常量               CI 或 CS         i或s参考表4-1-1	CI_NUMBER  CS_NAME
 全局变量	g_             g_number
 全局指针变量            g_p              g_pNumber
 对象级私有层变量         m_p               m_number 或 m_pNumber
 对象级保护层变量         m_p               m_number 或 m_pNumber
 文件作用                f_p                f_number 或 f_pNumber

 */



/***
 * cxLog.h

需求
type
output ( file , socket , shm )
debug
realtime , delay
sourceId ( url , ip , hostname , username , writer )
datetime
size
thread
 */



/***
 * cxSocket.h

htonl()--"Host to Network Long"
ntohl()--"Network to Host Long"
htons()--"Host to Network Short"
ntohs()--"Network to Host Short"
网络字节顺序NBO（Network Byte Order）： 按从高到低的顺序存储，在网络上使用统一的网络字节顺序，可以避免兼容性问题。BIG ENDIAN
主机字节顺序（HBO，Host Byte Order）： 不同的机器HBO不相同，与CPU设计有关，数据的顺序是由cpu决定的,而与操作系统无关。
如 Intel x86结构下little endian,short型数0x1234表示为3412, int型数0x12345678表示为78 56 34 12
如 IBM power PC结构下,short型数0x1234表示为1234, int型数0x12345678表示为12 34 56 78 由于这个原因不同体系结构的机器之间无法通信,所以要转换成一种约定的数序,也就是网络字节顺序,其实就是如同powerpc那样的顺序。


socket() 创建一个新的确定类型的套接字，类型用一个整型数值标识（文件描述符），并为它分配系统资源。
bind() 一般用于服务器端，将一个套接字与一个套接字地址结构相关联，比如，一个指定的本地端口和IP地址。
listen() 用于服务器端，使一个绑定的TCP套接字进入监听状态。
connect() 用于客户端，为一个套接字分配一个自由的本地端口号。 如果是TCP套接字的话，它会试图获得一个新的TCP连接。
accept() 用于服务器端。 它接受一个从远端客户端发出的创建一个新的TCP连接的接入请求，创建一个新的套接字，与该连接相应的套接字地址相关联。
send()和recv(),或者write()和read(),或者recvfrom()和sendto(), 用于往/从远程套接字发送和接受数据。
close() 用于系统释放分配给一个套接字的资源。 如果是TCP，连接会被中断。
getsockname函数返回与套接口关联的本地协议地址。
getpeername函数返回与套接口关联的远程协议地址。
gethostbyname()和gethostbyaddr() 用于解析主机名和地址。
select() 用于修整有如下情况的套接字列表： 准备读，准备写或者是有错误。
shutdown() 可以选择性的关闭读、写或读写。
poll() 用于检查套接字的状态。 套接字可以被测试，看是否可以写入、读取或是有错误。
getsockopt() 用于查询指定的套接字一个特定的套接字选项的当前值。
setsockopt() 用于为指定的套接字设定一个特定的套接字选项。


domain:
AF_INET     IPv4
AF_INET6    IPv6
AF_UNIX     Unix域
AF_UNSPEC   未指定


type:
SOCK_DGRAM      长度固定，无连接，不可靠传递 默认协议是UDP
SOCK_RAW        ip协议的数据报接口
SOCK_SEQPACKET  长度固定，有连接，可靠的传递
SOCK_STREAM     有序，可靠，双向的连接字节流 默认协议是TCP


ai_flags:
AI_ADDRCONFIG   查询配置的地址类型(IPv4 or IPv6)
AI_ALL          查找IPv4 and IPv6 地址(仅用于AI_V4MAPPED)
AI_CANONNAME    需要一个规范名
AI_NUMERICHOST  数字格式返回主机地址
AI_NUMERICSERV  以端口号返回服务
AI_PASSIVE      套接字地址用于监听绑定
AI_V4MAPPED     如果没有IPv6，返回映射到IPv4的地址


IP_TTL 是控制数据包的生存期的，一般设为1，如果是0，数据包将无法穿过本机，1，数据包无法穿过路由器，2，无法穿过第2个路由器，以此类推。


getsockopt():
setsockopt():
IP_MULTICAST_TTL    设置多播组数据的TTL值
IP_ADD_MEMBERSHIP   在指定接口上加入组播组
IP_DROP_MEMBERSHIP  退出组播组
IP_MULTICAST_IF     获取默认接口或设置接口
IP_MULTICAST_LOOP   禁止组播数据回送

 */



/***
 * re2 regexp 正规表达式

 RE2::Replace
 例如 ："pattern" = (qu|[b-df-hj-np-tv-z]*)([a-z]+)
     : "rewrite" = \2\1ay       解释：\2 表示匹配到([a-z]+)的值即上例中的 e , 而 \1 表示匹配到(qu|[b-df-hj-np-tv-z]*)的值即上例中的 th

*/


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


/**
//单个字符异或运算
char MakecodeChar(char c,int key)
{
    return c=c^key;
}
//单个字符解密
char CutcodeChar(char c,int key)
{
    return c^key;
}
//加密
void Encode(char *pstr,int *pkey)
{
    int len=strlen(pstr);//获取长度
    for(int i=0;i<len;i++)
        *(pstr+i)=MakecodeChar(*(pstr+i),pkey[i%5]);
}
//解密
void Decode(char *pstr,int *pkey)
{
    int len=strlen(pstr);
    for(int i=0;i<len;i++)
        *(pstr+i)=CutcodeChar(*(pstr+i),pkey[i%5]);
}

int main(void)
{
    printf("Hello World!\n");

    //示例代码
    int key[]={1,2,3,4,5};//加密字符 5位
    char s[]="www.xiaozhuanggushi.com";
    char *p=s;
    cout<<"Encode before:"<<p<<endl;
    Encode(s,key);//加密
    cout<<"Encode after:"<<p<<endl;
    Decode(s,key);//解密
    cout<<"Decode:"<<p<<endl;

    return 0;
}
 */
