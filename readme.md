---

## **CCXX** (en)
> 1. This is a cross-platform library software library about c, c ++, unix4, posix. 
> 2. This library has been continuously developed for more than three years, and it serves as the underlying support for many large projects. 
> 3. Incorporate unit test, benchmark test, cmake, process monitoring, daemon, asynchronous libraries libuv, lua, cpython, re2, json, yaml, mysql, redis, opencv, qt, lz4, oci ...

## **CCXX** (cn)
> 1. 这是关于c，c ++，unix4，posix的跨平台库软件库。 
> 2. 这个库已经连续开发了三年多，它为许多大型项目提供了基础支持。 
> 3. 合并了单元测试，基准测试，cmake，进程监视，守护程序，异步库libuv，lua，cpython，re2，json，yaml，mysql，redis，opencv，qt，lz4，oci ...

## 宣言
> 库要轻量，精简，尽量一个模块用一个文件完成，模块之间尽量不产生依赖，尽量单独拷贝几个文件就能完成目标功能。在达到效率基础上库的使用次序 -> clib ->stl -> unix api -> posix -> os api。要多写 unit test , benchmark , docker in k8s 并行分布式测试。

## Manifesto
> The library should be lightweight and streamlined, and one module should be completed with one file as much as possible. There should be no dependencies between the modules. Try to copy several files separately to complete the target function. On the basis of efficiency, the order of use of the library-> clib-> stl-> unix api-> posix-> os api. To write more unit test, benchmark, docker in k8s parallel distributed testing.

## 简介 (brief introduction)
> ccxx的核心库代码在根目录下ccxx文件夹中中，大概有（14MB 左右）。模块大多以一个文件.cpp来完成功能，默认静态类方式直接调用）。例子可参考 sample 及 test 的目录。
> 1.  cxappenv.h: 运行环境（运行程序名, 工作目录, 环境变量）
> 2.  cxinterinfo.h: 程序的信息输入输出接口。实现有: cxlog.h（日志）, CxConsoleInterinfo（终端）
> 3.  cxfile.h: 文件系统
> 4.  cxsocket.h: 网络通信
> 5.  cxthread.h: 线程信号,IPC
> 6.  cxapplication.h 类似（Node.js）中的Event Loop。采用 wait 信号机制对事件队列进行轮询并执行。目前参与到 Event Loop 中的有（cxchannel.h, cxtimer.h）。cxapplication_qt.h 是 Qt 参与到 Qt 的 Event Loop。
> 7.  cxchannel.h: （要实例化来使用）开独立线程进行收发和监听，收到数据及事件 push 到主线程的 Event Loop 中，因此数据不需要跨线加锁。实现有: cxchannel_udp.h, cxchannel_tcpserver.h, cxchannel_tcpclient.h, cxchannel_serial.h
> 8.  cxcrc.h: 数据检验
> 9.  cxdatabase.h: （要实例化来使用）以表的方式统一数据操作的接口类。实现有：cxdatabase_odbc.h, cxdatabase_sqlite.h（直接调用官方 sqlite3.c来实现相关功能）
> 10. cxprocess.h: 系统进程
> 11. cxprocmutex.h: 进程锁
> 12. cxtimer.h: （要实例化来使用）定时器
> 13. cxtime.h: 时间与日期
> 14. cxuuid.h: GUID, UUID
> 15. cxsharememory.h: 共享内存,进程间通信IPC (InterProcess Communication) 
> 16. cxprofile.h: （要实例化来使用）配置文件以表的方式来封闭使用。实现有: cxprofile_skv_json.h, cxprofile_skv_xml.h。(skv: section key value)
> 17. cxencoding.h: 实现 GBK 与 UTF-8 之间的转换
> 18. cxlua文件夹: 以 lua 的方式公开 ccxx 功能块，lua_run.h 是 main的入口。
> 19. 其它第三方封装有(cxhiredis.h(Redis的封闭), cxtinyxml.h(Xml文件读写), cxrapidjson.h(正则表达式)，lz4.h（压缩解压）)
> 20. gtest, luv, benchmark, yaml 的自身源码已经放到 3rd 中。
> 21. openssl, curl 是以头文件加库的方式 

