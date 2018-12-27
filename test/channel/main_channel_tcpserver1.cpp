//
// Created by oudream on 2017/3/17.
//
#include <ccxx/ccxx.h>

using namespace std;

typedef unsigned char BYTE;

//typedef int (*SocketSend_t)(const BYTE * pBuf, const int& nBufLen);

#define PACKAGE_OK                   0
#define PACKAGE_ERROR                300

#define PACKAGE_REQ_START            0xE9
#define PACKAGE_REQ_END              0xEA
#define PACKAGE_REQ_TRANS            0xE8
#define PACKAGE_REQ_CRC              0x7A

#define OMC_OK                   0

//无基本信息
#define OMC_OK_WITHINFO          100
#define OMC_ERROR                300
#define OMC_NE_NO_EXIST          301
#define OMC_PARAM_ERROR          302
#define OMC_ALARM_NO_LEN         4
#define OMC_ADDR_LEN             32
#define OMC_NE_LEN               32
#define OMC_NORMAL_INFO_LEN      32
#define OMC_PHYSICAL_NO_LEN      48

#define PACKAGE_ITEM_REQ_LEN         4

//一般的响应内存长度
#define PACKAGE_SIMPLE_REQ_LEN       512
#define PACKAGE_MAX_REQ_LEN          2048
//接收缓冲池打消
#define PACKAGE_MAX_BUF_SIZE         30000
//最大ip地址长度
#define PACKAGE_MAX_IP_ADDR          64
//接收信息buf 的负荷率
#define PACKAGE_BUF_BALANCE          0.6
//初始化客户端连接的有效数
#define PACKAGE_INIT_VALID           10
//服务器最大接受的连接数
#define PACKAGE_MAX_CLIENT_CONN      200
//client 端使用
#define PACKAGE_WAIT_ANSWER          10
#define PACKAGE_WAIT_INTERVAL        2
#define PACKAGE_WAIT_EXIT            100

//package type
//#define OMC_REQ_HEARTBEAT                0x11
#define OMC_REQ_LOGIN                    0x12
#define OMC_REQ_UPD_DATA                 0x13
#define OMC_REQ_MOD_ALARM                0x14
#define OMC_REQ_ALARM_INFO_BY_ADDR       0x15
#define OMC_REQ_ALARM_INFO_BY_IDX        0x16
#define OMC_REQ_EXIT                     0x17

#define OMC_ANS_LOGIN                    0x21
#define OMC_ANS_MOD_ALARM                0x23
#define OMC_ANS_ALARM_INFO_BY_ADDR       0x24
#define OMC_ANS_ALARM_INFO_BY_IDX        0x25
#define OMC_ANS_UPD_DATA                 0x26

//modify action for alarm
#define OMC_CONFIRM_ALARM          1  //告警确认
#define OMC_INVOKE_CONFIRM_ALARM   2  //取消确认
#define OMC_ERASE_ALARM            3  //告警清除
#define OMC_SET_ALARM              4  //告警同步
#define OMC_CLEAR_ALARM            5  //告警恢复

struct TPackageReq {
public:
    BYTE m_nStart;
    int m_nCmdSeq;
    int m_nCommand;
    int m_nReqLen;
    BYTE *m_pCmdBuf;
    BYTE m_nCrc;
    BYTE m_nEnd;

public:
    TPackageReq()
    {
        m_nStart = PACKAGE_REQ_START;
        m_nEnd = PACKAGE_REQ_END;
        m_nReqLen = 0;
        m_pCmdBuf = NULL;
        m_nCrc = 0;
    }

    ~TPackageReq()
    {
        //if (m_pCmdBuf != NULL)
        //{
        //	delete [] m_pCmdBuf;
        //	m_pCmdBuf = NULL;
        //}
    }
};

