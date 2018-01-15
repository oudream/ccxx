
set sExecDisk=%~d0%
set sExecPath=%~dp0%\
set sExecPath=%sExecPath:\\=\%

#sPathYgct_rtdb 是RTDB的路径
#sPathYgct_104_v100 是Ygct_104_v100的路径

set sPathYgct_rtdb=%sExecPath%ygct_rtdb\ygct_rtdb.exe
set sPathYgct_104_v100=ygct_104\ygct_104_v100.exe

echo  "sExecPath="%sExecPath%
echo  "sPathYgct_rtdb="%sPathYgct_rtdb%
echo  "sPathYgct_104_v100="%sPathYgct_104_v100%

cd /d %sExecPath%

#用带参数的守护程序来起动相关的应用程序
#支持多个应用程序，多个应用程序之间用分号隔开
#一个应用程序对应一个UDP的监听端口


###
#sample
#-config pf=bbb/bbb.exe;pf=aaa/aaa.exe,pt=1,pw=3000,np=5566,nt=6600,nh=a55aa55a
###
#
#pf [process file]是应用程序文件，可填写相对路径或绝对路径
#pt [process type]是应用程序类型32位中最低位表示是否核心程序，如果是核心，此程序一旦over就必须全部应用程序重新启动
#pw [process wait]是应用程序首次启动等待多长时间再启动下一个 --{默认为启动进程后不等待}
#np [network port]是网络监听端口号列表，监听 --{默认为空，不启动网络判断}
#nt [network timeout]是网络超时，没有收到数据的超时 --{默认为"5000"}
#nh [network heart]是网络心跳报文 --{默认为"a55aa55a"}
#pp [process param]进程的参数
#注意 : 多个应用程序之间用分隔符';',参数不填为默认值
#-config pf=%sPathYgct_rtdb%;pf=%sPathYgct_104_v100%,pt=1,pw=3000,np=5566,nt=6600,nh=a55aa55a
start %sExecPath%gcdaemon.exe -config pf=%sPathYgct_104_v100%,pt=1,pw=3000,np=5566,nt=6600,nh=a55aa55a;pf=%sPathYgct_rtdb%