## brief introduction
> The core library code of ccxx is located in the ccxx folder in the root directory (about 14MB). Modules mostly use a file .cpp to complete the function, and the default static class method is called directly). For examples, please refer to the directory of sample and test.
> 1.  cxappenv.h: running environment (run program name, working directory, environment variables)
> 2.  cxinterinfo.h: program information input and output interface. Implementations are: cxlog.h (log), CxConsoleInterinfo (terminal)
> 3.  cxfile.h: file system
> 4.  cxsocket.h: network communication
> 5.  cxthread.h: thread signal, IPC
> 6.  cxapplication.h is similar to the Event Loop in (Node.js). The wait signal mechanism is used to poll and execute the event queue. Currently participating in the Event Loop (cxchannel.h, cxtimer.h). cxapplication_qt.h is the event loop where Qt participates in Qt.
> 7.  cxchannel.h: (to be instantiated to use) open a separate thread to send and receive and monitor, receive data and events push to the Event Loop of the main thread, so the data does not need to be locked across lines. Implementations are: cxchannel_udp.h, cxchannel_tcpserver.h, cxchannel_tcpclient.h, cxchannel_serial.h
> 8.  cxcrc.h: data check
> 9.  cxdatabase.h: (to be instantiated for use) interface class that unifies data operations in a table. The implementations are: cxdatabase_odbc.h, cxdatabase_sqlite.h (directly call the official sqlite3.c to achieve related functions)
> 10. cxprocess.h: system process
> 11. cxprocmutex.h: process lock
> 12. cxtimer.h: (to be instantiated to use) timer
> 13. cxtime.h: time and date
> 14. cxuuid.h: GUID, UUID
> 15. cxsharememory.h: shared memory, IPC (InterProcess Communication)
> 16. cxprofile.h: (to be instantiated for use) The profile is closed for use as a table. The implementations are: cxprofile_skv_json.h, cxprofile_skv_xml.h. (skv: section key value)
> 17. cxencoding.h: convert between GBK and UTF-8
> 18. cxlua folder: exposes ccxx function blocks in lua, lua_run.h is the main entry.
> 19. Other third-party packages include (cxhiredis.h (closed by Redis), cxtinyxml.h (Xml file read and write), cxrapidjson.h (regular expression), lz4.h (compression and decompression))
> 20. gtest, luv, benchmark, yaml's own source code has been placed in 3rd.
> 21. openssl, curl is a header file plus library

---

