// EHHookSystem.h : Declaration of the CEHHookSystem

#pragma once
#include "EHHook_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>

using namespace ATL;

// CEHHookSystem

class ATL_NO_VTABLE CEHHookSystem :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CEHHookSystem, &CLSID_EHHookSystem>,
	public IDispatchImpl<IVoodooHookSystem, &IID_IVoodooHookSystem, &LIBID_EHHookLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CEHHookSystem()
	{
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_EHHOOKSYSTEM)

DECLARE_NOT_AGGREGATABLE(CEHHookSystem)

BEGIN_COM_MAP(CEHHookSystem)
    COM_INTERFACE_ENTRY(IVoodooHookSystem)
    COM_INTERFACE_ENTRY(IVoodooPlugin)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

public:
    // IVoodooPlugin
    virtual HRESULT STDMETHODCALLTYPE Initialize(IVoodooCore *pCore);
    virtual HRESULT STDMETHODCALLTYPE get_Core(IVoodooCore **ppCore);
    // IVoodooHookSystem
    /**
     * Install a hook at the specified function.
     *
     * @param pName The name for the hook.
     * @param pSource The point to install the hook at.
     * @param pDest The function to redirect execution into.
     * @return VSF_OK if the hook installed successfully, VSFERR_DUPLICATE_NAME if a hook with this
     *    name already exists, VSFERR_INVALID_ARG if any arguments are null or otherwise invalid.
     *
     * @note The name is often the name of the function in src (&func) for
     *    simplicities sake. 
     * @warning The calling convention of src and dest must be identical. You can use 
     *    <code>__declspec(naked)</code> to handle this yourself.
     */
    virtual HRESULT STDMETHODCALLTYPE Add(BSTR pName, FunctionPtr pSource, FunctionPtr pDest);

    /**
     * Removes a single hook.
     *
     * @param pName The name of the hook to remove.
     * @return VSF_OK if the hook was successfully removed, VSFERR_INVALID_ARG if pName is null,
     *    VSFOK_NOT_FOUND if the hook does not exist (removing a non-existent hook is not
     *    considered an error, as the end result of the call is the same as intended).
     * 
     * @warning <em>Do not</em>, under any circumstances, remove a hook while execution is passing 
     *    through the trampoline function. This can cause the process to crash. Until I replace 
     *    the hook handler, be careful!
     */
    virtual HRESULT STDMETHODCALLTYPE Remove(BSTR pName);

    /**
     * Removes all hooks.
     * 
     * @return VSF_OK if all hooks removed successfully.
     */
    virtual HRESULT STDMETHODCALLTYPE RemoveAll(void);

private:
    ULONG m_Refrs;
    IVoodooCore * m_Core;
    CComPtr<IVoodooLogger> m_Logger;
    CMap<CComBSTR, CComBSTR, TRACED_HOOK_HANDLE*, TRACED_HOOK_HANDLE*> m_Hooks;
};

OBJECT_ENTRY_AUTO(__uuidof(EHHookSystem), CEHHookSystem)
