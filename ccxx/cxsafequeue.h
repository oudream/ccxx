#ifndef CXSAFEQUEUE_H
#define CXSAFEQUEUE_H

#include "ccxx.h"

template <class T>
class CxSafeQueue
{
public:
    void push( const T& x )
    {
        CxMutexScope sl(_lock);
        _q.push( x );
    }

    bool pop( T& x )
    {
        bool bRet = false;
        CxMutexScope sl(_lock);
        if( !_q.empty() )
        {
            bRet = true;
            x = _q.front();
            _q.pop();
        }
        return bRet;
    }

    bool empty()
    {
        CxMutexScope sl(_lock);
        return _q.empty();
    }

    int size()
    {
         CxMutexScope sl(_lock);
         return _q.size();
    }

    void clear()
    {
        CxMutexScope sl(_lock);
        int nSize = _q.size();
        for(int i=0;i<nSize;i++)_q.pop();
    }
private:
     std::queue<T>  _q;
     CxMutex   _lock;

};

#endif // CXSAFEQUEUE_H
