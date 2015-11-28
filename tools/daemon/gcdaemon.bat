
set sExecDisk=%~d0%
set sExecPath=%~dp0%\
set sExecPath=%sExecPath:\\=\%

#sPathPc 是规约转换器的路径
#sPathTs 是票服务的路径

set sPathPc=%sExecPath%ProtocolConvertor\ProtocolConvertor.exe
set sPathTs=%sExecPath%TicketServer\TicketServer.exe

echo  "sExecPath="%sExecPath%
echo  "sPathPc="%sPathPc%
echo  "sPathTs="%sPathTs%

cd /d %sExecPath%

#用带参数的守护程序来起动相关的应用程序
#支持多个应用程序，多个应用程序之间用分号隔开
#一个应用程序对应一个UDP的监听端口
#-p [process]是应用程序列表
#-n 是监听端口号列表，监听
#-t [timeout]是没有收到数据的超时
#-h [heart]是心跳报文
#注意 : 应用程序与监听端口是一对一的对应关系
start %sExecPath%gcdaemon.exe -t 6000 -p %sPathPc%;%sPathTs% -n 5561;5562 -h a55aa55a;a55aa55a