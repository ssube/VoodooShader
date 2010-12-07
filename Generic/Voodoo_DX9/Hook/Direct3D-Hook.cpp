// Direct3D-Hook.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "apihijack.h"
#include "Direct3D-Hook.h"
#include "MyDirect3D9.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

HINSTANCE hDLL;

// Function pointer types.
typedef IDirect3D9* (WINAPI *Direct3DCreate9_t)(UINT sdk_version);

// Function prototypes.
IDirect3D9* WINAPI MyDirect3DCreate9(UINT sdk_version);

// Hook structure.
enum
{
	D3DFN_Direct3DCreate9 = 0
};

SDLLHook D3DHook = 
{
	"D3D9.DLL",
	false, NULL,		// Default hook disabled, NULL function pointer.
	{
		{ "Direct3DCreate9", MyDirect3DCreate9},
		{ NULL, NULL }
	}
};

// Hook function.
IDirect3D9* WINAPI MyDirect3DCreate9(UINT sdk_version)
{
	// Let the world know we're working.
	MessageBeep(MB_ICONINFORMATION);

	OutputDebugString( "Direct3D-Hook: MyDirect3DCreate9 called.\n" );

	Direct3DCreate9_t old_func = (Direct3DCreate9_t) D3DHook.Functions[D3DFN_Direct3DCreate9].OrigFn;
	IDirect3D9* d3d = old_func(sdk_version);
	
	return d3d? new MyDirect3D9(d3d) : 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
					   DWORD  ul_reason_for_call,
					   LPVOID lpReserved
					 )
{
	if(ul_reason_for_call == DLL_PROCESS_ATTACH)  // When initializing....
	{
		hDLL = hModule;
		// We don't need thread notifications for what we're doing.  Thus, get
		// rid of them, thereby eliminating some of the overhead of this DLL
		DisableThreadLibraryCalls( hModule );
		
		char targetProcess[512];
		DWORD count = 512;

		HKEY appKey = 0;
		if(ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Direct3D-Hook", 0, KEY_QUERY_VALUE,
			&appKey))
		{
			if(ERROR_SUCCESS == RegQueryValueEx(appKey, 0, 0, 0, reinterpret_cast<BYTE*>(targetProcess), &count))
			{
				char process[512];
				GetModuleFileName(GetModuleHandle(0), process, sizeof(process));
				PathStripPath(process);
				
				if(_strnicmp(targetProcess, process, 512) == 0)
					HookAPICalls(&D3DHook);
			}

			RegCloseKey(appKey);
		}
	}

	return TRUE;
}

// This segment must be defined as SHARED in the .DEF
#pragma data_seg (".HookSection")		
// Shared instance for all processes.
HHOOK hHook = NULL;	
#pragma data_seg ()

D3DHOOK_API LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam) 
{
    return CallNextHookEx( hHook, nCode, wParam, lParam); 
}

D3DHOOK_API void InstallHook()
{
    OutputDebugString( "D3DHOOK hook installed.\n" );
    hHook = SetWindowsHookEx( WH_CBT, HookProc, hDLL, 0 ); 
}

D3DHOOK_API void RemoveHook()
{
    OutputDebugString( "D3DHOOK hook removed.\n" );
    UnhookWindowsHookEx( hHook );
}


#ifdef _MANAGED
#pragma managed(pop)
#endif

