// LBR.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "LBR.h"
#include <ntverp.h>

//
// globals
//
EXT_API_VERSION         ApiVersion = { (VER_PRODUCTVERSION_W >> 8), 
	(VER_PRODUCTVERSION_W & 0xff), 
	5, // EXT_API_VERSION_NUMBER64, 
	0 };

WINDBG_EXTENSION_APIS   ExtensionApis;
ULONG SavedMajorVersion;
ULONG SavedMinorVersion;


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


// This is an example of an exported variable
LBR_API int nLBR=0;

// This is an example of an exported function.
LBR_API int fnLBR(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see LBR.h for the class definition
CLBR::CLBR()
{ 
	return; 
}

VOID
WinDbgExtensionDllInit(
    PWINDBG_EXTENSION_APIS lpExtensionApis,
    USHORT MajorVersion,
    USHORT MinorVersion
    )
{
    ExtensionApis = *lpExtensionApis;

    SavedMajorVersion = MajorVersion;
    SavedMinorVersion = MinorVersion;

    return;
}

LPEXT_API_VERSION
ExtensionApiVersion(
    VOID
    )
{
    //
    // ExtensionApiVersion should return EXT_API_VERSION_NUMBER64 in order for APIs
    // to recognize 64 bit addresses.  KDEXT_64BIT also has to be defined before including
    // wdbgexts.h to get 64 bit headers for WINDBG_EXTENSION_APIS
    //
    return &ApiVersion;
}

//
// Routine called by debugger after load
//
VOID
CheckVersion(
    VOID
    )
{
    return;
}
void Version()
{
    dprintf("Access LBR (Last Branch Recording) registers of IA-32 CPU.\n"
		"Version 1.0.0.2 by Raymond\n");
}
//
// Extension to read LBR registers for Pentium M processors
//
#define LBR_COUNT 8
#define LBR_MSR_START_ADDR 0x40
#define MSR_LASTBRANCH_TOS 0x1c9
#define MSR_DEBUGCTLB      0x1d9
DECLARE_API( lbr )
{
    ULONG64 llDbgCtrl,llLBR;
    ULONG   ulFrom,ulTo,ulTos;
	CHAR	szSymbol[MAX_PATH];
    ULONG   ulDisplacement;
	int nToRead;

	Version();
	ReadMsr(MSR_DEBUGCTLB,&llDbgCtrl);
    dprintf("MSR_DEBUGCTLB=%x\n", (ULONG)llDbgCtrl);
	llDbgCtrl&=0xFFFFFFFE;// clear the LBR bit, bit 0
	WriteMsr(MSR_DEBUGCTLB,llDbgCtrl);
    dprintf("LBR bit is cleared now.\n");

	ReadMsr(MSR_LASTBRANCH_TOS,&llLBR);
	ulTos=llLBR&0xF;
    dprintf("MSR_LASTBRANCH_TOS=%x\n", ulTos);

	nToRead=ulTos;
	for (int i=0; i< LBR_COUNT;i++)
	{
		ReadMsr(LBR_MSR_START_ADDR+nToRead,&llLBR);
		ulFrom = llLBR;
		ulTo = (llLBR>>32);
        
		szSymbol[0] = '!';
        GetSymbol((PVOID)ulTo, (PUCHAR)szSymbol, &ulDisplacement);
    	dprintf("MSR_LASTBRANCH_%x: [%08lx] %s+%x\n", nToRead, 
			ulTo,szSymbol,ulDisplacement);

        szSymbol[0] = '!';
        GetSymbol((PVOID)ulFrom, (PUCHAR)szSymbol, &ulDisplacement);
    	dprintf("MSR_LASTBRANCH_%x: [%08lx] %s+%x\n", nToRead,
			ulFrom, szSymbol,ulDisplacement);

		nToRead--;
		if(nToRead<0)
			nToRead=LBR_COUNT-1;
	}

	llDbgCtrl+=1; // set bit 0
	WriteMsr(MSR_DEBUGCTLB,llDbgCtrl);
    dprintf("LBR bit is set now.\n");
}
/*
  A built-in help for the extension dll
*/

DECLARE_API ( help ) 
{
	Version();
    dprintf(
		"   lbr         - Reads and dumps LBR registers\n"
        "   help         - Shows this help\n"
        );
}
