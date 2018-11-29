#include "iec104protocol.h"

IEC104Protocol::IEC104Protocol(SendSocket *oSendSocket)
    : m_oSendSocket(oSendSocket)
{
}

IEC104Protocol::~IEC104Protocol()
{
}

void IEC104Protocol::ProtocolAfterStart()
{
    FRecBufState = 0;
    FRecBufLen = 0;
    FRecBufIndex = 0;
    FRecTypeFlag = 0;
    FCalcerS = 0;
    FCalcerR = 0;
    _started = false;
    _tested = true;

    _lastReceiveTime = 0;
    _lastSendTime = 0;
    _lastDataTime = 0;

    _timeCallUp->StartTimer();
    _timeCheck->StartTimer();
}

void IEC104Protocol::ProtocolAfterStop()
{
    _timeCallUp->StopTimer();
    _timeCheck->StopTimer();
}

void IEC104Protocol::ProtocolReceivedData(const unsigned char *pData, int iLength)
{
    for (int i = 0; i < iLength; ++i)
    {
        switch (FRecBufState)
        {
            case 0:
                if (pData[i] == 0x68)
                {
                    FRecBufState ++;
                }
                break;
            case 1:
            {
                FRecBufLen = pData[i] + 2;
                if ( (FRecBufLen >= 6) & (FRecBufLen <= IEC104BUFLEN) )
                {
                    FRecBuf[0] = 0x68;
                    FRecBuf[FRecBufState] = pData[i];
                    FRecBufState ++;
                    FRecBufIndex = FRecBufState;
                }
                else
                    FRecBufState = 0;
            }
                break;
            case 2:
            {
                FRecBuf[FRecBufIndex] = pData[i];
                FRecBufIndex ++;
                if (FRecBufIndex >= FRecBufLen)
                {
                    DealData();
                    FRecBufState = 0;
                }
            }
                break;
        }
    }
}


void IEC104Protocol::ProtocolTimerTimeOut(const long long & dt)
{
    if (dt - _lastCallUpTime >= 10000)
    {
        StartCallUp();
    }
//    if (oTimer == _timeCallUp)
//    {
//        if (! _started)
//        {
//            SendStart();
//        }
//        else
//        {
//            StartCallUp();
//        }
//    }

//    if (oTimer == _timeCheck)
//    {
//        ChannelCheck();
//    }
}

void IEC104Protocol::StartCallUp()
{
    int stationId = 0;
    SendCallUp(6, stationId);
}


void IEC104Protocol::StopCallUp()
{
    int stationId = 0;
    SendCallUp(8, stationId);
}


void IEC104Protocol::ChannelCheck()
{
    if (DataTime::MilliSecToNow(_lastReceiveTime) > (_checkSecond * 1000 + _checkMicrosecond))
    {
        SendTest();
    }
    if (DataTime::MilliSecToNow(_lastSendTime) > (_checkSecond * 1000 + _callUpMicrosecond))
    {
        SendFrameS();
    }
}

int IEC104Protocol::SendCallUp(unsigned short reason, unsigned short stationId, int address)
{
    unsigned char data[16];

    data[6] = 0x64;
    data[7] = 0x01;
    data[8] = reason;
    data[9] = (reason >> 8);
    data[10] = stationId;
    data[11] = (stationId >> 8);
    data[12] = address;
    data[13] = (address >> 8);
    data[14] = (address >> 16);
    data[15] = 0x14;

    _lastCallUpTime = CxTime::currentSystemTime();
    return SendFrameI(data, 16);
}

int IEC104Protocol::SendFrameI(unsigned char* data, int length)
{
    if (! _started) return -1;
    int result = -1;
    if ( (length < 6) || (length > IEC104BUFLEN) )
        return result;

    data[0] = 0x68;
    data[1] = length - 2;
    data[2] = (FCalcerS * 2);
    data[3] = (FCalcerS * 2) >> 8;
    data[4] = (FCalcerR * 2);
    data[5] = (FCalcerR * 2) >> 8;
    result = SendBuffer(data, length);

    if (result > 0)
    {
        FCalcerS = FCalcerS + 1;
        if (FCalcerS > 0x7FFF)
            FCalcerS = 0;
    }

    return result;
}

int IEC104Protocol::WriteData(unsigned char* data, int length)
{
    return m_oSendSocket->sendData((char *)data, length);
}

int IEC104Protocol::SendBuffer(unsigned char* data, int length)
{
    int result = -1;

    result = WriteData(data, length);
    _lastSendTime = DataTime::Now();
    _lastDataTime = DataTime::Now();

    return result;
}

