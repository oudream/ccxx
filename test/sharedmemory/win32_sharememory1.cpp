
#include <ccxx/cxglobal.h>
#include <windows.h>
#include <stdio.h>
#include <conio.h>

#define BUF_SIZE 256
TCHAR szName[]=TEXT("Global\\MyFileMappingObject");
TCHAR szMsg[]=TEXT("Message from first process");

int main()
{
   HANDLE hMapFile;
   LPCTSTR pBuf;

   hMapFile = CreateFileMapping(
                 INVALID_HANDLE_VALUE,    // use paging file
                 NULL,                    // default security
                 PAGE_READWRITE,          // read/write access
                 0,                       // max. object size
                 BUF_SIZE,                // buffer size
                 szName);                 // name of mapping object

   if (hMapFile == NULL)
   {
      printf("Could not create file mapping object (%d).\n",
             GetLastError());
      return 1;
   }
   pBuf = (LPTSTR) MapViewOfFile(hMapFile,   // handle to map object
                        FILE_MAP_ALL_ACCESS, // read/write permission
                        0,
                        0,
                        BUF_SIZE);

   if (pBuf == NULL)
   {
      printf("Could not map view of file (%d).\n",
             GetLastError());
      return 2;
   }


   CopyMemory((PVOID)pBuf, szMsg, strlen(szMsg));

   printf("Please input any key to step 2...\n");
   _getch();

   UnmapViewOfFile(pBuf);

   printf("Please input any key to exit...\n");
   _getch();

//   CloseHandle(hMapFile);

   return 0;
}

