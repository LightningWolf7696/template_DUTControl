#pragma once

#include "../CommFunc.h"

class CPipeTest
{

    public:
        CPipeTest(void);
    public:
        ~CPipeTest(void);
    
    public:
         BOOL PipeLineOpen(int *PipeLineID, LPSTR lpszCmd, LPSTR lpszRunInfo);
};