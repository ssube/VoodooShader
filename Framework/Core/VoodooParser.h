// VoodooParser.h : Declaration of the CVoodooParser

#pragma once
#include "Core_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>

#include "Common.hpp"

using namespace ATL;

typedef CMap<BSTR, BSTR, BSTR, BSTR> Dictionary;

// CVoodooParser

class ATL_NO_VTABLE CVoodooParser :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVoodooParser, &CLSID_VoodooParser>,
	public IDispatchImpl<IVoodooParser, &IID_IVoodooParser, &LIBID_CoreLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CVoodooParser();
    virtual ~CVoodooParser();
    static IVoodooParser * Create(IVoodooCore * pCore);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_VOODOOPARSER)

DECLARE_NOT_AGGREGATABLE(CVoodooParser)

BEGIN_COM_MAP(CVoodooParser)
	COM_INTERFACE_ENTRY(IVoodooParser)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()




// IVoodooParser
public:
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE AddVariable( 
        /* [in] */ BSTR pName,
        /* [in] */ BSTR pValue,
        /* [in] */ BOOL System);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE RemoveVariable( 
        /* [in] */ BSTR pName);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE Parse( 
        /* [in] */ BSTR pString,
        /* [defaultvalue][in] */ ParseFlags Flags,
        /* [retval][out] */ LPBSTR pParsed);

private:
    HRESULT STDMETHODCALLTYPE ParseRaw(LPBSTR input, ParseFlags Flags, INT Depth, Dictionary * State);

    static const INT VarMaxDepth = 8;
    static const WCHAR VarDelimStart = '(';
    static const WCHAR VarDelimEnd   = ')';
    static const WCHAR VarDelimPre   = '$';

    UINT m_Refrs;
    IVoodooCore * m_Core;
    Dictionary m_Variables;
    Dictionary m_SysVariables;
};

OBJECT_ENTRY_AUTO(__uuidof(VoodooParser), CVoodooParser)
