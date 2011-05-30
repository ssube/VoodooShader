// VoodooFileSystem.h : Declaration of the CVoodooFileSystem

#pragma once
#include "Core_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>

#include "Common.hpp"

using namespace ATL;

// CVoodooFileSystem

class ATL_NO_VTABLE CVoodooFileSystem :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CVoodooFileSystem, &CLSID_VoodooFileSystem>,
    public IDispatchImpl<IVoodooFileSystem, &IID_IVoodooFileSystem, &LIBID_Voodoo_Core, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
    CVoodooFileSystem();
    virtual ~CVoodooFileSystem();
    static IVoodooFileSystem * Create(IVoodooCore * pCore);

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    HRESULT FinalConstruct()
    {
        return S_OK;
    }

    void FinalRelease()
    {
    }

DECLARE_REGISTRY_RESOURCEID(IDR_VOODOOFILESYSTEM)

DECLARE_NOT_AGGREGATABLE(CVoodooFileSystem)

BEGIN_COM_MAP(CVoodooFileSystem)
    COM_INTERFACE_ENTRY(IVoodooFileSystem)
    COM_INTERFACE_ENTRY(IVoodooPlugin)
    COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()




// IVoodooFileSystem
public:
    virtual HRESULT STDMETHODCALLTYPE Initialize(IVoodooCore *pCore);
    virtual HRESULT STDMETHODCALLTYPE get_Core(IVoodooCore **ppCore);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE AddDirectory( 
        /* [in] */ BSTR pPath);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE RemoveDirectory( 
        /* [in] */ BSTR pPath);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE FindFile( 
        /* [in] */ BSTR pPath,
        /* [retval][out] */ IVoodooFile **ppFile);

private:
    ULONG m_Refrs;
    CComBSTR m_Name;
    IVoodooCore * m_Core;
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooFileSystem), CVoodooFileSystem)
