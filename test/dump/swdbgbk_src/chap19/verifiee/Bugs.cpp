// Bugs.cpp: implementation of the CBugs class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "verifiee.h"
#include "Bugs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

typedef void (*PBUG_STOP_ROUTINE)(
    LPVOID lpParameter
    );
void BugAccessViolation(LPVOID lpParameter)
{
	*(int *)0=1;
}
void BugDestroyProcHeap(LPVOID lpParameter)
{
	HeapDestroy(GetProcessHeap());
}
void BugDoubleFree(LPVOID lpParameter)
{
	LPVOID lpvBase;               // base address of the test memory
	SYSTEM_INFO sSysInfo;         // useful information about the system
	DWORD dwPageSize;

	GetSystemInfo(&sSysInfo);     // initialize the structure

	dwPageSize = sSysInfo.dwPageSize;
	lpvBase = VirtualAlloc(
					 NULL,                 // system selects address
					 2*dwPageSize, // size of allocation
					 MEM_RESERVE,          // allocate reserved pages
					 PAGE_NOACCESS);       // protection = no access
	if (lpvBase == NULL )
	{
		MessageBox(NULL, _T("VirtualAlloc failed"),_T("verifiee"), MB_OK);
		return;
	}
	VirtualFree(lpvBase, 0, MEM_RELEASE);

	// try freeing the freed mempry.
	VirtualFree(lpvBase, 0, MEM_RELEASE);
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
typedef struct _BugStop{
		TCHAR *	StopName;	// title of column
		DWORD	StopCode;	// width of column in pixels
		PBUG_STOP_ROUTINE	BugRoutine;
	} BugStop;
static BugStop g_BugStops[] = {
		{	"Access Violation Exception", 	2, 	BugAccessViolation	},
		{	"Attempt to destroy process heap",	9, BugDestroyProcHeap},
		{	"Trying to free freed memory."	,	0x0610,	BugDoubleFree},
	};
CBugs::CBugs()
{

}

CBugs::~CBugs()
{

}

void CBugs::FillListBox(CListBox &lb)
{
	for (int i=0;i< sizeof(g_BugStops)/sizeof (BugStop);i++)
	{
		lb.AddString(g_BugStops[i].StopName);
	}
}

void CBugs::FireBug(int i)
{
	g_BugStops[i].BugRoutine(this);
}