void IEC104Protocol::DealData()
{
    _lastReceiveTime = DataTime::Now();
    _lastDataTime = DataTime::Now();

    unsigned char aType;
    unsigned char APDU_length;
    aType = FRecBuf[2] & 0x03;

    if ( (aType == 0x00) || (aType == 0x02) )  // I frame
    {
        DealFrameI();
    }
    else if ( aType == 0x01 )  // S frame
    {
    }
    else if ( aType == 0x03 )  // U frame
    {
        APDU_length = FRecBuf[2];

        switch (APDU_length)
        {
        // 0000 0111 STARTDT ACT
        case 0x07:
        {
        }
            break;

        // 0000 1011 STARTDT CON
        case 0x0B:
        {
            _started = true;
            ResetCalcer();
        }
            break;

        // 0001 0011 STOPDT ACT
        case 0x13:
        {
        }
            break;

        // 0010 0011 STDPDT CON
        case 0x23:
        {
            _started = false;
            ResetCalcer();
        }
            break;

        // 0100 0011 TESTFR ACT
        case  0x43:
        {
            SendTestConfirmed();
            _tested = false;
        }
            break;

        //1000 0011 TESTFR CON
        case 0x83:
        {
            _tested = false;
        }
            break;

        }
    }
}

void IEC104Protocol::DealFrameI()
{
    FCalcerS = ( FRecBuf[4] + (FRecBuf[5] << 8) ) / 2;
    FCalcerR = ( FRecBuf[2] + (FRecBuf[3] << 8) ) / 2;
    FCalcerR = FCalcerR + 1;
    if ( FCalcerR > 0x7FFF )
        FCalcerR = 0;

    FRecTypeFlag = FRecBuf[6]; //类型标识
    bool bNeedSure = true;

    switch (FRecTypeFlag)
    {
    //遥信
    case 1: case 3: case 30: case 31:
    {
        DealYX();
    }
        break;

    //遥测
    case 9: case 11: case 13: case 34: case 35: case 36:
    {
        DealYC();
    }
        break;

    case 15: case 37:
    {
        DealTotal();
    }
        break;

    //遥控
    case 45: case 46: case 58: case 59:
    {
        DealYK();
    }
        break;

    //总召
    case 100:
    {
        bNeedSure = false;
    }
        break;

    }

    if (bNeedSure)
    {
        SendFrameS();
    }

}

void IEC104Protocol::ResetCalcer()
{
    FCalcerR = 0;
    FCalcerS = 0;
    FRecBufIndex = 0;
}

int IEC104Address(const unsigned char * pData)
{
    int iAddress = ( * ( (int *) pData ) ) & 0xFFFFFF;
    return iAddress;
}

void IEC104Protocol::DealYX()
{
    unsigned char typeFlag = FRecBuf[6]; //类型标识
    unsigned char count = FRecBuf[7] & 0x7F; //可变结构限定词
    bool isContinueAddress = ( (FRecBuf[7] & 0x80) == 0x80 ) ? true : false; //可变结构限定词
    unsigned short reason = (FRecBuf[9] << 8) + FRecBuf[8]; //传送原因
    unsigned short stationId = (FRecBuf[11] << 8) + FRecBuf[10]; //公共地址

//    if ( ! _yxRecTable->StationIds().contains(stationId) )
//    {
//        GFunc_OutDebug(QString("[%1] - [DealYX] : Common Address [%2] Is Not Exist").arg(ProtocolName()).arg(stationId));
//        return;
//    }

//    QHash<int, int> addressValues;
    for (int i = 0; i < count; i++)
    {
        int address = 0;
        unsigned char state = 0xFF;

        switch (typeFlag)
        {
        case 1: //单点
        {
            if (! isContinueAddress)
            {
                if ( (15 + i * 4) > IEC104BUFLEN - 1 )
                    break;
                address = IEC104Address(& (FRecBuf[12 + i * 4]));
                state = FRecBuf [15 + i * 4];
            }
            else
            {
                if ( (15 + i) > IEC104BUFLEN - 1 )
                    break;
                address = IEC104Address(& (FRecBuf[12])) + i;
                state = FRecBuf [15 + i];
            }
        }
            break;

        case 3: //双点
        {
            if ( !isContinueAddress )
            {
                if ( (15 + i * 4) > IEC104BUFLEN - 1 )
                    break;
                address = IEC104Address(& (FRecBuf[12 + i * 4]));
                state = FRecBuf[15 + i * 4];
            }
            else
            {
                if ( (15 + i) > IEC104BUFLEN - 1 )
                    break;
                address = IEC104Address(& (FRecBuf[12])) + i;
                state = FRecBuf[15 + i];
            }
        }
            break;

        case 30: //带时标的单点
        {
        }
            break;

        case 31: //带时标的双点
        {
        }
            break;

        }

//        addressValues[address] = state;
    }

//    if (addressValues.count()>0)
//    {
//        _yxRecTable->ReceiveAddressValues(stationId, addressValues, ProtocolId(), 0, IsAutoCreateWorkerAtDeal());
//    }
}

