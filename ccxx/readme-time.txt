
time                        C系统调用	低	<1s
clcok                       C系统调用	低	<10ms
gettimeofday                linux环境下C系统调用	高	<0.1ms
clock_gettime               linux环境下C系统调用	高	<0.1ms
timeGetTime                 Windows API	中	<1ms
QueryPerformanceCounter     Windows API	高	<0.1ms
QueryPerformanceFrequency   Windows API	高	<0.1ms
GetTickCount                Windows API	中	<1ms
GetLocalTime                Windows API	中	<1ms
GetTickCount
GetSystemTimeAsFileTime
RDTSC                       汇编指令	高	<0.1ms