int memcpy_byte(unsigned &nDest, const BYTE *pSource, const int &nCount)
{
    if (pSource == NULL)
    {
        nDest = 0;
        return PACKAGE_OK;
    }

    int i = nCount - 1;
    unsigned int nCntVal = 0;
    while (i >= 0)
    {
        nCntVal = nCntVal << 8;
        nCntVal = nCntVal | pSource[i];
        i--;
    }
    nDest = nCntVal;
    return PACKAGE_OK;
}

int memcpy_byte(int &nDest, const BYTE *pSource, const int &nCount)
{
    if (pSource == NULL)
    {
        nDest = 0;
        return PACKAGE_OK;
    }

    int i = nCount - 1;
    unsigned int nCntVal = 0;
    while (i >= 0)
    {
        nCntVal = nCntVal << 8;
        nCntVal = nCntVal | pSource[i];
        i--;
    }
    nDest = nCntVal;
    return PACKAGE_OK;
}

int memcpy_byte(BYTE *pDest, const unsigned &nSource, const int &nCount)
{
    int i = nCount;
    unsigned nVal;
    while (i > 0)
    {
        nVal = 0x0ff;
        nVal = nVal << 8 * (i - 1);
        nVal = nVal & nSource;
        nVal = nVal >> 8 * (i - 1);
        pDest[i - 1] = (BYTE) (nVal);
        i--;
    }
    return PACKAGE_OK;
}

int PrepareMsg(BYTE *pBuf, int &nLen)
{
    int nNewLen = nLen;
    if (nNewLen < 2)
    {
        nNewLen = 2;
    }
    nNewLen += nLen;
    if (nLen <= 0)
    {
        return PACKAGE_ERROR;
    }
    BYTE *pDest = new BYTE[nNewLen];
    int i = 1;
    int j = 1;
    pDest[0] = pBuf[0];
    while (i < (nLen - 1))
    {
        if (pBuf[i] >= PACKAGE_REQ_TRANS)
        {
            pDest[j] = PACKAGE_REQ_TRANS;
            j++;
            pDest[j] = pBuf[i] - PACKAGE_REQ_TRANS;
        }
        else
        {
            pDest[j] = pBuf[i];
        }
        i++;
        j++;
    }
    pDest[j] = pBuf[i];
    nLen = j + 1;

    memcpy(pBuf, pDest, nLen);
    if (pDest != NULL)
    {
        delete[] pDest;
    }
    pDest = NULL;

    BYTE nCrc = 0;
    for (unsigned int h = 1; h < nLen - 2; h++)
    {
        nCrc += ((BYTE *) pBuf)[h];
    }
    ((BYTE *) pBuf)[nLen - 2] = PACKAGE_REQ_CRC - nCrc;
    return PACKAGE_OK;
}

int EncodeResponse(TPackageReq *pResponse, BYTE *pCmdBuf, int &nCmdLen)
{
    int nPos = 0;
    pCmdBuf[nPos] = pResponse->m_nStart;
    nPos += 1;

    memcpy_byte(&(pCmdBuf[nPos]), pResponse->m_nCmdSeq, PACKAGE_ITEM_REQ_LEN);
    nPos += PACKAGE_ITEM_REQ_LEN;

    memcpy_byte(&(pCmdBuf[nPos]), pResponse->m_nCommand, PACKAGE_ITEM_REQ_LEN);
    nPos += PACKAGE_ITEM_REQ_LEN;

    memcpy_byte(&(pCmdBuf[nPos]), pResponse->m_nReqLen, PACKAGE_ITEM_REQ_LEN);
    nPos += PACKAGE_ITEM_REQ_LEN;

    if (pResponse->m_nReqLen != 0)
    {
        memcpy(&(pCmdBuf[nPos]), pResponse->m_pCmdBuf, pResponse->m_nReqLen);
        nPos += pResponse->m_nReqLen;
    }

    pCmdBuf[nPos] = pResponse->m_nCrc;
    nPos += 1;

    pCmdBuf[nPos] = pResponse->m_nEnd;
    nPos += 1;
    nCmdLen = nPos;

    PrepareMsg((BYTE *) pCmdBuf, nCmdLen);
    return PACKAGE_OK;
}

