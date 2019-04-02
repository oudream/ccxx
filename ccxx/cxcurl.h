#ifndef CXCURL_H
#define CXCURL_H

#include "cxglobal.h"
#include <curl/curl.h>


#if defined(GM_BUILD_CCXX_CURL_LIB)
#define GM_CCXX_CURL_API  GM_DECL_EXPORT
#elif defined(GM_BUILD_CCXX_CURL_CODE)
#define GM_CCXX_CURL_API
#else
#define GM_CCXX_CURL_API  GM_DECL_IMPORT
// GM_BUILD_CCXX_CURL_INCLUDE
#endif


class GM_CCXX_CURL_API CxCurlFtp {
public:
    CxCurl(int iPort=-1, const std::string & sUserPwd = std::string(), const std::string & sDefaultLocalPath = std::string());

    ~CxCurl();

    int get(const std::string & sUrl, const std::string & sSaveFilePath = std::string());

    int put(const std::string & sLocalFilePath, const std::string & sUrl);

};

#endif //CXCURL_H
