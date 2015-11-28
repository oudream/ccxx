#ifndef CXCRC_H
#define CXCRC_H

#include "cxglobal.h"

class CxCrc
{
public:
    //8位和校验
    static uchar calcsum8(const uchar* pData, int iLength);

    //16位和校验
    static ushort calcsum16(const uchar* pData, int iLength);

    //查表法计算CRC8多项式校验
    static uchar calcCRC8(const uchar* pData, int iLength);

    //查表法计算CRC16多项式校验
    static ushort calcCRC16(const uchar* pData, int iLength);

    static short calcCRC16(const char* pData, int iLength);

    //查表法计算Modubus多项式校验
    static ushort calcModbus(const uchar* pData, int iLength);

    //MD5校验
    static std::string md5HexCode(const std::string& sData);

    static std::string md5HexCode(const std::vector<std::string>& sLines);

    static std::vector<char> md5(const std::vector<std::string>& sLines);

    static std::vector<char> md5(const char* pData, int iLength);

    //异或校验
    static uchar calcXOR(const uchar* pData, int iLength);
    //公式法计算CRC8多项式校验
    static uchar crc8(const uchar * ptr, int len);

    static unsigned int calculate_crc16_2(unsigned char *ptr, unsigned char len);

    //简单加密与解密
    static std::string encodeXorFix(const char* pData, int iDataLength, const char* pKey, int iKeyLength);

    static std::string decodeXorFix(const char* pData, int iDataLength, const char* pKey, int iKeyLength);

    static std::string encodeXorFix(const char* pData, int iLength);

    static std::string decodeXorFix(const char* pData, int iLength);

};

#endif // CXCRC_H