int FindReq(BYTE *pBuf, int iLen, BYTE *pMsg, int *pMsgLen)
{
    *pMsgLen = 0;
    if (iLen == 0)
    {
        return 0;
    }

    int nStartPos = -1;
    int i = 0;
    while (i < iLen)
    {
        //命令
        if (BYTE(pBuf[i]) == PACKAGE_REQ_START)
        {
            nStartPos = i;
            int j = i + 1;
            int nEndPos = 0;
            while ((j < iLen) && (nEndPos == 0))
            {
                if (BYTE(pBuf[j]) == PACKAGE_REQ_START)
                {
                    nStartPos = j;
                }
                if (BYTE(pBuf[j]) == PACKAGE_REQ_END)
                {
                    nEndPos = j;
                }
                j++;
            }

            if (nEndPos == 0)
            {
                //nothing to do, wait to receive all bytes
                *pMsgLen = 0;
                return PACKAGE_OK;
            }
            else
            {
                *pMsgLen = nEndPos - nStartPos + 1;
                for (int k = 0; k < *pMsgLen; k++)
                {
                    pMsg[k] = pBuf[nStartPos + k];
                }
                int nOriginLen = iLen;
                iLen = iLen - nStartPos - *pMsgLen;
                if (iLen > 0)
                {
                    memmove(pBuf, &(pBuf[nEndPos + 1]), iLen);
                }
                else
                {
                    memset(pBuf, 0, nOriginLen);
                }
                return PACKAGE_OK;
            }
        }
        i++;
    }
    if (nStartPos == -1)
    {
        *pMsgLen = 0;
    }
    if (i >= iLen)
    {
        *pMsgLen = 0;
    }
    return PACKAGE_OK;
}

int RepairReq(BYTE *pSrc, int nSrcLen, BYTE *pDes, int *pDesLen)
{
    int i, j;
    pDes[0] = pSrc[0];
    for (i = 1, j = 1; i < nSrcLen - 1; i++, j++)
    {
        if (BYTE(pSrc[i]) == PACKAGE_REQ_TRANS)
        {
            pDes[j] = pSrc[i] + pSrc[i + 1];
            i++;
        }
        else
        {
            pDes[j] = pSrc[i];
        }
    }
    pDes[j] = pSrc[i];
    j++;
    *pDesLen = j;

    BYTE nCrc = 0;
    for (int i = 1; i < j - 2; i++)
    {
        nCrc += pDes[i];
    }
    nCrc = PACKAGE_REQ_CRC - nCrc;
    if (nCrc != pDes[j - 2]) //校验失败
    {
        return PACKAGE_ERROR;
    }
    return PACKAGE_OK;
}

int ExtraReqSketch(BYTE *pDesMsg, const int &nMsgLen, TPackageReq *pReq)
{
    int nReqIdx = 0;
    pReq->m_nStart = pDesMsg[nReqIdx];
    nReqIdx += 1;
    memcpy_byte(pReq->m_nCmdSeq, &(pDesMsg[nReqIdx]), PACKAGE_ITEM_REQ_LEN);
    nReqIdx += PACKAGE_ITEM_REQ_LEN;
    memcpy_byte(pReq->m_nCommand, &(pDesMsg[nReqIdx]), PACKAGE_ITEM_REQ_LEN);
    nReqIdx += PACKAGE_ITEM_REQ_LEN;
    memcpy_byte(pReq->m_nReqLen, &(pDesMsg[nReqIdx]), PACKAGE_ITEM_REQ_LEN);
    nReqIdx += PACKAGE_ITEM_REQ_LEN;
    pReq->m_pCmdBuf = &(pDesMsg[nReqIdx]);
    nReqIdx += pReq->m_nReqLen;
    pReq->m_nCrc = pDesMsg[nReqIdx];
    nReqIdx += 1;
    pReq->m_nEnd = pDesMsg[nReqIdx];

    if (pReq->m_nReqLen < 0 || pReq->m_nReqLen > nMsgLen)
    {
        return PACKAGE_ERROR;
    }
    return PACKAGE_OK;
}

