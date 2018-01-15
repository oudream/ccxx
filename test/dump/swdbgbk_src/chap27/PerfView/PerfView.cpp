// PerfView.cpp : Defines the entry point for the console application.
//
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#define TOTALBYTES    8192
#define BYTEINCREMENT 1024

LPSTR lpNameStrings;
LPSTR *lpNamesArray;

/*****************************************************************
 *                                                               *
 * Functions used to navigate through the performance data.      *
 *                                                               *
 *****************************************************************/

PPERF_OBJECT_TYPE FirstObject( PPERF_DATA_BLOCK PerfData )
{
    return( (PPERF_OBJECT_TYPE)((PBYTE)PerfData + 
        PerfData->HeaderLength) );
}

PPERF_OBJECT_TYPE NextObject( PPERF_OBJECT_TYPE PerfObj )
{
    return( (PPERF_OBJECT_TYPE)((PBYTE)PerfObj + 
        PerfObj->TotalByteLength) );
}

PPERF_INSTANCE_DEFINITION FirstInstance( PPERF_OBJECT_TYPE PerfObj )
{
    return( (PPERF_INSTANCE_DEFINITION)((PBYTE)PerfObj + 
        PerfObj->DefinitionLength) );
}

PPERF_INSTANCE_DEFINITION NextInstance( 
    PPERF_INSTANCE_DEFINITION PerfInst )
{
    PPERF_COUNTER_BLOCK PerfCntrBlk;

    PerfCntrBlk = (PPERF_COUNTER_BLOCK)((PBYTE)PerfInst + 
        PerfInst->ByteLength);

    return( (PPERF_INSTANCE_DEFINITION)((PBYTE)PerfCntrBlk + 
        PerfCntrBlk->ByteLength) );
}

PPERF_COUNTER_DEFINITION FirstCounter( PPERF_OBJECT_TYPE PerfObj )
{
    return( (PPERF_COUNTER_DEFINITION) ((PBYTE)PerfObj + 
        PerfObj->HeaderLength) );
}

PPERF_COUNTER_DEFINITION NextCounter( 
    PPERF_COUNTER_DEFINITION PerfCntr )
{
    return( (PPERF_COUNTER_DEFINITION)((PBYTE)PerfCntr + 
        PerfCntr->ByteLength) );
}

/*****************************************************************
 *                                                               *
 * Load the counter and object names from the registry to the    *
 * global variable lpNamesArray.                                 *
 *                                                               *
 *****************************************************************/

BOOL GetNameStrings( )
{
    HKEY hKeyPerflib;      // handle to registry key
    HKEY hKeyPerflib009;   // handle to registry key
    DWORD dwMaxValueLen;   // maximum size of key values
    DWORD dwBuffer;        // bytes to allocate for buffers
    DWORD dwBufferSize = sizeof(DWORD);  // size of dwBuffer
    LPSTR lpCurrentString; // pointer for enumerating data strings
    DWORD dwCounter;       // current counter index
    LONG lResult;          // return value

// Get the number of Counter items.

    if( RegOpenKeyEx( HKEY_LOCAL_MACHINE,
        "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Perflib",
        0,
        KEY_READ,
        &hKeyPerflib) != ERROR_SUCCESS
      )
        return FALSE;

    lResult = RegQueryValueEx( hKeyPerflib,
        "Last Counter",
        NULL,
        NULL,
        (LPBYTE) &dwBuffer,
        &dwBufferSize );

    RegCloseKey( hKeyPerflib );

    if( lResult != ERROR_SUCCESS )
        return FALSE;

// Allocate memory for the names array.

    lpNamesArray = (char**)malloc( (dwBuffer+1) * sizeof(LPSTR) );

    if( lpNamesArray == NULL )
        return FALSE;

// Open the key containing the counter and object names.

    if( RegOpenKeyEx( HKEY_LOCAL_MACHINE,
    "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Perflib\\009",
        0,
        KEY_READ,
        &hKeyPerflib009) != ERROR_SUCCESS
      )
        return FALSE;

// Get the size of the largest value in the key (Counter or Help).

    if( RegQueryInfoKey( hKeyPerflib009,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        &dwMaxValueLen,
        NULL,
        NULL) != ERROR_SUCCESS
      )
        return FALSE;

// Allocate memory for the counter and object names.

    dwBuffer = dwMaxValueLen + 1;

    lpNameStrings = (char*)malloc( dwBuffer * sizeof(CHAR) );

    if (lpNameStrings == NULL)
    {
        free( lpNamesArray );
        return FALSE;
    }

// Read the counter value.

    lResult = RegQueryValueEx( hKeyPerflib009,
        "Counters",
        NULL,
        NULL,
        (unsigned char*)lpNameStrings, &dwBuffer );

    RegCloseKey( hKeyPerflib009 ); 

    if( lResult != ERROR_SUCCESS )
        return FALSE;

// Load names into an array, by index.

    for( lpCurrentString = lpNameStrings; *lpCurrentString;
         lpCurrentString += (lstrlen(lpCurrentString)+1) )
    {
        dwCounter = atol( lpCurrentString );

        lpCurrentString += (lstrlen(lpCurrentString)+1);

        lpNamesArray[dwCounter] = (LPSTR) lpCurrentString;
    }

   return TRUE;
}

