#pragma once

#include "../CommFunc.h"

class CCUSTOMIZE_TEST
{

    public:
        CCUSTOMIZE_TEST(void);
    public:
        ~CCUSTOMIZE_TEST(void);      

    public:
        enum HANDLE_TYPE
        {
            PIPE = 1,
            COM  = 2,
            SOCKET = 3
        };

        typedef struct _CMD_Struct
        {
            int	PipeID;
            int timeOut; 
            char cmd[32];
            char returnPass[32];
            char returnFail[32];
        }CMD_Struct, pCMD_Struct;

        typedef struct _Task
        {
            HANDLE_TYPE taskInfo;
            CMD_Struct cmdInfo;
        }Task, *pTask;

        CString file;
        Task task;
        CArray <CString, CString&> ker_word;
        CArray <Task, Task&> MVT_Task;

        BOOL Clean();
        BOOL SetParameter(LPSTR ParameterName,LPSTR ParameterValue);
        BOOL PreTest(LPSTR lpszRunInfo);
        BOOL Test(BOOL bRetry, LPSTR lpszRunInfo, TEMPLATE_PARA *templatePara);
};