class Protocol : public CxIChannelSubject {
public:
    Protocol()
    {
        _channel = new CxChannelTcpserver();

        _channel->addObserver(this);

        _channel->setLocalIp("127.0.0.1");
        _channel->setLocalPort(5556);
        _channel->setAutoOpenInterval(1000 * 5);
        _channel->open();
//        _channel->close();
//        _channel->open();
//        _channel->close();
//        _channel->open();
    }

    ~Protocol()
    {
        delete _channel;
    }

    int SocketSend(const BYTE *pBuf, const int &nBufLen)
    {
        return _channel->sendData(pBuf, nBufLen);
    }

protected:
    void channel_receivedData(const uchar *pData, int iLength, void *oSource)
    {
        bool b = _channel->connected();
        if (b)
        {
            _channel->sendData(pData, iLength);
        }
//        HandleSvrRecv((BYTE *)pData, iLength);
    }

    /*-----------------------------------------------------------------------------
    Function: 作为 socket 服务器, 接收的请求
    Parameter:  nPort: 当前接收数据的端口
    tSocket: 客户端
    pBuf: 数据存储
    pBufLen: 数据长度
    Return Value: 是否成功
    Date: 2017.02.20
    -----------------------------------------------------------------------------*/
    int HandleSvrRecv(BYTE *pBuf, int iBufLen)
    {
        int nRst = OMC_ERROR;
        int nMsgLen = -1;

        BYTE pMsg[PACKAGE_MAX_BUF_SIZE];
        while ((iBufLen >= 1) && (nMsgLen != 0))
        {
            memset(pMsg, 0, PACKAGE_MAX_BUF_SIZE);
            FindReq(pBuf, iBufLen, pMsg, &nMsgLen);
            if (nMsgLen == 0)
            {
                return nRst;
            }

            BYTE pDesMsg[PACKAGE_MAX_REQ_LEN];
            memset(pDesMsg, 0, PACKAGE_MAX_REQ_LEN);

            int nRealCnt = -1;
            nRst = RepairReq(pMsg, nMsgLen, pDesMsg, &nRealCnt);
            if (nRst != OMC_OK)
            {
                return nRst;
            }

            TPackageReq tReq;
            nRst = ExtraReqSketch(pDesMsg, nRealCnt, &tReq);
            if (nRst != PACKAGE_OK)
            {
                return nRst;
            }
            BYTE *pReqInfo = tReq.m_pCmdBuf;

            switch (tReq.m_nCommand)
            {
                case OMC_REQ_LOGIN:
                {
                    cout << "OMC_REQ_LOGIN" << endl;
                    return nRst;
                }
                    break;

                case OMC_REQ_UPD_DATA:
                {
                    cout << "OMC_REQ_UPD_DATA" << endl;
                    return nRst;
                }
                    break;

                case OMC_REQ_MOD_ALARM:
                {
                    cout << "OMC_REQ_MOD_ALARM" << endl;
                    return nRst;
                }
                    break;

                case OMC_REQ_ALARM_INFO_BY_ADDR:
                {
                    cout << "OMC_REQ_ALARM_INFO_BY_ADDR" << endl;
                    return nRst;
                }
                    break;

                case OMC_REQ_EXIT:
                {
                    cout << "OMC_REQ_EXIT" << endl;
                    return nRst;
                }
                    break;
            }
        }
        return nRst;
    }

private:
    CxChannelTcpserver *_channel;

};

Protocol *f_oProtocol = NULL;

