/***
 * setimer 的精度是 ms，POSIX Timer 是针对有实时要求的应用所设计的，接口支持 ns 级别的时钟精度。
 * 首先，一个进程同一时刻只能有一个 timer。假如应用需要同时维护多个 Interval 不同的计时器，必须自己写代码来维护。这非常不方便。使用 POSIX Timer，一个进程可以创建任意多个 Timer。
//
setitimer
getitimer

//POSIX Timer:
timer_create 	创建一个新的 Timer；并且指定定时器到时通知机制
timer_delete	删除一个 Timer
timer_gettime	Get the time remaining on a POSIX.1b interval timer
timer_settime	开始或者停止某个定时器。
timer_getoverrun	获取丢失的定时通知个数
 */