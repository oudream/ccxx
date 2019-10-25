#include "cxsafebuffer.h"

CxSafeBuffer::CxSafeBuffer()
{
    m_MaxLen  = CX_LEN_DGM;
    m_wWpoint = 0;
    m_pLoopBuf = NULL;
    memset(m_buf,0,CX_LEN_DGM);
}

CxSafeBuffer::~CxSafeBuffer()
{
    if(m_pLoopBuf)
    {
        delete m_pLoopBuf;m_pLoopBuf = NULL;
    }
    memset(m_buf,0,CX_LEN_DGM);
}

void  CxSafeBuffer::putIn(const uchar *pBuf,const ushort &wSize)
{
    if((m_wWpoint + wSize) <= m_MaxLen)
    {
         memcpy(&m_buf[m_wWpoint],pBuf,wSize);
         m_wWpoint += wSize;
    }else{
         if(m_pLoopBuf==NULL) m_pLoopBuf = new CxLoopBuf();

         ushort wLen1 = m_MaxLen-m_wWpoint;
         memcpy(&m_buf[m_wWpoint],pBuf,wLen1);
         m_wWpoint = m_MaxLen;

         ushort wLen2 =  wSize-wLen1; // excess data
         if(m_pLoopBuf!=NULL)
         {
             for(ushort i=0;i<wLen2;i++)
             {
                 m_pLoopBuf->write(pBuf[wLen1+i]);
             }
         }
    }
}

uchar* CxSafeBuffer::getBuf(ushort& wLen)
{
    wLen = m_wWpoint;
    return m_buf;
}

void CxSafeBuffer::removeLeft(int nlen)
{
    if((ushort)nlen > m_wWpoint)return;

    m_wWpoint -=(ushort)nlen;
    memset(m_byTmp,0,CX_LEN_DGM);
    memcpy(m_byTmp,&m_buf[nlen],m_wWpoint);
    memset(m_buf,0,CX_LEN_DGM);
    memcpy(m_buf,m_byTmp,m_wWpoint);
// read from loopbuf
    if(m_pLoopBuf!=NULL)
    {
        while(m_pLoopBuf->read(&m_buf[m_wWpoint]))
        {
            m_wWpoint++;
            if(m_wWpoint>=m_MaxLen)break;// full cache
        }
    }
}

void CxSafeBuffer::setMaxLen(ushort wLen)
{
    if(wLen <= CX_LEN_DGM)
    {
        m_MaxLen = wLen;
    }
}

void CxSafeBuffer::clear()
{
     m_wWpoint =0;
     memset(m_buf,0,CX_LEN_DGM);

    if(m_pLoopBuf!=NULL)
    {
//        m_pLoopBuf->reset();
    }
}
CxSafeBuffer& CxSafeBuffer::operator =(  CxSafeBuffer&  pI)
{
    ushort wLen =0;
    uchar* pBy = pI.getBuf(wLen);
    memset(m_buf,0,CX_LEN_DGM);
    memcpy(m_buf,pBy,wLen);
    m_MaxLen = pI.getMaxLen();
    m_wWpoint = pI.getPoint();
    return *this;
}

