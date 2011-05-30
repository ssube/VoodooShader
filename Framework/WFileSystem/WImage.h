// WImage.h : Declaration of the CWImage

#pragma once
#include "WFileSystem_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>

using namespace ATL;

// CWImage

class ATL_NO_VTABLE CWImage :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CWImage, &CLSID_WImage>,
    public IDispatchImpl<IVoodooImage, &IID_IVoodooImage, &LIBID_WFileSystemLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
    static IVoodooFile * Create(CStringW Filename);

    CWImage();
    virtual ~CWImage();

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    HRESULT FinalConstruct()
    {
        return S_OK;
    }

    void FinalRelease()
    {
    }

DECLARE_REGISTRY_RESOURCEID(IDR_WIMAGE)

DECLARE_NOT_AGGREGATABLE(CWImage)

BEGIN_COM_MAP(CWImage)
    COM_INTERFACE_ENTRY(IVoodooImage)
    COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IVoodooFile
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
virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Desc( 
    /* [retval][out] */ TextureDesc *pDesc);

virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Data( 
    /* [in] */ TextureRegion Region,
    /* [out][in] */ int *pSize,
    /* [size_is][retval][out] */ byte ppBuffer[  ]);

public:
    ULONG m_Refrs;
    CStringW m_Path;
    CFile m_File;
    ILuint m_Image;
};

OBJECT_ENTRY_AUTO(__uuidof(WImage), CWImage)
