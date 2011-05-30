// VoodooHookSystem.h : Declaration of the CVoodooHookSystem

#pragma once
#include "Core_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>

#include "Common.hpp"

using namespace ATL;

// CVoodooHookSystem

class ATL_NO_VTABLE CVoodooHookSystem :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CVoodooHookSystem, &CLSID_VoodooHookSystem>,
    public IDispatchImpl<IVoodooHookSystem, &IID_IVoodooHookSystem, &LIBID_Voodoo_Core, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
    CVoodooHookSystem();
    virtual ~CVoodooHookSystem();
    static IVoodooHookSystem * Create(IVoodooCore * pCore);

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    HRESULT FinalConstruct()
    {
        return S_OK;
    }

    void FinalRelease()
    {
    }

DECLARE_REGISTRY_RESOURCEID(IDR_VOODOOHOOKSYSTEM)

DECLARE_NOT_AGGREGATABLE(CVoodooHookSystem)

BEGIN_COM_MAP(CVoodooHookSystem)
    COM_INTERFACE_ENTRY(IVoodooHookSystem)
    COM_INTERFACE_ENTRY(IVoodooPlugin)
    COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IVoodooHookSystem
public:

    virtual HRESULT STDMETHODCALLTYPE Initialize(IVoodooCore *pCore);
    virtual HRESULT STDMETHODCALLTYPE get_Core(IVoodooCore **ppCore);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE Add( 
        /* [in] */ BSTR pName,
        /* [in] */ FunctionPtr pSource,
        /* [in] */ FunctionPtr pDest);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE Remove( 
        /* [in] */ BSTR pName);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE RemoveAll( void);

private:
    ULONG m_Refrs;
    CComBSTR m_Name;
    IVoodooCore * m_Core;
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooHookSystem), CVoodooHookSystem)