### Source Code
```bash
git clone https://github.com/oudream/ccxx.git
```
[Docker of ccxx as -> https://hub.docker.com/u/oudream](https://hub.docker.com/u/oudream)

### Two libraries are required ( 有两个库是必须的 )
> 1. ODBC : ccxx_database_odbc is a library for connecting databases with odbc
> 1. ODBC : ccxx_database_odbc 是一个用 odbc 连接数据库的库
> 2. UUID : Except for windows, CxUuid (uuid) is implemented using the libuuid library
> 2. UUID : 除了 windows 以外， CxUuid (uuid) 是用 libuuid库实现的
```bash

### mac
brew install unixodbc

### Ubuntu: -D"OTL_ODBC_UNIX" -lodbc
sudo apt-get install unixodbc unixodbc-dev
sudo apt-get install uuid-dev

### CentOS: -D"OTL_ODBC_UNIX" -lodbc
sudo yum install unixODBC-devel
sudo yum install libuuid-devel

```

### Simple Compile
```cmd

# git and compile
git clone https://github.com/oudream/ccxx.git
cd ccxx
cmake . -DCMAKE_BUILD_TYPE=Debug --build . -B"./build/cmake-gcc"
cd ./build/cmake-gcc && make

# try run gtest (google test)
make test

# try run app
cd /opt/ccxx/build/deploy/unix/bin_d
./cxtest_timer
./cxtest_channel_udp_client1
./cxtest_channel_udp_server1
./cxsample_lua
./benchmark_cxstring
# ...

```

### Customize Compile
```bash

vim /opt/ccxx/build/cmake/local.all.config.cmake
# e.g. modify -> set(gs_project_enable_uv 1) to set(gs_project_enable_uv 0)
# e.g. modify -> set(gs_project_enable_openssl 1) to set(gs_project_enable_openssl 0)

```

### Compiled pictures for each operating system (en)
> 各个操作系统编译后的图片 (cn)
- [man](./man)
- [Ubuntu](./ubuntu-run-snips.md)
- [Windows](./windows-run-snips.md)
- [MacOs](./macos-run-snips.md)

### Full Compile On Ubuntu
```bash

### 1 step: build and install OpenCV 4.1.2 (4c71dbf)
## https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html
## https://opencv.org/releases/
# cd /opt/<my_working_directory>
### 1 step: apt insall depend libs
# libopencv-dev
sudo echo "deb http://security.ubuntu.com/ubuntu xenial-security main" | tee /etc/apt/sources.list.d/libjasper.list && \
sudo apt update -y ; apt upgrade -y && \
sudo apt install -y gcc g++ cmake build-essential gdb gdbserver git \
        unixodbc unixodbc-dev libcurl4-openssl-dev uuid uuid-dev \
        qt5-default libqt5svg5 libqt5svg5-dev qtcreator \
        libssl-dev libncurses5-dev \
        libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev \
        python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev \
        software-properties-common 

### 2 step: git code and make it
cd /opt && \
git clone https://github.com/opencv/opencv.git && \
cd /opt/opencv && git reset --hard 4c71dbf && \
mkdir build && cd build && \
cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local .. && \
make -j7 && sudo make install

### 3 step: down source and compile
cd /opt && \
git clone https://github.com/oudream/ccxx.git && \
cd ccxx && \
cmake . -DCMAKE_BUILD_TYPE=Debug -DCCXX_BUILD_TYPE=all --build . -B"./build/cmake-gcc" && \
cd build/cmake-gcc && make

### 4 step: run apps
cd /opt/ccxx/build/deploy/unix/bin_d
./cxtest_timer
./cxtest_channel_udp_client1
./cxtest_channel_udp_server1
./cxsample_lua
./benchmark_cxstring
# ...

```

### ccxx source code
- [ccxx source ->](./ccxx)
- [third-party source ->](./3rd)

### examples 
- [sample code ->](./sample)

### test, but not googletest
- [test code ->](./test)

### Tools
- [clion - https://www.jetbrains.com/clion/download/other.html](https://www.jetbrains.com/clion/download/other.html)
- [vscode - https://code.visualstudio.com/download](https://code.visualstudio.com/download)
- [qt - http://download.qt.io/archive/qt/](http://download.qt.io/archive/qt/)

### Tools On Windows
- Download and install Microsoft Visual C++ Build Tools 2015[http://go.microsoft.com/fwlink/?LinkId=691126](http://go.microsoft.com/fwlink/?LinkId=691126)
- Download and install cmake-> [https://github.com/Kitware/CMake/releases/download/v3.16.4/cmake-3.16.4-win64-x64.msi](https://github.com/Kitware/CMake/releases/download/v3.16.4/cmake-3.16.4-win64-x64.msi)
- or [https://cmake.org/download/](https://cmake.org/download/)
- Download and install qt [http://download.qt.io/archive/qt/5.6/5.6.3/qt-opensource-windows-x86-mingw492-5.6.3.exe](http://download.qt.io/archive/qt/5.6/5.6.3/qt-opensource-windows-x86-mingw492-5.6.3.exe)
- or [http://download.qt.io/archive/qt/](http://download.qt.io/archive/qt/)
- Add ( your qt5.6.3mingw\Tools\mingw492_32\bin) To PATH
- Download and install git [https://github.com/git-for-windows/git/releases/download/v2.25.1.windows.1/Git-2.25.1-64-bit.exe](https://github.com/git-for-windows/git/releases/download/v2.25.1.windows.1/Git-2.25.1-64-bit.exe)
- or [https://git-scm.com/download/win](https://git-scm.com/download/win) 
- Download and install github desktop [https://desktop.githubusercontent.com/releases/2.3.1-8a055015/GitHubDesktopSetup.exe](https://desktop.githubusercontent.com/releases/2.3.1-8a055015/GitHubDesktopSetup.exe)
- or [https://desktop.github.com/](https://desktop.github.com/)

### May be install related dependent libraries according to your needs
- [windows sdk - https://developer.microsoft.com/en-us/windows/downloads/sdk-archive/](https://developer.microsoft.com/en-us/windows/downloads/sdk-archive/)
- [windows sdk - https://developer.microsoft.com/zh-cn/windows/downloads/sdk-archive/](https://developer.microsoft.com/zh-cn/windows/downloads/sdk-archive/)

### Jenkins
- [refer to -> https://github.com/oudream/hello-jenkins](https://github.com/oudream/hello-jenkins)

### Docker
- [https://hub.docker.com/u/oudream](https://hub.docker.com/u/oudream)
- [refer to -> https://github.com/oudream/hello-docker](https://github.com/oudream/hello-docker)

### reference
- The ccxx base layer calls these libraries such as clib, stl, unix4 interface, OS Api and so on. And referenced stl, boost, qt, ucommon (en)
- ccxx底基层调用了clib, stl, unix4接口, OS Api等这些库。并参照了 stl,boost, qt, ucommon (cn)
- [stl -> http://www.cplusplus.com/](http://www.cplusplus.com/)
- [qt -> https://github.com/oudream/qt-4.8.7](https://github.com/oudream/qt-4.8.7)
- [boost -> https://github.com/oudream/boost_1_42_0](https://github.com/oudream/boost_1_42_0)
- [ucommon -> https://github.com/oudream/ucommon](https://github.com/oudream/ucommon)

### ccxx的交叉编译目录
- [ccxx的交叉编译目录](./build/cross-compile)

### todo
todo:
CxDatabase::loadSql : if loaded count > max return

todo:
add libuv channel

todo:
exit or signal : reentrant function
e.g. : CxLog() <<
