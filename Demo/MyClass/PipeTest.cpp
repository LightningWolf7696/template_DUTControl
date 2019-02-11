#include "../StdAfx.h"
#include "PipeTest.h"
#include <io.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>

#define BUFSIZE 4096 

HANDLE g_hChildStd_IN_Rd = NULL;
HANDLE g_hChildStd_IN_Wr = NULL;
HANDLE g_hChildStd_OUT_Rd = NULL;
HANDLE g_hChildStd_OUT_Wr = NULL;
HANDLE g_hInputFile = NULL;

CPipeTest::CPipeTest(void)
{

}

CPipeTest::~CPipeTest(void)
{

}


BOOL CPipeTest::PipeLineOpen(int *PipeLineID, LPSTR lpszCmd, LPSTR lpszRunInfo)
{
    char szMessage[512];
    SECURITY_ATTRIBUTES saAttr; 

    printf("\n->Start of parent execution.\n");

    // Set the bInheritHandle flag so pipe handles are inherited. 
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
    saAttr.bInheritHandle = TRUE; 
    saAttr.lpSecurityDescriptor = NULL;

    // Create a pipe for the child process's STDOUT. 
    if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0))
    {
        sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "CreatePipe is fail");
        return FALSE;
    }
        
    return TRUE;
}