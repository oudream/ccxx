#undef UNICODE

#include <stdio.h>
#include <stdlib.h>

#include <windows.h>
#include <sys/stat.h>

int fileTest1(int argc, char *argv[])
{
    FILE * pFile;
    long lSize;
    char * buffer;
    size_t result;

    pFile = fopen ( "c:/1.txt.lnk" , "rb" );
    if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    rewind (pFile);

    // allocate memory to contain the whole file:
    buffer = (char*) malloc (sizeof(char)*lSize);
    if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

    // copy the file into the buffer:
    result = fread (buffer,1,lSize,pFile);
    if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

    /* the whole file is now loaded in the memory buffer. */

    printf(buffer);

    // terminate
    fclose (pFile);
    free (buffer);
    return 0;
}

int fileTest2(int argc, char *argv[])
{
    char * sFilePath = "COM1";
    struct _stat stat1;
    int iResult = _stat(sFilePath, &stat1);
    printf("%d", stat1.st_mode);
    return 0;
}

int fileTest3(int argc, char *argv[])
{
//    char szFind[MAX_PATH];

    WIN32_FIND_DATA FindFileData;
    char * sFilePath = "c:/test";
    HANDLE hFind=::FindFirstFile(sFilePath,&FindFileData);
    printf("%d\n", FindFileData.dwFileAttributes);
    return 0;
}


int main1(int argc, char *argv[])
{
    return fileTest3(argc, argv);
}

