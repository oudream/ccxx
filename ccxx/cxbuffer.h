#ifndef CXBUFFER_H
#define CXBUFFER_H

#include "cxglobal.h"

class GM_CCXX_CORE_API CxBuffer
{
public:
    ///
    /// \brief setBitTrue
    /// \param data
    /// \param n : index of data ; size_of( * data ) <= 32
    ///
    static void setBitTrue(uchar* data, uchar n);

    static void setBitFalse(uchar* data, uchar n);

    static uchar getBit(const uchar& data, uchar n);

    static void set2Bit(uchar* data, uchar n, uchar value);

    static uchar get2Bit(const uchar& data, uchar n);

    static int divBig(int iQuantity, int iMod);

    static int index(uchar mod, int address, uchar * indexMod = NULL, int beginAddress = 0);

    static int count(int iQuantity, int iMod);

    static std::vector<uchar> toBuffer(const std::vector<bool>& values);

    static std::vector<uchar> toBuffer(const std::map<int, int>& addressValues, int iMinAddress);

    static std::vector<uchar> toBuffer(const std::map<int, int>& addressValues, int iMinAddress, int iMaxAddress);

    static std::vector<uchar> toBuffer(const std::map<int, double>& addressValues, int iMinAddress);

    template<typename T>
    static bool isContinueAddress(const std::map<int, T>& addressValues, int* pMinAddress = NULL, int* pMaxAddress = NULL)
    {
        int iMaxAddress = (std::numeric_limits<int>::max)();
        int iMinAddress = (std::numeric_limits<int>::min)();
        for (typename std::map<int, T>::const_iterator it = addressValues.begin(); it != addressValues.end(); ++it)
        {
            int iAddress = it->first;
            if (iAddress > iMaxAddress) {
                iMaxAddress = iAddress;
            }
            if (iAddress < iMinAddress) {
                iMinAddress = iAddress;
            }
        }
        if (pMinAddress) *pMinAddress = iMinAddress;
        if (pMaxAddress) *pMaxAddress = iMaxAddress;
        return (iMaxAddress - iMinAddress == addressValues.size() - 1);
    }

    static int toBuf(uint8 *pBuf,uint16 val,int mode = 1);
    static int toBuf(uint8 *pBuf,int16 val,int mode = 1);
    static int toBuf(uint8 *pBuf,uint32 val,int mode = 1);
    static int toBuf(uint8 *pBuf,int32 val,int mode = 1);
    static int toBuf(uint8 *pBuf,uint64 val,int mode = 1);
    static int toBuf(uint8 *pBuf,int64 val,int mode = 1);
    static int toBuf(uint8 *pBuf,float val,int mode = 1);
    static int toBuf(uint8 *pBuf,double val,int mode = 1);


    static bool fromBuf(uint8 *pBuf,int len,uint16 &val,int mode = 1);
    static bool fromBuf(uint8 *pBuf,int len,int16 &val,int mode = 1);
    static bool fromBuf(uint8 *pBuf,int len,uint32 &val,int mode = 1);
    static bool fromBuf(uint8 *pBuf,int len,int32 &val,int mode = 1);
    static bool fromBuf(uint8 *pBuf,int len,uint64 &val,int mode = 1);
    static bool fromBuf(uint8 *pBuf,int len,int64 &val,int mode = 1);

    static bool fromBuf(uint8 *pBuf,int len,float &val,int mode=0);
    static bool fromBuf(uint8 *pBuf,int len,double &val,int mode=0);

};

#endif // CXBUFFER_H
