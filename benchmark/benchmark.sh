#!/usr/bin/env bash


# fix bug : error compile on windows
/opt/ddd/ccpp/ccxx/3rd/benchmark/src/sysinfo.cc
/opt/ddd/ccpp/ccxx/3rd/benchmark/src/timers.cc
#ifndef BENCHMARK_OS_MINGW
#include <versionhelpers.h>
#include <windows.h>
#include <codecvt>
#endif

# http://hk.uwenku.com/question/p-ujdvlkzx-ud.html
# 即使使用GCC或Clang或其他編譯器，您仍然使用VersionHelpers.h。這些函數與Visual C++無關。它們是所有Windows應用程序必須使用的Windows SDK的一部分。
#
#您可以download the Windows SDK from Microsoft並將其安裝在您的開發機器上，如果它尚未安裝。安裝完成後，您需要告訴Code :: Blocks它可以找到SDK的頭文件和LIB文件的路徑。
#
#VersionHelpers.h將會包含在這些標題中（但您需要包含的內容是Windows.h）。 VersionHelpers.h文件定義了一系列內聯函數，最終調用從kernel32.lib和ntdll.lib導出的函數。只要鏈接器能夠找到這些LIB文件，一切都將按預期工作。
#
#但是請注意，很少有必要確定Windows的實際版本。相反，您應該檢查是否需要支持您需要的功能。如果您想要調用可能無法使用的API函數（例如，僅在Windows 10上支持的功能，而您仍需要支持較早的版本），則應該將GetModuleHandle或LoadLibrary與GetProcAddress結合使用dynamically call that function。 （如果您致電LoadLibrary，則在完成後還必須致電FreeLibrary。）