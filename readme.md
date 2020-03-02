
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

### Git Clone Project 
git clone https://github.com/oudream/ccxx.git

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
cd ~/ccxx/build/deploy/unix/bin_d
./cxtest_timer
./cxtest_channel_udp_client1
./cxtest_channel_udp_server1
./cxsample_lua
./benchmark_cxstring
# ...

```

### Customize Compile
```bash

vim ~/ccxx/build/cmake/local.all.config.cmake
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
# cd ~/<my_working_directory>
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
cd ~ && \
git clone https://github.com/opencv/opencv.git && \
cd ~/opencv && git reset --hard 4c71dbf && \
mkdir build && cd build && \
cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local .. && \
make -j7 && sudo make install

### 3 step: down source and compile
cd ~ && \
git clone https://github.com/oudream/ccxx.git && \
cd ccxx && \
cmake . -DCMAKE_BUILD_TYPE=Debug -DCCXX_BUILD_TYPE=all --build . -B"./build/cmake-gcc" && \
cd build/cmake-gcc && make

### 4 step: run apps
cd ~/ccxx/build/deploy/unix/bin_d
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
