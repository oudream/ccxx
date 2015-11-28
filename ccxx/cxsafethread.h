#ifndef CXSAFETHREAD_H
#define CXSAFETHREAD_H

#include "ccxx.h"

template <class T>
class  CxSafeThread: public CxJoinableThread
{
public:
    // 单个通知接口（一般不需调用）
    void notifyOne() {
        m_SingWait.signal();
    }

    // 推入流数据
    void push(T& t) {
        m_record_set.push(t);
        // 发送通知信号
        m_SingWait.signal();
    }

    // 获取缓冲区buffer size的接口
    int bufferSize() {
        return m_record_set.size();
    }

    void waitExit() {
        m_state = 0;
        m_SingWait.signal();
    }

protected:
    // 开始运行
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

    // 处理数据的函数，可在运行时绑定
    virtual void dataFunc(T& t) = 0;

protected:
    // 流数据集
    std::queue<T> m_record_set;
    // 信号
    CxSingleWait m_SingWait; 
    CxMutex m_oMutex;
    volatile int m_state;

};

#endif // CXSAFETHREAD_H
