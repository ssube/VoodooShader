// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
#include "DX89_i.h"
#include "dllmain.h"
#include "compreg.h"
#include "xdlldata.h"

CDX89Module _AtlModule;

class CDX89App : public CWinApp
{
public:

// Overrides
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CDX89App, CWinApp)
END_MESSAGE_MAP()

CDX89App theApp;

BOOL CDX89App::InitInstance()
{
#ifdef _MERGE_PROXYSTUB
	if (!PrxDllMain(m_hInstance, DLL_PROCESS_ATTACH, NULL))
		return FALSE;
#endif
	return CWinApp::InitInstance();
}

int CDX89App::ExitInstance()
{
	return CWinApp::ExitInstance();
}
