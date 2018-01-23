
要做的 needtodo：
增加：
11)bus在有10.99.3.48时会把自己实时值改变
10)退出时不保存配置
9)MeasureShareMemoryAll::setAfterLoadCallback 改成支持多个回调
8)怎么样中断此 stdin in CxConsoleInputThread
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
 *
 Here are values of _MSC_VER for various versions of the Visual C++ compiler:
 MSVC++ 4.x  _MSC_VER == 1000 (Visual Studio 4.0?)
 MSVC++ 5.0  _MSC_VER == 1100 (Visual Studio 97)
 MSVC++ 6.0  _MSC_VER == 1200 (Visual Studio 6.0)
 MSVC++ 7.0  _MSC_VER == 1300 (Visual Studio .NET 2002)
 MSVC++ 7.1  _MSC_VER == 1310 (Visual Studio .NET 2003)
 MSVC++ 8.0  _MSC_VER == 1400 (Visual Studio 2005)
 MSVC++ 9.0  _MSC_VER == 1500 (Visual Studio 2008)
 MSVC++ 10.0 _MSC_VER == 1600 (Visual Studio 2010)
 MSVC++ 11.0 _MSC_VER == 1700 (Visual Studio 2012)
 MSVC++ 12.0 _MSC_VER == 1800 (Visual Studio 2013)
 MSVC++ 14.0 _MSC_VER == 1900 (Visual Studio 2015)
 MSVC++ 14.1 _MSC_VER >= 1910 (Visual Studio 2017)
 */


/***
 * oledb connection

    packet size=4096;user id=sa;data source='.';Initial Catalog =AcmData;persist security info=True;password=1234

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
ms-help://MS.MSDNQTR.v90.chs/winsock/winsock/getting_started_with_winsock.htm

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



ICS 目录
|--- deploy        发布目录
    |--- unix
    |--- solaris
    |--- win64
    |--- win32
        |--- bin        exe / dll 文件目录， Release版本，不依赖大型第三方库（如，QT，Boost等）
        |--- bin_d      exe / dll 文件目录， Debug版本，不依赖大型第三方库（如，QT，Boost等）
        |--- bin_qt     exe / dll 文件目录， Debug版本，依赖QT库
        |--- doc        分子目录，存放总体和子模块的需求、设计、测试用例、测试方案文档
        |--- config     模块配置文件
        |--- data       本地运行产生的数据，可以存放到单机DB或相应目录的文件中
        |--- log        分子目录
        |--- script     本工程依赖或使用到的脚本目录
        |--- temp       临时使用、生成的文件，些目录的文件可随时删除的
        |--- res        resource、图片、UI、样式


|--- cc4000
    |--- build    makefile、project等文件（如果单个程序的构建文件在三个以上，要以文件夹方式出现）windows 上 vs 编译器对应的 sln ncb 等文件
        |--- vcpro  windows 上 vs 编译器对应的 sln ncb 等文件
        |--- linux  linux 上 makefile 文件
    |--- src       源代码，是否按照细节再分子目录依据担当，但是最多按照子模块加以区分
        //以下为模块实现代码
        |--- db      数据库一致访问接口
        |--- softbus 软总线实现
        |--- rtdb    实时库
        |--- 图形
        |--- ......
    |--- test      代码测试
        |--- db      数据库一致访问接口的测试
        |--- SoftBus 软总线实现
        |--- RTDB    实时库
        |--- 图形
        |--- ......
    |--- temp       此目录下文件不上传
    |--- res        resource
    |--- example    对于所有的基础模块库，提供使用例子，强制使用
    |--- out        编译中的临时文件，修改 vs 中的设置，可以将输出的 exe / dll / lib / 中间文件导向到不同的文件夹
    |--- doc        分子目录，存放总体和子模块的需求、设计、测试用例、测试方案文档
    |--- lib        lib
    |--- basic      基础的源码库，例如字符串处理、文件处理、时间处理、协议封包/解包、log库 这些都不会封装成单独的 dll 或者 so 了，
    |--- include    所有外部引用库的头文件，第三方库一旦引用，和自己编写的库同等对待
       |--- db        数据库一致访问接口头文件
       |--- softbus   软总线头文件





select ID,Name,1 as Level from tblCity where ParentID=0
union all
select a.ID,a.Name,c.RowID as Level from tblCity a
inner join tblCity b on a.ParentID=b.ID
inner join
(
select ParentID from tblCity group by ParentID
) c on a.ParentID=c.ParentID

表结构：tblCity(ID, ParentID, Name)

因为sqlite 没有row_number函数，也不能递归查询，所幸它有RowID 这个字段。只好采用这种 笨方法





static pid_os_t exec(const std::string & sProgram,
                     const std::vector<std::string> & arguments = std::vector<std::string>(),
                     const std::string & sWorkingDirectory = std::string(),
                     const std::vector<std::string> & environments = std::vector<std::string>());

static pid_os_t exec(const std::string & sProgram,
                     const std::string & sArguments = std::string(),
                     const std::string & sWorkingDirectory = std::string(),
                     const std::string & sEnvironments = std::string());
