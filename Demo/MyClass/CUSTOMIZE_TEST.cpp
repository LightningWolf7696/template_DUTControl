#include "../StdAfx.h"
#include "CUSTOMIZE_TEST.h"
#include <io.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>

CCUSTOMIZE_TEST::CCUSTOMIZE_TEST(void)
{

}

CCUSTOMIZE_TEST::~CCUSTOMIZE_TEST(void)
{

}

BOOL CCUSTOMIZE_TEST::Clean()
{
    file = "";
    memset(&task,0,sizeof(task));
    ker_word.RemoveAll();
    MVT_Task.RemoveAll();
    
    return TRUE;
}

BOOL CCUSTOMIZE_TEST::SetParameter(LPSTR ParameterName,LPSTR ParameterValue)
{	
   if(strcmp(ParameterName,_T("FILE")) == NULL)
        file = ParameterValue;
   else if(strcmp(ParameterName,_T("HANDLE")) == NULL)
   {
       if(strcmp(ParameterValue,_T("PIPE")) == NULL)
          task.taskInfo = PIPE;
       else if(strcmp(ParameterValue,_T("COM")) == NULL)
          task.taskInfo = COM;
       else if(strcmp(ParameterValue,_T("SOCKET")) == NULL)
          task.taskInfo = SOCKET;
       else
          return FALSE;
       MVT_Task.Add(task);
   }
   else if(strcmp(ParameterName,_T("CMD")) == NULL)
       strcpy(task.cmdInfo.cmd, ParameterValue);
   else if(strcmp(ParameterName,_T("RESPONSE_PASS")) == NULL)
       strcpy(task.cmdInfo.returnPass, ParameterValue);
   else if(strcmp(ParameterName,_T("RESPONSE_FAIL")) == NULL)
       strcpy(task.cmdInfo.returnFail, ParameterValue);
   else if(strcmp(ParameterName,_T("TIMEOUT")) == NULL)
       task.cmdInfo.timeOut = atoi(ParameterValue);
   return TRUE;
}

BOOL CCUSTOMIZE_TEST::PreTest(LPSTR lpszRunInfo)
{
    char szMessage[512];
    char szLocalPath[64];

    GetLocalDirectory(szLocalPath,szMessage);

    switch (task.taskInfo) 
    { 
        case PIPE:
        {
            if(!PipeLineOpen(&task.cmdInfo.PipeID,_T("cmd.exe"),lpszRunInfo))
                return FALSE;
            break;
        }
        default:
            break;  
    } 
    return TRUE;
}

BOOL CCUSTOMIZE_TEST::Test(BOOL bRetry, LPSTR lpszRunInfo, TEMPLATE_PARA *templatePara)
{
    char szMessage[PIPE_MAX_LENGTH];
    char szLocalPath[64];
    char szErrorTest[32];
    char szErrorTestDetail[128];

    GetLocalDirectory(szLocalPath,szMessage);

    switch (task.taskInfo) 
    { 
        case PIPE:
        {
           PipeLineCleanData(task.cmdInfo.PipeID, lpszRunInfo);
           if(!CCommFunc::PipeLineQueryData(task.cmdInfo.PipeID, task.cmdInfo.cmd, task.cmdInfo.returnPass, task.cmdInfo.timeOut, szMessage))
           {
               sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Query command is fail, [Command:%s, Response:%s]", task.cmdInfo.cmd, task.cmdInfo.returnPass);		
               sprintf_s(szErrorTest, sizeof(szErrorTest), "PipeLineQueryData");
               sprintf_s(szErrorTestDetail, sizeof(szErrorTestDetail), "[TaskInfo %d]: PipeLineQueryData", task.taskInfo);
               CCommFunc::OutputMsgToBoth(templatePara->cbListMessage, szMessage);
               goto T809;
           }
           CCommFunc::OutputMsgToBoth(templatePara->cbListMessage, szMessage);
           sprintf_s(lpszRunInfo, DLL_INFO_SIZE, "Query command is PASS, [Command:%s, Response:%s]", task.cmdInfo.cmd, task.cmdInfo.returnPass);
           if(!PipeLineClose(task.cmdInfo.PipeID, lpszRunInfo))            
               return FALSE;   
           break;
        }
        default:
            break;  
    }
    return TRUE;

T809:
    strcpy(templatePara->ErrorReport->ErrorCode, "T809");
    templatePara->ErrorReport->DisableSfcsLink = TRUE;
    templatePara->ErrorReport->TestResult=FALSE;
    strCopy(templatePara->ErrorReport->ErrorDetail,_T(szErrorTestDetail));	
    strCopy(templatePara->ErrorReport->FailedTest,_T(szErrorTest));
    strCopy(templatePara->ErrorReport->ErrorMsg,szMessage);
    return FALSE;
}
