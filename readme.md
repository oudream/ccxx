
### 
- This is a cross-platform library software library about c, c ++, unix4, posix. 
- This library has been continuously developed for more than three years, and it serves as the underlying support for many large projects. 
- Incorporate unit test, benchmark test, cmake, process monitoring, daemon, asynchronous libraries libuv, lua, cpython, re2, json, yaml, mysql, redis, opencv, qt, lz4, oci ...

### 
- 这是关于c，c ++，unix4，posix的跨平台库软件库。 
- 这个库已经连续开发了三年多，它为许多大型项目提供了基础支持。 
- 合并了单元测试，基准测试，cmake，进程监视，守护程序，异步库libuv，lua，cpython，re2，json，yaml，mysql，redis，opencv，qt，lz4，oci ...

### Git Clone Project 
git clone https://github.com/oudream/ccxx.git

### Tools Download
- [clion - https://www.jetbrains.com/clion/download/other.html](https://www.jetbrains.com/clion/download/other.html)
- [vscode - https://code.visualstudio.com/download](https://code.visualstudio.com/download)
- [qt - http://download.qt.io/archive/qt/](http://download.qt.io/archive/qt/)

### May be install related dependent libraries according to your needs
- [windows sdk - https://developer.microsoft.com/en-us/windows/downloads/sdk-archive/](https://developer.microsoft.com/en-us/windows/downloads/sdk-archive/)
- [windows sdk - https://developer.microsoft.com/zh-cn/windows/downloads/sdk-archive/](https://developer.microsoft.com/zh-cn/windows/downloads/sdk-archive/)

### Compile
cd ccxx
cmake . -DCMAKE_BUILD_TYPE=Debug --build . -B"./build/cmake-gcc"

### OpenCv On Windows
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
```cmd

git clone https://github.com/oudream/ccxx.git


```
ssh root@122.51.12.151 -AXY

### Full Compile On Ubuntu
```bash

### 1 step: build and install OpenCV 4.1.2 (4c71dbf)
## https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html
## https://opencv.org/releases/
# cd ~/<my_working_directory>
### 1 step
sudo apt-get update -y && \
sudo apt-get install build-essential && \
sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev && \
sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev 

### 2 step: git code and make it
cd ~ && \
git clone https://github.com/opencv/opencv.git && \
git reset --hard 4c71dbf && \
cd ~/opencv && mkdir build && cd build && \
cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local .. && \
make -j7 && sudo make install

### 3 step: apt insall depend libs
# libopencv-dev
sudo apt-get update -y ; sudo apt-get upgrade -y && \
sudo apt-get install gcc g++ cmake build-essential gdb gdbserver \
    unixodbc unixodbc-dev libcurl4-openssl-dev uuid uuid-dev \
    qt5-default libqt5svg5 libqt5svg5-dev qtcreator -y && \
sudo rm -rf /var/lib/apt/lists/*

### 4 step: down source and compile
cd ~ && \
git clone https://github.com/oudream/ccxx.git && \
cd ccxx && \
cmake . -DCMAKE_BUILD_TYPE=Debug --build . -B"./build/cmake-gcc" && \
cd build/cmake-gcc && make

### 5 step: run apps
~/ccxx/build/deploy/unix/bin_d/benchmark_cxstring

```

# ccxx
clib, std, stl, Cross-platform, tcp, udp, serail, xml, regular expression, gtest, lua, json

todo:
CxDatabase::loadSql : if loaded count > max return

todo:
add libuv channel

todo:
exit or signal : reentrant function
e.g. : CxLog() <<

