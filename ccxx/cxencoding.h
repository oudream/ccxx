#ifndef CXENCODING_H
#define CXENCODING_H

#include "cxglobal.h"

class GM_CCXX_CORE_API CxEncoding
{
public:
    static std::string gb2312ToUtf8(const std::string & sSource);

    static std::string utf8ToGb2312(const std::string & sSource);

    static std::vector<std::string> gb2312ToUtf8(const std::vector<std::string> & sSources);

    static std::vector<std::string> utf8ToGb2312(const std::vector<std::string> & sSources);

    static std::string base64Encode(const unsigned char *str, int length);

    static std::string base64Decode(const unsigned char *str, int length);

    static std::string base64Encode(const std::string & sSource);

    static std::string base64Decode(const std::string & sSource);

    static bool hasChinese(const std::string & sSource);

    static bool isUtf8(const void* pBuffer, long size);

};

#endif // CXENCODING_H
