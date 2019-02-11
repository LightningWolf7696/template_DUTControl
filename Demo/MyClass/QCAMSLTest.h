#pragma once

#include "../CommFunc.h"
#include "QMSL_Core.h"
#include "QLib_Defines.h"

class CQCAMSL
{
    public:
    CQCAMSL(void);
    public:
     ~CQCAMSL(void);

    public:
      BOOL Import_QCAMSL(LPSTR lpszRunInfo); 
      BOOL GetLibraryVersion(LPSTR lpszRunInfo);  
};