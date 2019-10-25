
#ifndef CXSAFEBUFFER_H
#define CXSAFEBUFFER_H

#include "ccxx.h"

#define CX_LEN_DATA_EM     1024
#define CX_LEN_DGM         4096

class GM_CCXX_CORE_API CxLoopBuf
{
public:
    /* : m_semaphore(0,ET_MAX_SIZE * 8) */
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
        // max
        if(m_nLen >= m_nMaxLen)
            resizeBuf();
        // end, loop back
        if(m_pWrite  >= m_pDate + m_nMaxLen)
            m_pWrite  = m_pDate;

        // ++
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
    uchar*   m_pWrite;
    uchar*   m_pRead;
    uchar*   m_pDate;
    int     m_nMaxLen;
    int     m_nLen;
    int     m_nResize;
    CxMutex m_lock;

    void resizeBuf()
    {
        if(m_nResize > 3)return;
        m_nResize++;
        // * 2
        uchar* pnew = new uchar[m_nMaxLen * 2];
        for(int i=0;i< m_nLen; i++)
        {
            if(m_pRead - m_pDate >= m_nMaxLen)
                m_pRead = m_pDate;
            pnew[i] = *m_pRead++;
        }

        uchar*   pDel = m_pDate;
        m_pDate  = pnew;
        m_pRead  = m_pDate;             // all to m_pRead
        m_pWrite = m_pDate + m_nLen;
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
