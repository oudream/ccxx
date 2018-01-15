
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LBR_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LBR_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef LBR_EXPORTS
#define LBR_API __declspec(dllexport)
#else
#define LBR_API __declspec(dllimport)
#endif

// This class is exported from the LBR.dll
class LBR_API CLBR {
public:
	CLBR(void);
	// TODO: add your methods here.
};

extern LBR_API int nLBR;

LBR_API int fnLBR(void);

