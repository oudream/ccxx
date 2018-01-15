#ifndef CXUUID_H
#define CXUUID_H

#include "cxglobal.h"

#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct GUID {
  unsigned int Data1;
  unsigned short Data2;
  unsigned short Data3;
  unsigned char Data4[8];
} GUID;
#endif

class GM_CCXX_CORE_API CxUuid
{
public:
    static std::string createGuidString();

    static std::string guidToString(const GUID & guid);

    static GUID guidFromString(const std::string & sGuidString);

    static std::string createRandString();

#ifdef __ANDROID__
    static GUID createGuid(JNIEnv *env);
#else
    static GUID createGuid();
#endif
};

#endif // CXUUID_H
