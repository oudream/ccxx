//
// Created by Administrator on 2017/9/13.
//

#ifndef PROJECT_DEFINE_OS_H
#define PROJECT_DEFINE_OS_H


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



#endif //PROJECT_DEFINE_OS_H
