
#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void * WINAPI Voodoo3DCreate8(UINT sdkVersion)
{	
	// Get the Voodoo location from the registry and load the core
	HKEY VoodooPathKey;

	LONG result = RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\VoodooShader", 0, KEY_QUERY_VALUE, &VoodooPathKey);

	if ( result != ERROR_SUCCESS )
	{
		result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\VoodooShader", 0, KEY_QUERY_VALUE, &VoodooPathKey);

		if ( result != ERROR_SUCCESS )
		{
			char error[4096];
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, result, 0, error, 4096, NULL);
			MessageBoxA(NULL, error, "Voodoo Gem Hook Error 1", MB_ICONERROR);
			return NULL;
		}
	}

	DWORD valueSize = MAX_PATH;
	char valuePath[MAX_PATH]; memset(valuePath, 0, MAX_PATH);

	result = RegQueryValueEx(VoodooPathKey, "Path", NULL, NULL, (BYTE*)valuePath, &valueSize);

	if ( result != ERROR_SUCCESS )
	{
		char error[4096];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, result, 0, error, 4096, NULL);
		MessageBoxA(NULL, error, "Voodoo Gem Hook Error 2", MB_ICONERROR);
		return NULL;
	}

	strcat_s(valuePath, MAX_PATH, "\\bin\\");

	SetDllDirectory(valuePath);

	char libraryFile[MAX_PATH];
	strcpy_s(libraryFile, MAX_PATH, valuePath);
#ifdef _DEBUG
	strcat_s(libraryFile, MAX_PATH, "Voodoo_Gem_d.dll");
#else
	strcat_s(libraryFile, MAX_PATH, "Voodoo_Gem.dll");
#endif

	HMODULE library = LoadLibrary(libraryFile);

	if ( !library )
	{
		char error[4096];
		sprintf_s(error, 4096, "Could not load DLL: %s", libraryFile);
		MessageBoxA(NULL, error, "Voodoo Gem Hook Error 3", MB_ICONERROR);
		return NULL;
	}

	typedef void * (__stdcall * InitFuncType)(UINT);
	InitFuncType initFunc = (InitFuncType)GetProcAddress(library, "?Direct3DCreate8@@YGPAXI@Z");

	if ( !initFunc )
	{
		MessageBoxA(NULL, "Could not find init func.", "Voodoo Gem Hook Error 4", MB_ICONERROR);
		return NULL;
	}

	void * obj = (*initFunc)(sdkVersion);

	return obj;
}

#pragma comment(linker, "/export:Direct3DCreate8=?Voodoo3DCreate8@@YGPAXI@Z")
