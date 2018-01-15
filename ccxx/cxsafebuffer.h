
#ifndef CXSAFEBUFFER_H
#define CXSAFEBUFFER_H

#include "ccxx.h"

#define CX_LEN_DATA_EM     1024   //数据单元长度定义
#define CX_LEN_DGM         4096   //数据报文长度

class GM_CCXX_CORE_API CxLoopBuf
{
public:
    /* : m_semaphore(0,ET_MAX_SIZE * 8) */
    //初始化信号灯的最大最小值
    CxLoopBuf( )
    {
        m_nMaxLen  = CX_LEN_DATA_EM;
        m_pDate    = new uchar[m_nMaxLen];
        m_pWrite   = m_pRead = m_pDate;
        m_nLen     = 0;
        m_nResize  = 0;

    }

    ~CxLoopBuf( )
    {
        if(m_pDate) {
            delete []m_pDate;
		}
    }

    void write(uchar by)
    {
         CxMutexScope _lock(m_lock);
        //收到的数据计数器累加,如果超过最大存储容量，考虑扩容
        if(m_nLen >= m_nMaxLen)
            resizeBuf();
        //写指针到尾部了，回头
        if(m_pWrite  >= m_pDate + m_nMaxLen)
            m_pWrite  = m_pDate;

        //写入数据 ,指针后移
        *m_pWrite     = by;
        m_pWrite++;
        m_nLen++;
    }

    bool read(uchar* pby)
    {
        CxMutexScope _lock(m_lock);
        bool bRead = false;
        if(m_nLen>0)
        {
            if(m_pRead - m_pDate >= m_nMaxLen)
                m_pRead = m_pDate;
            *pby  = *m_pRead++;
            bRead = true;
            m_nLen--;
        }
        return bRead;
    }

//    void reset()
//    {
//        uchar by;
//        while(read(&by))
//        {

//        }
//    }

private:
    uchar*   m_pWrite;            //写指针
    uchar*   m_pRead;             //读指针
    uchar*   m_pDate;             //数据指针
    int     m_nMaxLen;           //最大容量
    int     m_nLen;              //收到的数据计数器
    int     m_nResize;           //扩容计数器
    CxMutex m_lock;

    void resizeBuf()
    {
        if(m_nResize > 3)return;
        m_nResize++;
        //申请新的存储区域，翻倍
        uchar* pnew = new uchar[m_nMaxLen * 2];
        //将原来的数据拷贝到新的存储区
        for(int i=0;i< m_nLen; i++)
        {
            if(m_pRead - m_pDate >= m_nMaxLen)
                m_pRead = m_pDate;
            pnew[i] = *m_pRead++;
        }

        uchar*   pDel = m_pDate;
        m_pDate  = pnew;
        m_pRead  = m_pDate;             //全部搬过来了，所以读指针与存储首地址平
        m_pWrite = m_pDate + m_nLen;    //写指针相应的就等于首地址 + 收到的数据量
        if(pDel)
        {
            delete []pDel;
            pDel = NULL;
        }
        m_nMaxLen *= 2;
    }

};


class GM_CCXX_CORE_API CxSafeBuffer
{
public:
    CxSafeBuffer(void);
    
	virtual ~CxSafeBuffer(void);
	
	CxSafeBuffer& operator =(CxSafeBuffer&  pI);

public:
    void  putIn(const uchar *pBuf,const ushort &wSize);

    uchar* getBuf(ushort& wLen);

	void removeLeft(int nlen);
    
    void setMaxLen(ushort wLen);
    
	void clear(void);

public:
    ushort getMaxLen(){return m_MaxLen;}
    
    ushort getPoint(){return m_wWpoint;}

private:
    uchar m_buf[CX_LEN_DGM];
    uchar m_byTmp[CX_LEN_DGM];
    ushort m_wWpoint;
    ushort m_MaxLen;
    CxLoopBuf *m_pLoopBuf;

};

#endif // CXSAFEBUFFER_H
