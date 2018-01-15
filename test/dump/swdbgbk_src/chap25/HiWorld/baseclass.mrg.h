// Created by Microsoft (R) C/C++ Compiler Version 14.00.50727.42
//
// c:\dig\dbg\author\code\chap25\hiworld\baseclass.mrg.h
// compiler-generated file created 03/09/08 at 14:58:36
//
// This C++ source file is intended to be a representation of the
// source code injected by the compiler.  It may not compile or
// run exactly as the original source file.
//


//+++ Start Injected Code
[no_injected_text(true)];      // Suppress injected text, it has already been injected
#pragma warning(disable: 4543) // Suppress warnings about skipping injected text
#pragma warning(disable: 4199) // Suppress warnings from attribute providers

#pragma message("\n\nNOTE: This merged source file should be visually inspected for correctness.\n\n")
//--- End Injected Code



//+++ Start Injected Code For Attribute 'event'
#injected_line 15 "c:\\dig\\dbg\\author\\code\\chap25\\hiworld\\baseclass.h"
struct __EventingCriticalSectionStub 
{
    void Lock() 
    {
    }
    void Unlock() 
    {
    }
};
#injected_line 15 "c:\\dig\\dbg\\author\\code\\chap25\\hiworld\\baseclass.h"
// injected
template <class T>
    struct __eventingGetAddr 
{
    typedef void ( T::*pmfn_type) ();
    typedef void ( *pgfn_type) ();
    union U 
    {
        void *addr;
        void ( T::*pmfn)();
        void ( *pgfn)();
    };
    static pmfn_type  __getMAddr(void *addr) 
    {
        U u;
        u.addr = addr;
        return u.pmfn;
    }
    static void*  __getVAddr(pmfn_type pmfn) 
    {
        U u;
        u.pmfn = pmfn;
        return u.addr;
    }
    static pgfn_type  __getSMAddr(void *addr) 
    {
        U u;
        u.addr = addr;
        return u.pgfn;
    }
    static void*  __getSVAddr(pgfn_type pgfn) 
    {
        U u;
        u.pgfn = pgfn;
        return u.addr;
    }
};
#injected_line 15 "c:\\dig\\dbg\\author\\code\\chap25\\hiworld\\baseclass.h"
struct __eventNode 
{
    virtual int __isEqual(void*, void*) = 0;
    virtual int __isEqual(void*) = 0;
    __eventNode* next;
};
#injected_line 15 "c:\\dig\\dbg\\author\\code\\chap25\\hiworld\\baseclass.h"
struct __eventMainNode 
{
    int key;
    __eventNode* root_node;
    __eventMainNode* next_event;
};

//--- End Injected Code For Attribute 'event'
#pragma once

class CBaseClass
{
private:
	int m_nPrivate;
protected:
	TCHAR m_szName[MAX_PATH];
public:
	CBaseClass(void);
	~CBaseClass(void);
	virtual int Run(LPARAM lpParam)=0;
	__declspec(noinline) virtual int Setup(LPCTSTR szName);
	LPCTSTR GetName()const{return m_szName;} 
	__event void f();

	//+++ Start Injected Code For Attribute 'event'
#injected_line 15 "c:\\dig\\dbg\\author\\code\\chap25\\hiworld\\baseclass.h"
    __EventingCriticalSectionStub __EventingCS;
#injected_line 15 "c:\\dig\\dbg\\author\\code\\chap25\\hiworld\\baseclass.h"
    struct __eventNode_CBaseClass_f 
    {
        virtual void  __invoke() = 0;
        virtual int __isEqual(void*, void*) = 0;
        virtual int __isEqual(void*) = 0;
        __eventNode_CBaseClass_f* next;
    };
    template <class T>
        struct __InvokeHandlers_CBaseClass_f : __eventNode_CBaseClass_f 
    {
        T* p;
        void ( T::*pmfn) ();
        void  __invoke() 
        {
            return (p->*pmfn) ();
        }
        int __isEqual(void* p, void* pfn) 
        {
            return ((T*) p == this->p) && (__eventingGetAddr<T>::__getMAddr(pfn) == (void ( T::*) ()) pmfn);
        }
        int __isEqual(void* p) 
        {
            return ((T*) p == this->p);
        }
    };
    __eventNode_CBaseClass_f* __eventHandlerList_CBaseClass_f;
    template <class T>
        long __AddEventHandler_CBaseClass_f(T* pThis, void ( T::*pmfn) ()) 
    {
        __InvokeHandlers_CBaseClass_f<T>* newHead = new __InvokeHandlers_CBaseClass_f<T>;
        newHead->p = pThis;
        newHead->pmfn = pmfn;
        __EventingCS.Lock();
        __try 
        {
            newHead->next = __eventHandlerList_CBaseClass_f;
            __eventHandlerList_CBaseClass_f = (__eventNode_CBaseClass_f*) newHead;
        }
        __finally 
        {
            __EventingCS.Unlock();
        }
        return 0;
    }
    template <class T>
        long __RemoveEventHandler_CBaseClass_f(T* pThis, void ( T::*pmfn) ()) 
    {
        __EventingCS.Lock();
        long retcode = 1;
        __try 
        {
            __eventNode_CBaseClass_f* node = __eventHandlerList_CBaseClass_f;
            __eventNode_CBaseClass_f* prev = 0;
            for (; node != 0; node = node->next) {
                if (node->__isEqual(pThis,
                                    __eventingGetAddr<T>::__getVAddr((void (T::*) ()) pmfn)) != 0) {
                    if (prev == 0) {
                        __eventHandlerList_CBaseClass_f = node->next;
                    } 
                    else {
                        prev->next = node->next;
                    }
                    delete node;
                    retcode = 0;
                    __leave;
                }
                prev = node;
            }
        }
        __finally 
        {
            __EventingCS.Unlock();
        }
        return retcode;
    }
    template <class T>
        void __RemoveAllEventHandlers_CBaseClass_f(T* pThis) 
    {
        __EventingCS.Lock();
        __try 
        {
            __eventNode_CBaseClass_f* node = __eventHandlerList_CBaseClass_f;
            __eventNode_CBaseClass_f* prev = 0;
            while (node != 0) {
                if (node->__isEqual(pThis) != 0) {
                    __eventNode_CBaseClass_f* dead = node;
                    if (prev == 0) {
                        __eventHandlerList_CBaseClass_f = node = node->next;
                    } 
                    else {
                        prev->next = node = node->next;
                    }
                    delete dead;
                } 
                else {
                    prev = node;
                    node = node->next;
                }
            }
        }
        __finally 
        {
            __EventingCS.Unlock();
        }
    }
#injected_line 15 "c:\\dig\\dbg\\author\\code\\chap25\\hiworld\\baseclass.h"
    void __RemoveAllEventHandlers(void* pThis) 
    {
        __RemoveAllEventHandlers_CBaseClass_f(pThis);
    }

	//--- End Injected Code For Attribute 'event'
};

//+++ Start Injected Code For Attribute 'event'
#injected_line 15 "c:\\dig\\dbg\\author\\code\\chap25\\hiworld\\baseclass.h"
inline void  CBaseClass::f() 
{
    __EventingCS.Lock();
    __try 
    {
        __eventNode_CBaseClass_f* node = __eventHandlerList_CBaseClass_f;
        for (; node != 0; node = node->next) {
            node->__invoke();
        }
    }
    __finally 
    {
        __EventingCS.Unlock();
    }
}

//--- End Injected Code For Attribute 'event'

