// VoodooImage.h : Declaration of the CVoodooImage

#pragma once
#include "Core_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>

#include "Common.hpp"

#include "VoodooFile.h"

using namespace ATL;



// CVoodooImage

class ATL_NO_VTABLE CVoodooImage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVoodooImage, &CLSID_VoodooImage>,
    public IDispatchImpl<IVoodooImage, &IID_IVoodooImage, &LIBID_Voodoo_Core, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CVoodooImage();
    virtual ~CVoodooImage();
    static IVoodooImage * Create(IVoodooCore * pFileSystem, BSTR pPath);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_VOODOOIMAGE)

DECLARE_NOT_AGGREGATABLE(CVoodooImage)

BEGIN_COM_MAP(CVoodooImage)
	COM_INTERFACE_ENTRY(IVoodooImage)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IVoodooFile
virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
    /* [retval][out] */ LPBSTR pName);

virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Core( 
    /* [retval][out] */ IVoodooCore **ppCore);

virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Path( 
    /* [retval][out] */ LPBSTR pPath);

virtual /* [id] */ HRESULT STDMETHODCALLTYPE Open( 
    /* [in] */ DWORD Mode);

virtual /* [id] */ HRESULT STDMETHODCALLTYPE Close(void);

virtual /* [id] */ HRESULT STDMETHODCALLTYPE Read( 
    /* [out][in] */ int *Length,
    /* [size_is][out] */ byte Buffer[  ]);

virtual /* [id] */ HRESULT STDMETHODCALLTYPE Write( 
    /* [in] */ int Length,
    /* [size_is][in] */ byte Buffer[  ]);

// IVoodooImage
public:
    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Desc( 
        /* [retval][out] */ TextureDesc *pDesc);

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Data( 
        /* [in] */ TextureRegion Region,
        /* [out][in] */ int *pSize,
        /* [size_is][retval][out] */ byte ppBuffer[  ]);

private:
    ULONG m_Refrs;
    CComBSTR m_Name;
    IVoodooCore * m_Core;
    CComBSTR m_Path;
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooImage), CVoodooImage)
