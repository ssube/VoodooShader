// dllmain.cpp : Defines the entry point for the DLL application.

#include "Meta.hpp"
#include "Core.hpp"
#include "Version.hpp"

using namespace VoodooShader;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    UNREFERENCED_PARAMETER(lpReserved);
    UNREFERENCED_PARAMETER(hModule);

    Core * pc = Core::GetPrimary();

    if ( ul_reason_for_call == DLL_PROCESS_ATTACH && !pc )
    {
        pc = Core::Create();
        if ( pc == NULL )
        {
            return TRUE;
        }
        if ( lpReserved )
        {
            pc->Log(LL_Info, VOODOO_CORE_NAME, "Core has been statically loaded.");
        } else {
            pc->Log(LL_Info, VOODOO_CORE_NAME, "Core has been dynamically loaded.");
        }
    } else if ( ul_reason_for_call == DLL_PROCESS_DETACH && pc ) {
        if ( lpReserved )
        {
            pc->Log(LL_Info, VOODOO_CORE_NAME, "Process is terminating.");
        } else {
            pc->Log(LL_Info, VOODOO_CORE_NAME, "Core disconnected with FreeLibrary.");
        }
        Core::Destroy(pc);
    }

    return TRUE;
}

