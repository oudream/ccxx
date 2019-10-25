#ifndef CXSAFETHREAD_H
#define CXSAFETHREAD_H

#include "cxglobal.h"
#include "cxthread.h"

template <class T>
class  CxSafeThread: public CxJoinableThread
{
public:
    void notifyOne() {
        m_SingWait.signal();
    }

    void push(T& t) {
        m_record_set.push(t);
        m_SingWait.signal();
    }

    int bufferSize() {
        return m_record_set.size();
    }

    void waitExit() {
        m_state = 0;
        m_SingWait.signal();
    }

protected:
    void run()
    {
        m_state = 1;
        while(m_state)
        {
            m_SingWait.wait();
            while(!m_record_set.empty())
            {
                T t;
                t = m_record_set.front();
                m_record_set.pop();
                dataFunc(t);
            }
        }
    }

    virtual void dataFunc(T& t) = 0;

protected:
    std::queue<T> m_record_set;
    CxSingleWait m_SingWait;
    CxMutex m_oMutex;
    volatile int m_state;

};

#endif // CXSAFETHREAD_H
