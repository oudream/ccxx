#ifndef CXCRC_H
#define CXCRC_H

#include "cxglobal.h"

class GM_CCXX_CORE_API CxCrc
{
public:
    static uchar calcsum8(const uchar* pData, int iLength);

    static ushort calcsum16(const uchar* pData, int iLength);

    // check table style
    static uchar calcCRC8(const uchar* pData, int iLength);

    // check table style
    static ushort calcCRC16(const uchar* pData, int iLength);

    static short calcCRC16(const char* pData, int iLength);

    //check table style
    static ushort calcModbus(const uchar* pData, int iLength);

    static std::string md5HexCode(const std::string& sData);

    static std::string md5HexCode(const std::vector<std::string>& sLines);

    static std::vector<char> md5(const std::vector<std::string>& sLines);

    static std::vector<char> md5(const char* pData, int iLength);

    /**
     * @brief md5
     * @param sFilePath
     * @param iCodeType : 0=data ; 1=hex
     * @return : empty=file is empty or error
     */
    static std::string file2md5(const std::string &sFilePath, int iCodeType = 1);

    static bool isSameMd5FileData(const std::string &sFilePath1, const std::string &sFilePath2);

    static uchar calcXOR(const uchar* pData, int iLength);

    // formula style
    static uchar crc8(const uchar * ptr, int len);

    static unsigned int calculate_crc16_2(unsigned char *ptr, unsigned char len);

    // simple encrypt
    static std::string encodeXorFix(const char* pData, int iDataLength, const char* pKey, int iKeyLength);

    static std::string decodeXorFix(const char* pData, int iDataLength, const char* pKey, int iKeyLength);

    static std::string encodeXorFix(const char* pData, int iLength);

    static std::string decodeXorFix(const char* pData, int iLength);

};

#endif // CXCRC_H
