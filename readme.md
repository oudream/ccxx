
### Git Clone Project 
git clone https://github.com/oudream/ccxx.git

### 
### Compile
cd ccxx
cmake . -DCMAKE_BUILD_TYPE=Debug --build $PWD -B"$PWD/build/cmake-gcc"

### OpenCv On Windows
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

### Full Compile On Ubuntu
```bash

### build and install OpenCV 4.1.2 (4c71dbf)
## https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html
## https://opencv.org/releases/
# cd ~/<my_working_directory>
sudo apt-get update -y && \
sudo apt-get install build-essential && \
sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev && \
sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev 

### git code and make it
cd ~ && \
git clone https://github.com/opencv/opencv.git && \
git reset --hard 4c71dbf && \
cd ~/opencv && mkdir build && cd build && \
cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local .. && \
make -j7 && sudo make install

### apt insall depend libs
# libopencv-dev
sudo apt-get update -y ; sudo apt-get upgrade -y && \
sudo apt-get install gcc g++ cmake build-essential gdb gdbserver \
    unixodbc unixodbc-dev libcurl4-openssl-dev uuid uuid-dev \
    qt5-default libqt5svg5 libqt5svg5-dev qtcreator -y && \
sudo rm -rf /var/lib/apt/lists/*

### down source and compile
cd ~ && \
git clone https://github.com/oudream/ccxx.git && \
cd ccxx && \
cmake . -DCMAKE_BUILD_TYPE=Debug --build "$PWD" -B"$PWD/build/cmake-gcc" && \
cd build/cmake-gcc && make


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