unsigned short IEC104Value2(const unsigned char* pData)
{
    unsigned short value = * ( (unsigned short *) pData );
    return value;
}

float IEC104Value4(const unsigned char* pData)
{
    float value = * ( (float *) pData );
    return value;
}

void IEC104Protocol::DealYC()
{
    /*
    遥测规一化、标度化的数据结构
    ( 3 + [3] * N )        连续 不带时标
    ( [3 + 3] * N )        不连续 不带时标
    ( 3 + [3 + 7] * N )    连续 带时标
    ( [3 + 3 + 7] * N )    不连续 带时标

    遥测短浮点的数据结构
    ( 3 + [5] * N )        连续 不带时标
    ( [3 + 5] * N )        不连续 不带时标
    ( 3 + [5 + 7] * N )    连续 带时标
    ( [3 + 5 + 7] * N )    不连续 带时标
    */

    unsigned char typeFlag = FRecBuf[6]; //类型标识
    unsigned char count = FRecBuf[7] & 0x7F; //可变结构限定词
    bool isContinueAddress = ( (FRecBuf[7] & 0x80 ) == 0x80 ) ? true : false; //可变结构限定词
    unsigned short reason = (FRecBuf[9] << 8) + FRecBuf[8]; //传送原因
    unsigned short stationId = ( FRecBuf[11] << 8 ) + FRecBuf[10]; //公共地址

//    if ( ! _ycRecTable->StationIds().contains( stationId ) )
//    {
//        GFunc_OutDebug(QString("[%1] - [DealYC] : Common Address [%2] Is Not Exist").arg(ProtocolName()).arg(stationId));
//        return;
//    }

//    QHash<int, double> addressValues;
    for (int i = 0; i < count; i++)
    {
        int address = 0;
        double value = 0;
        unsigned char quality = 0xFF;
        switch (typeFlag)
        {
        case 9: //规一化值
        {
            if (! isContinueAddress)
            {
                if ( (12 + (i + 1) * 6) > IEC104BUFLEN - 1 )
                    break;
                address = IEC104Address(& (FRecBuf[12 + i * 6]));
                value = IEC104Value2(& ( FRecBuf[15 + i * 6]));
                quality = FRecBuf[17 + i * 6];
            }
            else
            {
                if ( (15 + (i + 1) * 3) > IEC104BUFLEN - 1 )
                    break;
                address = IEC104Address(& (FRecBuf[12])) + i;
                value = IEC104Value2(& ( FRecBuf[15 + i * 3]));
                quality = FRecBuf[17 + i * 3];
            }
        }
            break;

        case 11: //标度化值
        {
            if (! isContinueAddress)
            {
                if ( (12 + (i + 1) * 6) > IEC104BUFLEN - 1 )
                    break;
                address = IEC104Address(& (FRecBuf[12 + i * 6]));
                value = IEC104Value2(& ( FRecBuf[15 + i * 6]));
                quality = FRecBuf[17 + i * 6];
            }
            else
            {
                if ( (15 + (i + 1) * 3) > IEC104BUFLEN - 1 )
                    break;
                address = IEC104Address(& (FRecBuf[12])) + i;
                value = IEC104Value2(& ( FRecBuf[15 + i * 3]));
                quality = FRecBuf[17 + i * 3];
            }
        }
            break;

        case 13: //短浮点值
        {
            if (! isContinueAddress)
            {
                if ( (12 + (i + 1) * 8) > IEC104BUFLEN - 1 )
                    break;
                address = IEC104Address(& (FRecBuf[12 + i * 8]));
                value = IEC104Value4(& (FRecBuf[15 + i * 8]));
                quality = FRecBuf[19 + i * 8];
            }
            else
            {
                if ( (15 + (i + 1) * 5) > IEC104BUFLEN - 1 )
                    break;
                address = IEC104Address(& (FRecBuf[12])) + i;
                value = IEC104Value4(& (FRecBuf[15 + i * 5]));
                quality = FRecBuf[19 + i * 5];
            }
        }
            break;

        case 34: //带时标的规一化值
        {
        }
            break;

        case 35: //带时标的标度化值
        {
        }
            break;

        case 36: //带时标的短浮点值
        {
        }
            break;

        }

//        addressValues[address] = value;
    }

//    if (addressValues.count()>0){
//        _ycRecTable->ReceiveAddressValues(stationId, addressValues, ProtocolId(), 0, IsAutoCreateWorkerAtDeal());
//    }
}

