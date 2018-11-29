#include "cxuuid.h"

// https://github.com/graeme-hill/crossguid

#include "cxstring.h"
#include "cxtime.h"
#include "cxmath.h"

using namespace std;

string CxUuid::createGuidString()
{
    return guidToString(createGuid());
}

string CxUuid::guidToString(const GUID &guid)
{
    char buf[64] = {0};
    #ifdef __GNUC__
        snprintf(
    #else // MSVC
        _snprintf_s(
    #endif
            buf,
            sizeof(buf),
             "%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
            guid.Data1, guid.Data2, guid.Data3,
            guid.Data4[0], guid.Data4[1],
            guid.Data4[2], guid.Data4[3],
            guid.Data4[4], guid.Data4[5],
            guid.Data4[6], guid.Data4[7]);
        return std::string(buf);
}

//AB8A5EDD-BD9A-5547-B2ED-6504035A761A
//{AB8A5EDD-BD9A-5547-B2ED-6504035A761A}
GUID CxUuid::guidFromString(const string &sGuidString)
{
    GUID rGuid;
    if (sGuidString.size()<36) return rGuid;
    const char * str = sGuidString.c_str();
    size_t iIndex = sGuidString.find('{');
    if (iIndex != string::npos) str += (iIndex + 1);
    /* parse hex values of "time" parts */
    rGuid.Data1 = strtoul(str,    NULL, 16);
    rGuid.Data2 = strtoul(str+9,  NULL, 16);
    rGuid.Data3 = strtoul(str+14, NULL, 16);

    /* parse hex values of "clock" parts */
    int tmp16 = strtoul(str+19, NULL, 16);

    long long unsigned int tmp64;
    sscanf(str+24, "%llx", &tmp64);
//    long long tmp64 = strtoul(str+24, NULL, 16);
    rGuid.Data4[1] = tmp16 & 0xff;
    rGuid.Data4[0] = (tmp16 >> 8) & 0xff;
    rGuid.Data4[7] = tmp64 & 0xff;
    rGuid.Data4[6] = (tmp64 >>  8) & 0xff;
    rGuid.Data4[5] = (tmp64 >> 16) & 0xff;
    rGuid.Data4[4] = (tmp64 >> 24) & 0xff;
    rGuid.Data4[3] = (tmp64 >> 32) & 0xff;
    rGuid.Data4[2] = (tmp64 >> 40) & 0xff;

    return rGuid;
}

string CxUuid::createRandString()
{
    string r = CxString::toString(CxTime::currentSystemTime());

    int i1 = (CxMath::random(255) << 24) & 0xFF000000;
    int i2 = (CxMath::random(255) << 24) & 0x00FF0000;
    int i3 = (CxMath::random(255) << 24) & 0x0000FF00;
    int i4 = (CxMath::random(255) << 24) & 0x000000FF;
    uint32 iRand = i1 + i2 + i3 + i4;

    char str[10+1];
    sprintf(str, "%010u", iRand);

    return r + string(str);
}


#ifdef GM_OS_WIN
#include <objbase.h>

GUID CxUuid::createGuid()
{
    GUID guid;
    CoCreateGuid(&guid);
    return guid;
}

#elif defined(__APPLE__)
#include <CoreFoundation/CFUUID.h>

GUID CxUuid::createGuid()
{
    auto newId = CFUUIDCreate(NULL);
    auto bytes = CFUUIDGetUUIDBytes(newId);
    CFRelease(newId);

    const unsigned char byteArray[16] =
    {
        bytes.byte0,
        bytes.byte1,
        bytes.byte2,
        bytes.byte3,
        bytes.byte4,
        bytes.byte5,
        bytes.byte6,
        bytes.byte7,
        bytes.byte8,
        bytes.byte9,
        bytes.byte10,
        bytes.byte11,
        bytes.byte12,
        bytes.byte13,
        bytes.byte14,
        bytes.byte15
    };
    return *((GUID *)byteArray);
}

#elif defined(__ANDROID__)
#include <jni.h>

GUID CxUuid::createGuid(JNIEnv *env)
{
    jclass _uuidClass = env->FindClass("java/util/UUID");
    jmethodID _newGuidMethod = env->GetStaticMethodID(_uuidClass, "randomUUID", "()Ljava/util/UUID;");
    jmethodID _mostSignificantBitsMethod = env->GetMethodID(_uuidClass, "getMostSignificantBits", "()J");
    jmethodID _leastSignificantBitsMethod = env->GetMethodID(_uuidClass, "getLeastSignificantBits", "()J");

    jobject javaUuid = _env->CallStaticObjectMethod(_uuidClass, _newGuidMethod);
    jlong mostSignificant = _env->CallLongMethod(javaUuid, _mostSignificantBitsMethod);
    jlong leastSignificant = _env->CallLongMethod(javaUuid, _leastSignificantBitsMethod);

    unsigned char bytes[16] =
    {
        (mostSignificant >> 56) & 0xFF,
        (mostSignificant >> 48) & 0xFF,
        (mostSignificant >> 40) & 0xFF,
        (mostSignificant >> 32) & 0xFF,
        (mostSignificant >> 24) & 0xFF,
        (mostSignificant >> 16) & 0xFF,
        (mostSignificant >> 8) & 0xFF,
        (mostSignificant) & 0xFF,
        (leastSignificant >> 56) & 0xFF,
        (leastSignificant >> 48) & 0xFF,
        (leastSignificant >> 40) & 0xFF,
        (leastSignificant >> 32) & 0xFF,
        (leastSignificant >> 24) & 0xFF,
        (leastSignificant >> 16) & 0xFF,
        (leastSignificant >> 8) & 0xFF,
        (leastSignificant) & 0xFF,
    };
    return *((GUID *)bytes);
}

#else
#include <uuid/uuid.h>

//*centos
//yum install libuuid libuuid-devel

GUID CxUuid::createGuid()
{
    GUID guid;
    uuid_generate(reinterpret_cast<unsigned char *>(&guid));
    return guid;
}

#endif

