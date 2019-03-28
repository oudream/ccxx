#include "cxbuffer.h"

using namespace std;

typedef union u{
    double v;
    unsigned char s[8];
} U;


void CxBuffer::setBitTrue(uchar *data, uchar n)
{
    *data = (*data) | ( 1 << n );
}

void CxBuffer::setBitFalse(uchar *data, uchar n)
{
    *data = (*data) & ( ~( 1 << n ) );
}

uchar CxBuffer::getBit(const uchar &data, uchar n)
{
    return ( data >> n) & 0x01;
}

int CxBuffer::divBig(int iQuantity, int iMod)
{
    int iResult = iQuantity / iMod;
    if (iQuantity % iMod > 0) iResult ++;
    return iResult;
}

int CxBuffer::index(uchar mod, int address, uchar * indexMod, int beginAddress)
{
    if (address < beginAddress) return -1;
    int qantity = address - beginAddress;
    int result = qantity / mod;
    uchar iIndexMod = qantity % mod;
    if (indexMod) *indexMod = iIndexMod;
    return result;
}

int CxBuffer::count(int iQuantity, int iMod)
{
    int iResult = iQuantity / iMod;
    if (iQuantity % iMod > 0) iResult ++;
    return iResult;
}

vector<uchar> CxBuffer::toBuffer(const vector<bool> &values)
{
    int iLength = divBig(values.size(), 8);
    vector<uchar> data(iLength, 0);
    int iIndex = -1;
    for (size_t i = 0; i < values.size(); ++i)
    {
        int n = i % 8;
        if (n == 0) iIndex ++;
        uchar dataValue = data[ iIndex ];
        if (values.at(i)) {
            setBitTrue(&dataValue, n);
        }
    }
    return data;
}

vector<uchar> CxBuffer::toBuffer(const map<int, int> &addressValues, int iMinAddress)
{
    int iLength = divBig(addressValues.size(), 8);
    vector<uchar> data(iLength, 0);
    int iIndex;
    uchar indexMod;
    map<int, int>::const_iterator it = addressValues.begin();
    while (it != addressValues.end()) {
        if (it->second != 0) {
            iIndex = index(8, it->first, &indexMod, iMinAddress);
            if (iIndex > 0 && iIndex < iLength) {
                uchar value = data[iIndex];
                setBitTrue(&value, indexMod);
                data[ iIndex ] = value;
            }
        }
        it ++;
    }
    return data;
}

vector<uchar> CxBuffer::toBuffer(const map<int, int> &addressValues, int iMinAddress, int iMaxAddress)
{
    vector<uchar> data;
    if (iMaxAddress > iMinAddress && iMinAddress >= 0)
    {
        int iLength = divBig(iMaxAddress - iMinAddress + 1, 8);
        data.resize(iLength, 0);
        int iIndex;
        uchar indexMod;
        map<int, int>::const_iterator it = addressValues.begin();
        while (it != addressValues.end()) {
            if (it->second != 0) {
                iIndex = index(8, it->first, &indexMod, iMinAddress);
                if (iIndex >= 0 && iIndex < iLength) {
                    uchar value = data[iIndex];
                    setBitTrue(&value, indexMod);
                    data[ iIndex ] = value;
                }
            }
            it ++;
        }
    }
    return data;
}

vector<uchar> CxBuffer::toBuffer(const map<int, double> &addressValues, int iMinAddress)
{
    int iCount = addressValues.size();
    vector<uchar> data;
    data.resize(iCount * 2);
    map<int, double>::const_iterator it = addressValues.begin();
    while (it != addressValues.end()) {
        int iIndex = it->first - iMinAddress;
        ushort value = it->second;
        if (iIndex > 0 && iIndex < iCount) {
            data[ iIndex * 2 ] = value >> 8;
            data[ iIndex * 2 + 1 ] = value;
        }
        it ++;
    }
    return data;
}

void CxBuffer::set2Bit(uchar *data, uchar n, uchar value)
{
    *data = ( (*data) & ( ~ (0x03 << n) ) ) | (value << n);
}

