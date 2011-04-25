// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
#include "XmlLogger_i.h"
#include "dllmain.h"
#include "compreg.h"
#include "xdlldata.h"

CXmlLoggerModule _AtlModule;

class CXmlLoggerApp : public CWinApp
{
public:

// Overrides
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CXmlLoggerApp, CWinApp)
END_MESSAGE_MAP()

CXmlLoggerApp theApp;

BOOL CXmlLoggerApp::InitInstance()
{
#ifdef _MERGE_PROXYSTUB
	if (!PrxDllMain(m_hInstance, DLL_PROCESS_ATTACH, NULL))
		return FALSE;
#endif
	return CWinApp::InitInstance();
}

int CXmlLoggerApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}