void memcpy_1(void *_Dst, const void *_Src, size_t _Size)
{
    size_t iLen = strlen((char *) _Src);
    iLen = (_Size > iLen) ? iLen : _Size;
    memcpy(_Dst, _Src, iLen);
}

int ClientLogin(const string &sUser, const string &sPassword,
                const string &sPhysicalNo, const int &nKind)
{
    TPackageReq tReq;
    tReq.m_nStart = PACKAGE_REQ_START;
    tReq.m_nEnd = PACKAGE_REQ_END;
    tReq.m_nCmdSeq = 1;
    tReq.m_nCommand = OMC_REQ_LOGIN;
    tReq.m_nReqLen = OMC_NORMAL_INFO_LEN + OMC_NORMAL_INFO_LEN + OMC_PHYSICAL_NO_LEN + PACKAGE_ITEM_REQ_LEN;

    BYTE pCmdBuf[PACKAGE_SIMPLE_REQ_LEN];
    memset(pCmdBuf, 0, PACKAGE_SIMPLE_REQ_LEN);

    memcpy_1(pCmdBuf, sUser.c_str(), OMC_NORMAL_INFO_LEN);
    int nIdx = OMC_NORMAL_INFO_LEN;
    memcpy_1(&(pCmdBuf[nIdx]), sPassword.c_str(), OMC_NORMAL_INFO_LEN);
    nIdx += OMC_NORMAL_INFO_LEN;
    memcpy_1(&(pCmdBuf[nIdx]), sPhysicalNo.c_str(), OMC_PHYSICAL_NO_LEN);
    nIdx += OMC_PHYSICAL_NO_LEN;
    memcpy_byte(&(pCmdBuf[nIdx]), nKind, PACKAGE_ITEM_REQ_LEN);
    nIdx += PACKAGE_ITEM_REQ_LEN;

    tReq.m_pCmdBuf = pCmdBuf;

    int nCmdLen = 0;
    BYTE pBuf[PACKAGE_MAX_REQ_LEN];
    memset(pBuf, 0, PACKAGE_MAX_REQ_LEN);
    EncodeResponse(&tReq, pBuf, nCmdLen);

    //set alarm
    int nRst = f_oProtocol->SocketSend(pBuf, nCmdLen);
    if (nRst != OMC_OK)
    {
        return OMC_ERROR;
    }
    return nRst;
}

int NotifyAlarm(const string &sUser, const int nNeID, const int nAlarmType,
                            const int nModuleNo, const int nCardNo, const int nPortNo,
                            const int nAction)
{
    TPackageReq tReq;
    tReq.m_nStart = PACKAGE_REQ_START;
    tReq.m_nEnd = PACKAGE_REQ_END;
    tReq.m_nCmdSeq = 1;
    tReq.m_nCommand = OMC_REQ_MOD_ALARM;
    tReq.m_nReqLen = 60;  //7*4+32 = 60

    BYTE pCmdBuf[PACKAGE_SIMPLE_REQ_LEN];
    memset(pCmdBuf, 0, PACKAGE_SIMPLE_REQ_LEN);

    int nIdx = 0;
    int nAlarmNo = 0;
    memcpy_byte(&(pCmdBuf[nIdx]), nAlarmNo, PACKAGE_ITEM_REQ_LEN);
    nIdx += PACKAGE_ITEM_REQ_LEN;
    memcpy_byte(&(pCmdBuf[nIdx]), nAction, PACKAGE_ITEM_REQ_LEN);
    nIdx += PACKAGE_ITEM_REQ_LEN;
    memcpy_1(pCmdBuf, sUser.c_str(), OMC_NORMAL_INFO_LEN);
    nIdx += OMC_NORMAL_INFO_LEN;
    memcpy_byte(&(pCmdBuf[nIdx]), nNeID, PACKAGE_ITEM_REQ_LEN);
    nIdx += PACKAGE_ITEM_REQ_LEN;
    memcpy_byte(&(pCmdBuf[nIdx]), nAlarmType, PACKAGE_ITEM_REQ_LEN);
    nIdx += PACKAGE_ITEM_REQ_LEN;
    memcpy_byte(&(pCmdBuf[nIdx]), nModuleNo, PACKAGE_ITEM_REQ_LEN);
    nIdx += PACKAGE_ITEM_REQ_LEN;
    memcpy_byte(&(pCmdBuf[nIdx]), nCardNo, PACKAGE_ITEM_REQ_LEN);
    nIdx += PACKAGE_ITEM_REQ_LEN;
    memcpy_byte(&(pCmdBuf[nIdx]), nPortNo, PACKAGE_ITEM_REQ_LEN);
    nIdx += PACKAGE_ITEM_REQ_LEN;

    tReq.m_pCmdBuf = pCmdBuf;

    int nCmdLen = 0;
    BYTE pBuf[PACKAGE_MAX_REQ_LEN];
    memset(pBuf, 0, PACKAGE_MAX_REQ_LEN);
    EncodeResponse(&tReq, pBuf, nCmdLen);

    //set alarm
    int nRst = f_oProtocol->SocketSend(pBuf, nCmdLen);
    if (nRst != OMC_OK)
    {
        return OMC_ERROR;
    }
    return nRst;
}