uchar CxBuffer::get2Bit(const uchar& data, uchar n)
{
    return ( data >> n) & 0x03;
}
////////////////////////////////////////////////////////////////////////////////
bool CxBuffer::fromBuf(uint8 *pBuf, int len, uint16 &val, int mode)
{
    uchar * data = (uchar *)pBuf;
    if((data==NULL)||len<(sizeof(uint16))) return false;

    switch(mode)
    {
        case 0://高字节在前，低字节在后
            val = ((data[1] & 0xFF) | (data[0]<<8));
            break;
        case 1://低字节在前，高字节在后
            val = ((data[0] & 0xFF) | (data[1]<<8));
            break;
        default: //1
            val = ((data[0] & 0xFF) | (data[1]<<8));
            break;
    }

    return true;
}
int CxBuffer::toBuf(uint8 *pBuf, uint16 val, int mode)
{
    switch(mode)
    {
        case 0://高字节在前，低字节在后
            *pBuf++ = val>>8;
            *pBuf   = val&0xff;
            break;
        case 1: //低字节在前，高字节在后
            *pBuf++ = val&0xff;
            *pBuf   = val>>8;
            break;
        default: //1
            *pBuf++ = val&0xff;
            *pBuf   = val>>8;
            break;
    }
    return sizeof(uint16);
}

bool CxBuffer::fromBuf(uint8 *pBuf, int len, int16 &val, int mode)
{
    uchar * data = (uchar *)pBuf;
    if((data==NULL)||len<(sizeof(int16))) return false;

    switch(mode)
    {
        case 0:
            val = (data[1]& 0xFF) | (data[0]<<8);
            break;
        case 1:
            val = (data[0]& 0xFF) | (data[1]<<8);
            break;
        default:
            val = (data[0]& 0xFF) | (data[1]<<8);
            break;
    }
    return true;
}

int CxBuffer::toBuf(uint8 *pBuf, int16 val, int mode)
{
    return toBuf(pBuf,(uint16)val,mode);
}

//-----------------------------------------------------------------------
bool CxBuffer::fromBuf(uint8 *pBuf, int len, uint32 &val, int mode)
{
    uchar * data = (uchar *)pBuf;
    if((data==NULL)||len<(sizeof(uint32))) return false;

    switch(mode)
    {
        case 0: //L0L1H0H1
            val = ((data[0]&0xFF)|(data[1]<<8))|((data[2]|(data[3]<<8))<<16);
            break;
        case 1://L1L0H1H0
            val = ((data[1]&0xFF)|(data[0]<<8))|((data[3]|(data[2]<<8))<<16);
            break;
        case 2://H0H1L0L1
            val = (((data[1]<<8)|(data[0]&0xFF))<<16)|((data[3]<<8)|(data[2]&0xFF));
            break;
        case 3://H1H0L1L0
            val = (((data[0]<<8)|(data[1]&0xFF))<<16)|((data[2]<<8)|(data[3]&0xFF));
            break;
        default:
            val = ((data[0]&0xFF)|(data[1]<<8))|((data[2]|(data[3]<<8))<<16);
            break;
    }
    return true;
}
int CxBuffer::toBuf(uint8 *pBuf, uint32 val, int mode)
{
    switch(mode)
    {
        case 0://L0L1 H0H1
            *pBuf++ = val&0xff;
            *pBuf++ = val>>8;
            *pBuf++ = val>>16;
            *pBuf   = val>>24;
            break;
        case 1: //L1L0H1H0
            *pBuf++ = val>>8;
            *pBuf++ = val&0xFF;
            *pBuf++ = val>>24;
            *pBuf   = val>>16;
            break;
        case 2://H0H1L0L1
            *pBuf++ = val>>16;
            *pBuf++ = val>>24;
            *pBuf++ = val&0xFF;
            *pBuf = val>>8;
            break;
        case 3://H1H0L1L0
            *pBuf++ = val>>24;
            *pBuf++ = val>>16;
            *pBuf++ = val>>8;
            *pBuf = val&0xff;
            break;
        default: //1
            *pBuf++ = val&0xff;
            *pBuf++ = val>>8;
            *pBuf++ = val>>16;
            *pBuf   = val>>24;
            break;
    }
    return sizeof(uint32);
}
bool CxBuffer::fromBuf(uint8 *pBuf, int len, int32 &val, int mode)
{
    uchar * data = (uchar *)pBuf;
    if((data==NULL)||len<(sizeof(int32))) return false;

    switch(mode)
    {
        case 0: //L0L1 H0H1
            val = ((data[0]&0xFF)|(data[1]<<8))|((data[2]|(data[3]<<8))<<16);
            break;
        case 1://L1L0 H1H0
            val = ((data[1]&0xFF)|(data[0]<<8))|((data[3]|(data[2]<<8))<<16);
            break;
        case 2://H0H1 L0L1
            val = (((data[1]<<8)|(data[0]&0xFF))<<16)|((data[3]<<8)|(data[2]&0xFF));
            break;
        case 3://H1H0 L1L0
            val = (((data[0]<<8)|(data[1]&0xFF))<<16)|((data[2]<<8)|(data[3]&0xFF));
            break;
        default:
            val = ((data[0]&0xFF)|(data[1]<<8))|((data[2]|(data[3]<<8))<<16);
            break;
    }
    return true;
}


