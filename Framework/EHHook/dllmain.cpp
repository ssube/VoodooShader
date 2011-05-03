// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
#include "EHHook_i.h"
#include "dllmain.h"
#include "compreg.h"
#include "xdlldata.h"

CEHHookModule _AtlModule;

class CEHHookApp : public CWinApp
{
public:

// Overrides
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CEHHookApp, CWinApp)
END_MESSAGE_MAP()

CEHHookApp theApp;

BOOL CEHHookApp::InitInstance()
{
#ifdef _MERGE_PROXYSTUB
	if (!PrxDllMain(m_hInstance, DLL_PROCESS_ATTACH, NULL))
		return FALSE;
#endif
	return CWinApp::InitInstance();
}

int CEHHookApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}
