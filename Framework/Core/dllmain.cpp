// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
#include "Core_i.h"
#include "dllmain.h"
#include "compreg.h"
#include "xdlldata.h"

CCoreModule _AtlModule;

class CCoreApp : public CWinApp
{
public:

// Overrides
    virtual BOOL InitInstance();
    virtual int ExitInstance();

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CCoreApp, CWinApp)
END_MESSAGE_MAP()

CCoreApp theApp;

BOOL CCoreApp::InitInstance()
{
#ifdef _MERGE_PROXYSTUB
    if (!PrxDllMain(m_hInstance, DLL_PROCESS_ATTACH, NULL))
        return FALSE;
#endif
    return CWinApp::InitInstance();
}

int CCoreApp::ExitInstance()
{
    return CWinApp::ExitInstance();
}
