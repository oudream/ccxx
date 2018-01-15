#ifndef GCL_VERSION_H
#define GCL_VERSION_H


//编译版本号
#define VERSION_BUILD  "73"
//源码版本范围
#define REVRANGE_BUILD  "76"
//编译时SVN服务器时间
#define SERVER_DATETIME_BUILD "2017/03/10 17:02:07"
//编译时本地时间
#define LOCAL_DATETIME_BUILD "2017/03/16 08:49:27"
//是否为混合版本（1：混合版本；0：非混合版本）
#define IS_MIXED 0
//本地源码是否存在修改（1：存在修改；0：不存在修改），若存在表示本地与SVN服务器源码不一致，此时应禁止发布
#define IS_MODS 0
//源码在服务器上的存放目录
#define SVN_URL "https://10.31.16.253/svn/ygct_ics_cc4000/trunk/cc4000/build/qtpro"


#endif	/* GCL_VERSION_H */
