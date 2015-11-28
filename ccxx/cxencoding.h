#ifndef CXENCODING_H
#define CXENCODING_H

#include "cxglobal.h"

class CxEncoding
{
public:
    static std::string gb2312ToUtf8(const std::string & sSource);

    static std::string utf8ToGb2312(const std::string & sSource);

    static std::vector<std::string> gb2312ToUtf8(const std::vector<std::string> & sSources);

    static std::vector<std::string> utf8ToGb2312(const std::vector<std::string> & sSources);

};

#endif // CXENCODING_H
