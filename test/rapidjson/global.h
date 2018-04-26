
#ifndef PROJECT_GLOBAL_H
#define PROJECT_GLOBAL_H


#if defined(_MSC_VER) && (_MSC_VER==1600)
#define GM_CPP_11_ON
#endif

#if defined(_MSC_VER) && (_MSC_VER==1700)
#define GM_CPP_11_ON
#endif

#if defined(_MSC_VER) && (_MSC_VER==1800)
#define GM_CPP_11_ON
#endif

#if defined(__GNUC__) && defined(__GNUC_MINOR__) && (__GNUC__*100+__GNUC_MINOR__<=407) && (__GNUC__*100+__GNUC_MINOR__>=404)
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif

#if defined(__GNUC__) && defined(__GNUC_MINOR__) && (__GNUC__*100+__GNUC_MINOR__>=701)
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
#endif

#if defined(__clang__) && (__clang_major__*100+__clang_minor__ >= 306)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wimplicit-fallthrough"
#endif


// auto-detection of "std" compiler switches

#if defined(_MSC_VER)

// >= VC++ 2015 Update 3, and /std:c++14
#if defined(_MSVC_LANG) && (_MSVC_LANG==201402L)
#if !defined(GM_CPP_11_ON)
#define GM_CPP_11_ON
#endif
#if !defined(GM_CPP_14_ON)
#define GM_CPP_14_ON
#endif
#endif

// >= VC++ 2015 Update 3, and /std:c++latest
#if defined(_MSVC_LANG) && (_MSVC_LANG>201402L)
#if !defined(GM_CPP_11_ON)
#define GM_CPP_11_ON
#endif
#if !defined(GM_CPP_14_ON)
#define GM_CPP_14_ON
#endif
#if !defined(GM_CPP_17_ON) && defined(_MSC_VER) && (_MSC_VER>=1910)
#define GM_CPP_17_ON
#endif
#if defined(_MSC_VER) && (_MSC_VER>=1910)
#define GM_COMPILER_HAS_STD_OPTIONAL
#endif
#endif

#endif

#if defined(__clang__) || defined(__GNUC__)
// clang or g++
#if defined(__cplusplus) && (__cplusplus==201103L)

// std=c++11 is used
#if !defined(GM_CPP_11_ON)
#define GM_CPP_11_ON
#endif

#elif defined(__cplusplus) && (__cplusplus==201402L)

// std=c++14 is used
#if !defined(GM_CPP_11_ON)
#define GM_CPP_11_ON
#endif
#if !defined(GM_CPP_14_ON)
#define GM_CPP_14_ON
#endif

#elif defined(__cplusplus) && (__cplusplus>201402L)

// std=c++17 / std=c++1z is used
#if !defined(GM_CPP_11_ON)
#define GM_CPP_11_ON
#endif
#if !defined(GM_CPP_14_ON)
#define GM_CPP_14_ON
#endif
#if !defined(GM_CPP_17_ON)
#define GM_CPP_17_ON
#endif

#endif
#endif

#if defined(GM_CPP_11_ON) && defined(GM_FUNC_THROW_SPEC_ON)
#error GM_CPP_11_ON and GM_FUNC_THROW_SPEC_ON cannot be used together
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma warning(disable : 28251)
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1900)
#pragma warning(disable : 6385)
#pragma warning(disable : 6386)
#endif



#include <typeinfo>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <bitset>
#include <stack>
#include <queue>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <stddef.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <signal.h>
#include <ctype.h>
#include <wchar.h>
#include <wctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>

static void gprint(std::string msg)
{
    std::cout << msg << std::endl;
}

static std::string ginput()
{
    std::string msg;
    std::cin >> msg;
    return msg;
}

inline static void TestVector21()
{
    std::string str;
    do
    {
        str = ginput();

        if (str[0] == 'C')
        {
            if (str[1] == 'B')
            {
                if (str[2] == '0')
                {

                }
            }
            else if (str[1] == 'E')
            {
            }
            else if (str[1] == 'T')
            {
            }
            else if (str[1] == 'R')
            {
            }
            else if (str[1] == 'C')
            {
                break;
            }
        }
        else
        {
        }
    } while (str != "CC");
}

static int argc = 0;
static char *agrv[] = {NULL};

#endif //PROJECT_GLOBAL_H
