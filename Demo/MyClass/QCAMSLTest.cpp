#include "../StdAfx.h"
#include "QCAMSLTest.h"
#include "QCAMSLDLL.h"


BOOL CQCAMSL::Import_QCAMSL(LPSTR lpszRunInfo)
{
    return TRUE;
}

BOOL CQCAMSL::GetLibraryVersion(LPSTR lpszRunInfo)
{
    char sLibraryVersion[50];

    // Get the library version
    QLIB_GetLibraryVersion(sLibraryVersion);
    strcpy(lpszRunInfo, sLibraryVersion);

    return TRUE;
}