int UpdateData(const string& sUser, const int& nKind)
{
    TPackageReq tReq;
    tReq.m_nStart = PACKAGE_REQ_START;
    tReq.m_nEnd = PACKAGE_REQ_END;
    tReq.m_nCmdSeq = 1;
    tReq.m_nCommand = OMC_REQ_UPD_DATA;
    tReq.m_nReqLen = OMC_NORMAL_INFO_LEN + PACKAGE_ITEM_REQ_LEN;

    BYTE pCmdBuf[PACKAGE_SIMPLE_REQ_LEN];
    memset(pCmdBuf, 0, PACKAGE_SIMPLE_REQ_LEN);

    memcpy(pCmdBuf, sUser.c_str(), OMC_NORMAL_INFO_LEN);
    int nIdx = OMC_NORMAL_INFO_LEN;
    memcpy_byte(&(pCmdBuf[nIdx]), nKind, PACKAGE_ITEM_REQ_LEN);
    nIdx += PACKAGE_ITEM_REQ_LEN;

    tReq.m_pCmdBuf = pCmdBuf;

    int nCmdLen = 0;
    BYTE pBuf[PACKAGE_MAX_REQ_LEN];
    memset(pBuf, 0, PACKAGE_MAX_REQ_LEN);
    EncodeResponse(&tReq, pBuf, nCmdLen);

    //set alarm
    int nRst = f_oProtocol->SocketSend(pBuf, nCmdLen);
    if (nRst != OMC_OK)
    {
        return OMC_ERROR;
    }
    return nRst;
}

void fn_timer_timeout(int iInterval)
{
    static int iTimes = 0;
    iTimes++;

    cout << "..." << endl;

    switch (iTimes % 3)
    {
        case 0:
            ClientLogin("user1", "password1", "no1", 1001);
            cout << "ClientLogin" << endl;
            break;
        case 1:
            NotifyAlarm("我是测试部门一用户1abc", 123456, 456789, 123456789, 0x81001122, 0x71001122, 0xFF001122);
            cout << "NotifyAlarm" << endl;
            break;
        case 2:
            UpdateData("我是测试部门一用户1abc", 0xFF00AA00);
            cout << "UpdateData" << endl;
            break;
    }
};

int main(int argc, const char *argv[])
{
    cout << "begin test tcp server" << endl;

    CxApplication::init(argc, argv);

    f_oProtocol = new Protocol();

    CxTimerManager::startTimer(fn_timer_timeout, 3000);

    return CxApplication::exec();
}