/*****************************************************************
 *                                                               *
 * Display the indexes and/or names for all performance objects, *
 * instances, and counters.                                      *
 *                                                               *
 *****************************************************************/

int main()
{
    PPERF_DATA_BLOCK PerfData = NULL;
    PPERF_OBJECT_TYPE PerfObj;
    PPERF_INSTANCE_DEFINITION PerfInst;
    PPERF_COUNTER_DEFINITION PerfCntr, CurCntr;
    PPERF_COUNTER_BLOCK PtrToCntr;
    DWORD BufferSize = TOTALBYTES;
    DWORD i, j, k;

// Get the name strings through the registry.

    if( !GetNameStrings( ) )
       return FALSE;

// Allocate the buffer for the performance data.

    PerfData = (PPERF_DATA_BLOCK) malloc( BufferSize );

    if( PerfData == NULL )
       return FALSE;

    while( RegQueryValueEx( HKEY_PERFORMANCE_DATA,
                               "234",//"Global",//
                               NULL,
                               NULL,
                               (LPBYTE) PerfData,
                               &BufferSize ) == ERROR_MORE_DATA )
    {
    // Get a buffer that is big enough.

        BufferSize += BYTEINCREMENT;
        PerfData = (PPERF_DATA_BLOCK) realloc( PerfData, BufferSize );
    }

// Get the first object type.

    PerfObj = FirstObject( PerfData );

// Process all objects.

    for( i=0; i < PerfData->NumObjectTypes; i++ )
    {
    // Display the object by index and name.

        printf( "\nObject %ld: %s\n", PerfObj->ObjectNameTitleIndex,
            lpNamesArray[PerfObj->ObjectNameTitleIndex] );

    // Get the first counter.

        PerfCntr = FirstCounter( PerfObj );

        if( PerfObj->NumInstances > 0 )
        {
        // Get the first instance.

            PerfInst = FirstInstance( PerfObj );

        // Retrieve all instances.

            for( k=0; k < PerfObj->NumInstances; k++ )
            {
            // Display the instance by name.

                printf( "\n\tInstance %S: \n", 
                    (char *)((PBYTE)PerfInst + PerfInst->NameOffset));
                CurCntr = PerfCntr;

            // Retrieve all counters.

                for( j=0; j < PerfObj->NumCounters; j++ )
                {
                // Display the counter by index and name.

                    printf("\t\tCounter %ld: %s\n", 
                        CurCntr->CounterNameTitleIndex,
                        lpNamesArray[CurCntr->CounterNameTitleIndex]);

                // Get the next counter.

                    CurCntr = NextCounter( CurCntr );
                }

            // Get the next instance.

                PerfInst = NextInstance( PerfInst );
            }
        }
        else
        {
        // Get the counter block.

            PtrToCntr = (PPERF_COUNTER_BLOCK) ((PBYTE)PerfObj +
                            PerfObj->DefinitionLength );

        // Retrieve all counters.

            for( j=0; j < PerfObj->NumCounters; j++ )
            {
            // Display the counter by index and name.

                printf( "\tCounter %ld: %s\n", PerfCntr->CounterNameTitleIndex,
                    lpNamesArray[PerfCntr->CounterNameTitleIndex] );

            // Get the next counter.

                PerfCntr = NextCounter( PerfCntr );
            }
        }

    // Get the next object type.

        PerfObj = NextObject( PerfObj );
    }

    free( lpNamesArray );
    free( lpNameStrings );
    free( PerfData );

    return TRUE;
}