int CxBuffer::toBuf(uint8 *pBuf, int32 val, int mode)
{
    return toBuf(pBuf,(uint32)val,mode);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CxBuffer::fromBuf(uint8 *pBuf, int len, uint64 &val, int mode)
{
    bool bRet = false;

    return bRet;
}

int CxBuffer::toBuf(uint8 *pBuf, uint64 val, int mode)
{
    int pos = toBuf(pBuf,(uint32)(val&0xFFFFFFFF),mode);
    toBuf(pBuf+pos,(uint32)(val>>32),mode);
    return sizeof(uint64);
}

bool CxBuffer::fromBuf(uint8 *pBuf, int len, int64 &val, int mode)
{
    bool bRet = false;

    return bRet;
}

int CxBuffer::toBuf(uint8 *pBuf, int64 val, int mode)
{
    return toBuf(pBuf,(uint64)(val),mode);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
bool CxBuffer::fromBuf(uint8 *pBuf, int len, float &val, int mode)
{
    uchar * data = (uchar *)pBuf;
    if((data==NULL)||len<(sizeof(float))) return false;

    int32 nVal=0;
    switch(mode)
    {
        case 0://L0L1 H0H1
            nVal = (data[0]|(data[1]<<8)|(data[2]<<16)|(data[3]<<24));
            break;
        case 1://L1L0 H1H0 IEEE 454 sigle float , small end mode
            nVal = (data[1]|(data[0]<<8)|(data[3]<<16)|(data[2]<<24));
            break;
        case 2://H0H1 L0L1
            nVal = (data[2]|(data[3]<<8)|(data[0]<<16)|(data[1]<<24));
            break;
        case 3://H1H0 L1L0
            nVal = (data[3]|(data[2]<<8)|(data[1]<<16)|(data[0]<<24));
            break;
        default:
            nVal = (data[0]|(data[1]<<8)|(data[2]<<16)|(data[3]<<24));
            break;
    }
    val = *(float*)&nVal;
    return true;
}
int CxBuffer::toBuf(uint8 *pBuf, float val, int mode)
{
    uchar *p = (uchar*)&val;

    switch(mode)
    {
        case 0:
            *pBuf++ = p[0];
            *pBuf++ = p[1];
            *pBuf++ = p[2];
            *pBuf   = p[3];
            break;
        case 1://IEEE 454 sigle float , small end 's mode
            *pBuf++ = p[1];
            *pBuf++ = p[0];
            *pBuf++ = p[3];
            *pBuf   = p[2];
            break;
        case 2:
            *pBuf++ = p[2];
            *pBuf++ = p[3];
            *pBuf++ = p[0];
            *pBuf   = p[1];
            break;
        case 3://H1H0 L1L0
            *pBuf++ = p[3];
            *pBuf++ = p[2];
            *pBuf++ = p[1];
            *pBuf   = p[0];
            break;
        default:
            *pBuf++ = p[0];
            *pBuf++ = p[1];
            *pBuf++ = p[2];
            *pBuf   = p[3];
            break;
    }
    return sizeof(float);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CxBuffer::fromBuf(uint8 *pBuf, int len, double &val, int mode)
{
    uchar * data = (uchar *)pBuf;
    if((data==NULL)||len<(sizeof(double))) return false;

    bool bRet = true;

    u a;

    int64 nVal=0;
    switch(mode)
    {
        case 0:
            memcpy(&a.s[0],pBuf,sizeof(double));
            val = a.v;
            break;
        case 1:// stand order byte
            a.s[0] = pBuf[1];
            a.s[1] = pBuf[0];
            a.s[2] = pBuf[3];
            a.s[3] = pBuf[2];
            a.s[4] = pBuf[5];
            a.s[5] = pBuf[4];
            a.s[6] = pBuf[7];
            a.s[7] = pBuf[6];
            val = a.v;
            break;
        default:
            val = *(double*)pBuf;
            break;
    }

    return bRet;
}
int CxBuffer::toBuf(uint8 *pBuf, double val, int mode)
{
    u a;
    a.v = val;
    switch(mode)
    {
        case 0:
            memcpy(pBuf,a.s,sizeof(double));
            break;
        case 1:
            *pBuf++ = a.s[1];
            *pBuf++ = a.s[0];
            *pBuf++ = a.s[3];
            *pBuf++ = a.s[2];
            *pBuf++ = a.s[5];
            *pBuf++ = a.s[4];
            *pBuf++ = a.s[7];
            *pBuf   = a.s[6];
            break;
        default:
            memcpy(pBuf,a.s,sizeof(double));
            break;
    }
    return sizeof(double);
}
///////////////////////////////////////////////////////////////////////////////////
int CxBuffer::toBuf(uint8 *pBuf, int64 val, string fmt, int mode)
{
    int ret = 0;
    if (fmt == string("BYTE") || fmt == string("UINT8"))
    {
        *pBuf = (BYTE)val;
        ret = sizeof(BYTE);
    }
    else if(fmt == string("CHAR")|| fmt == string("INT8"))
    {
        *pBuf = (char)val;
        ret = sizeof(char);
    }
    else if(fmt == string("SHORT")|| fmt == string("INT16"))
    {
        ret = toBuf(pBuf,(int16)val,mode);
    }
    else if(fmt == string("WORD")|| fmt == string("UINT16"))
    {
        ret = toBuf(pBuf,(uint16)val,mode);
    }
    else if(fmt == string("INT")|| fmt == string("INT32"))
    {
        ret = toBuf(pBuf,val,mode);
    }
    else if(fmt == string("DWORD")|| fmt == string("UINT32"))
    {
        ret = toBuf(pBuf,(uint32)val,mode);
    }
    else if(fmt == string("FLOAT"))
    {
        ret = toBuf(pBuf,(float)val,mode);
    }
    else if(fmt == string("DOUBLE"))
    {
        ret = toBuf(pBuf,(double)val,mode);
    }
    return ret;
}

int CxBuffer::toBuf(uint8 *pBuf, double val, std::string fmt, int mode)
{
    int ret = 0;
    if (fmt == string("BYTE") || fmt == string("UINT8"))
    {
        *pBuf = (BYTE)val;
        ret = sizeof(BYTE);
    }
    else if(fmt == string("CHAR")|| fmt == string("INT8"))
    {
        *pBuf = (char)val;
        ret = sizeof(char);
    }
    else if(fmt == string("SHORT")|| fmt == string("INT16"))
    {
        ret = toBuf(pBuf,(int16)val,mode);
    }
    else if(fmt == string("WORD")|| fmt == string("UINT16"))
    {
        ret = toBuf(pBuf,(uint16)val,mode);
    }
    else if(fmt == string("INT")|| fmt == string("INT32"))
    {
        ret = toBuf(pBuf,val,mode);
    }
    else if(fmt == string("DWORD")|| fmt == string("UINT32"))
    {
        ret = toBuf(pBuf,(uint32)val,mode);
    }
    else if(fmt == string("FLOAT"))
    {
        ret = toBuf(pBuf,(float)val,mode);
    }
    else if(fmt == string("DOUBLE"))
    {
        ret = toBuf(pBuf,val,mode);
    }
    return ret;
}

int CxBuffer::toBuf(uint8 *pBuf, std::string val, std::string fmt, int mode)
{
    int ret = 0;
    if(fmt == string("STRING")) //标准模式
    {
        ret = val.size();
        memcpy(pBuf,val.data(),val.size());
    }
    else if (fmt == string("STRING_S7")) //S7 模式
    {
        ret = val.size();
        if (ret > 0xFD) ret = 0xFD;
        pBuf[0] = 0xFE;
        pBuf[1] = (BYTE)ret;
        memcpy(&pBuf[2], val.data(), ret);
        ret += 2;
    }
    return ret;
}
