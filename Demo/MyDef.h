
typedef struct _GLOBAL_INFO{
	int		PipeID;
	funcGetActualReport fGetActualReport;
	funcQueryGpib  fQueryGpib;
	funcReadGpib   fReadGpib;
	funcWriteGpib  fWriteGpib;
	funcAssignVariable fAssignVariable;
	int		GpibID;
}GLOBAL_INFO, *pGLOBAL_INFO;

typedef struct _Template_Para
{
    funcGetMessage cbListMessage;
    funcGetMessage cbXmlMessage;
    funcGetMessage cbSfcsSpecMessage; 
    funcGetMessage cbSfcsResultMessage;
    ERRORREPORT *ErrorReport;
}TEMPLATE_PARA, *pTEMPLATE_PARA;

extern GLOBAL_INFO g_GlobalInfo;
extern TEMPLATE_PARA g_TemplatePara;