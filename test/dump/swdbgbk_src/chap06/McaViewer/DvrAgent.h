// DvrAgent.h: interface for the CDvrAgent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DVRAGENT_H__4556493F_BCED_4DD8_B87A_07503D714349__INCLUDED_)
#define AFX_DVRAGENT_H__4556493F_BCED_4DD8_B87A_07503D714349__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
typedef enum _DEBUG_CONTROL_CODE {
	DebugSysReadIoSpace = 14,
	DebugSysWriteIoSpace = 15,
	DebugSysReadMsr = 16,
	DebugSysWriteMsr = 17,
	DebugSysReadBusData = 18,
	DebugSysWriteBusData = 19,
} DEBUG_CONTROL_CODE;
typedef struct _MSR_STRUCT {
	DWORD MsrNum;			// MSR number
	DWORD NotUsed;			// Never accessed by the kernel
	DWORD MsrLo;			// IN (write) or OUT (read): Low 32 bits of MSR
	DWORD MsrHi;			// IN (write) or OUT (read): High 32 bits of MSR
} MSR_STRUCT;
typedef
int
(NTAPI *PZwSystemDebugControl)(
	DEBUG_CONTROL_CODE ControlCode,
    PVOID InputBuffer,
	ULONG InputBufferLength,
	PVOID OutputBuffer,
    ULONG OutputBufferLength,
	PULONG ReturnLength
	);

class CDvrAgent  
{
public:
	BOOL IsInited();
	BOOL Init();
	CDvrAgent();
	virtual ~CDvrAgent();
	int RDMSR(MSR_STRUCT& msr);
	int WRMSR(MSR_STRUCT& msr);
protected:
	PZwSystemDebugControl ZwSystemDebugControl; 
};

#endif // !defined(AFX_DVRAGENT_H__4556493F_BCED_4DD8_B87A_07503D714349__INCLUDED_)