void IEC104Protocol::DealTotal()
{
    unsigned char typeFlag = FRecBuf[6]; //类型标识
    unsigned char count = FRecBuf[7] & 0x7F; //可变结构限定词
    bool isContinueAddress = ( (FRecBuf[7] & 0x80 ) == 0x80 ) ? true : false; //可变结构限定词
    unsigned short reason = (FRecBuf[9] << 8) + FRecBuf[8]; //传送原因
    unsigned short stationId = ( FRecBuf[11] << 8 ) + FRecBuf[10]; //公共地址

//    if ( ! _ycRecTable->StationIds().contains( stationId ) )
//    {
//        GFunc_OutDebug(QString("[%1] - [DealYC] : Common Address [%2] Is Not Exist").arg(ProtocolName()).arg(stationId));
//        return;
//    }

//    QHash<int, double> addressValues;
    for (int i = 0; i < count; i++)
    {
        int address = 0;
        double value = 0;
        unsigned char quality = 0xFF;
        switch (typeFlag)
        {
        case 15: //累计量
        {
            if (! isContinueAddress)
            {
                if ( (12 + (i + 1) * 8) > IEC104BUFLEN - 1 )
                    break;
                address = IEC104Address(& (FRecBuf[12 + i * 8]));
                value = IEC104Value4(& (FRecBuf[15 + i * 8]));
                quality = FRecBuf[19 + i * 8];
            }
            else
            {
                if ( (15 + (i + 1) * 5) > IEC104BUFLEN - 1 )
                    break;
                address = IEC104Address(& (FRecBuf[12])) + i;
                value = IEC104Value4(& (FRecBuf[15 + i * 5]));
                quality = FRecBuf[19 + i * 5];
            }
        }
            break;

        case 37: //带时标的累计量
        {
        }
            break;

        }

//        addressValues[address] = value;
    }

//    if (addressValues.count()>0){
//        _ycRecTable->ReceiveAddressValues(stationId, addressValues, ProtocolId(), 0, IsAutoCreateWorkerAtDeal());
//    }
}

void IEC104Protocol::DealYK()
{
}

void IEC104Protocol::SendFrameS()
{
    unsigned char buf[6];

    buf[0] = 0x68;
    buf[1] = 0x04;
    buf[2] = 0x01;  //0000 0001
    buf[3] = 0x00;
    buf[4] = (FCalcerR * 2);
    buf[5] = (FCalcerR * 2) >> 8;
    SendBuffer(buf, sizeof(buf));
}

void IEC104Protocol::SendStart()
{
    unsigned char buf[6];

    buf[0] = 0x68;
    buf[1] = 0x04;
    buf[2] = 0x07;  //0000 0111
    buf[3] = 0x00;
    buf[4] = 0x00;
    buf[5] = 0x00;
    SendBuffer(buf, sizeof(buf));
}

void IEC104Protocol::SendStop()
{
    unsigned char buf[6];

    buf[0] = 0x68;
    buf[1] = 0x04;
    buf[2] = 0x13;  //0001 0011
    buf[3] = 0x00;
    buf[4] = 0x00;
    buf[5] = 0x00;
    SendBuffer(buf, sizeof(buf));
}

void IEC104Protocol::SendTest()
{
    if (! _tested) return;

    unsigned char buf[6];

    buf[0] = 0x68;
    buf[1] = 0x04;
    buf[2] = 0x43;  //0100 0011
    buf[3] = 0x00;
    buf[4] = 0x00;
    buf[5] = 0x00;
    SendBuffer(buf, sizeof(buf));
}

void IEC104Protocol::SendTestConfirmed()
{
    unsigned char buf[6];

    buf[0] = 0x68;
    buf[1] = 0x04;
    buf[2] = 0x83;  //1000 0011
    buf[3] = 0x00;
    buf[4] = 0x00;
    buf[5] = 0x00;
    SendBuffer(buf, sizeof(buf));
}






