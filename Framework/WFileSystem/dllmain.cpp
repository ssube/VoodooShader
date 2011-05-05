// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
#include "WFileSystem_i.h"
#include "dllmain.h"
#include "compreg.h"
#include "xdlldata.h"

CWFileSystemModule _AtlModule;

class CWFileSystemApp : public CWinApp
{
public:

// Overrides
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CWFileSystemApp, CWinApp)
END_MESSAGE_MAP()

CWFileSystemApp theApp;

BOOL CWFileSystemApp::InitInstance()
{
#ifdef _MERGE_PROXYSTUB
	if (!PrxDllMain(m_hInstance, DLL_PROCESS_ATTACH, NULL))
		return FALSE;
#endif
	return CWinApp::InitInstance();
}

int CWFileSystemApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}
