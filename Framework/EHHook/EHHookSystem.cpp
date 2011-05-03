// EHHookSystem.cpp : Implementation of CEHHookSystem

#include "stdafx.h"
#include "EHHookSystem.h"

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

STDMETHODIMP CEHHookSystem::Initialize(IVoodooCore *pCore)
{
    if ( pCore == NULL ) return VSFERR_INVALID_ARG;

    m_Core = pCore;
    m_Core->get_Logger(&m_Logger);

    ULONG ThreadCount = 1;
    ULONG ThreadIDs[1] = { 0 };

    LhSetGlobalInclusiveACL(ThreadIDs, ThreadCount);

    m_Logger->Log(LL_Info, VOODOO_HOOK_NAME, "Created hook manager.", NULL);
}

STDMETHODIMP CEHHookSystem::CreateHook(BSTR pName, FunctionPtr src, FunctionPtr dest)
{
    HookMap::iterator hook = mHooks.find(name);

    if ( hook != mHooks.end() )
    {
        Throw
            (
            VOODOO_HOOK_NAME, 
            "Attempted to create a hook with a duplicate name.", 
            mCore
            );
    }

    mCore->GetLogger()->Log
    (
        LL_Debug, 
        VOODOO_HOOK_NAME,
        "Creating hook %s. Redirecting function %p to %p.",
        name.c_str(), src, dest
    );

    TRACED_HOOK_HANDLE hookHandle = new HOOK_TRACE_INFO();

    DWORD result = LhInstallHook(src, dest, NULL, hookHandle);

    if ( ( result != 0 ) || ( hookHandle == NULL ) )
    {
        mCore->GetLogger()->Log
            (
            LL_Error,
            VOODOO_HOOK_NAME,
            "Error %u creating hook %s (%p, %p).",
            result, name.c_str(), src, dest
            );

        return false;
    } else {
        LhSetInclusiveACL(mThreadIDs, mThreadCount, hookHandle);

        mHooks[name] = hookHandle;

        return true;
    }
}

STDMETHODIMP CEHHookSystem::RemoveHook(std::string name)
{
    HookMap::iterator hook = mHooks.find(name);

    mCore->GetLogger()->Log
    (
        LL_Debug,
        VOODOO_HOOK_NAME,
        "Removing hook %s.",
        name.c_str()
    );

    if ( hook != mHooks.end() )
    {
        TRACED_HOOK_HANDLE tracedHandle = (TRACED_HOOK_HANDLE)hook->second;

        DWORD result = LhUninstallHook(tracedHandle);
        delete tracedHandle;

        if ( result != 0 )
        {
            mCore->GetLogger()->Log
                (
                LL_Error,
                VOODOO_HOOK_NAME,
                "Error %u removing hook %s.",
                result, name.c_str()
                );

            return true;
        } else {
            mHooks.erase(hook);

            return false;
        }
    } else {
        Throw
        (
            VOODOO_HOOK_NAME,
            "Trying to remove hook that does not exist.", 
            mCore
        );
    }
}

STDMETHODIMP CEHHookSystem::RemoveAll()
{
    LhUninstallAllHooks();
    LhWaitForPendingRemovals();

    m_Hooks.RemoveAll();
}