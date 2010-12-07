#pragma once

#ifdef DIRECT3DHOOK_EXPORTS
	#define D3DHOOK_API __declspec(dllexport)
#else
	#define D3DHOOK_API __declspec(dllimport)
#endif

D3DHOOK_API void InstallHook();
D3DHOOK_API void RemoveHook();
