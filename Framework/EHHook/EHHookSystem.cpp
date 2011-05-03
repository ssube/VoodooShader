// EHHookSystem.cpp : Implementation of CEHHookSystem

#include "stdafx.h"
#include "EHHookSystem.h"
#include "VoodooSupport.hpp"
#include "Hook_Version.hpp"

// CEHHookSystem
STDMETHODIMP CEHHookSystem::QueryInterface(REFIID iid, void ** pp) throw()
{
    if ( pp == NULL )
    {
        return E_POINTER;
    } else if ( iid == IID_IUnknown || iid == IID_IVoodooPlugin || iid == IID_IVoodooHookSystem ) {
        this->AddRef();
        *pp = this;
        return S_OK;
    } else {
        *pp = NULL;
        return E_NOINTERFACE;
    }
}

STDMETHODIMP_(ULONG) CEHHookSystem::AddRef()
{
    return (++m_Refrs);
}

STDMETHODIMP_(ULONG) CEHHookSystem::Release()
{
    --m_Refrs;
    if ( m_Refrs == 0 )
    {
        delete this;
        return 0;
    } else {
        return m_Refrs;
    }
}

STDMETHODIMP CEHHookSystem::get_Core(IVoodooCore ** ppCore)
{
    if ( ppCore == NULL ) return VSFERR_INVALID_ARG;

    *ppCore = m_Core;
    return VSF_OK;
}

STDMETHODIMP CEHHookSystem::Initialize(IVoodooCore *pCore)
{
    if ( pCore == NULL ) return VSFERR_INVALID_ARG;

    m_Core = pCore;
    m_Core->get_Logger(&m_Logger);

    m_ThreadCount = 1;
    m_ThreadIDs = new ULONG[1];
    m_ThreadIDs[0] = 0;

    LhSetGlobalInclusiveACL(m_ThreadIDs, m_ThreadCount);

    m_Logger->Log(LL_Info, VOODOO_HOOK_NAME, L"Created hook manager.", NULL);

    return VSF_OK;
}

STDMETHODIMP CEHHookSystem::Add(BSTR pName, FunctionPtr src, FunctionPtr dest)
{
    if ( m_Hooks.PLookup(pName) != NULL )
    {
        return VSFERR_DUP_NAME;
    }

    LogFormat
    (
        m_Logger, LL_Debug, 
        VOODOO_HOOK_NAME, L"Creating hook %s. Redirecting function %p to %p.",
        pName, src.Address, dest.Address
    );

    HookPtr hookHandle(new HOOK_TRACE_INFO());

    DWORD result = LhInstallHook((void*)src.Address, (void*)dest.Address, NULL, hookHandle.get());

    if ( ( result != 0 ) || ( hookHandle == NULL ) )
    {
        LogFormat
        (
            m_Logger, LL_Debug, 
            VOODOO_HOOK_NAME, L"Error %u creating hook %s (%p, %p).",
            result, pName, src.Address, dest.Address
        );

        return VSF_FAIL;
    } else {
        LhSetInclusiveACL(m_ThreadIDs, m_ThreadCount, hookHandle.get());

        m_Hooks.SetAt(pName, hookHandle);

        return VSF_OK;
    }
}

STDMETHODIMP CEHHookSystem::Remove(BSTR pName)
{
    LogFormat
    (
        m_Logger, LL_Debug,
        VOODOO_HOOK_NAME, L"Removing hook %s.",
        pName
    );

    HookMap::CPair * hook = m_Hooks.PLookup(pName);
    if ( hook != NULL )
    {
        return VSF_OK;
    } else {
        HookPtr tracedHandle = hook->value;

        DWORD result = LhUninstallHook(tracedHandle.get());

        if ( result != 0 )
        {
            LogFormat
            (
                m_Logger, LL_Error,
                VOODOO_HOOK_NAME, L"Error %u removing hook %s.",
                result, pName
            );
            return VSF_FAIL;
        } else {
            m_Hooks.RemoveKey(pName);
            return VSF_OK;
        }
    }
}

STDMETHODIMP CEHHookSystem::RemoveAll()
{
    LhUninstallAllHooks();
    LhWaitForPendingRemovals();

    m_Hooks.RemoveAll();

    return VSF_OK;
}