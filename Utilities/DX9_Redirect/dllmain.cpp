
#ifdef _DEBUG
#	pragma comment(linker, "/export:Direct3DCreate9=Voodoo_DX9_d.Direct3DCreate9")
#else
#	pragma comment(linker, "/export:Direct3DCreate9=Voodoo_DX9.Direct3DCreate9")
#endif

/*
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern "C" __declspec(dllexport) void * Direct3DCreate9(unsigned int sdkVersion)
{	// Get the Voodoo location from the registry and load the core
	HKEY VoodooPathKey;

	LONG result = RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\VoodooShader", 0, KEY_QUERY_VALUE, &VoodooPathKey);

	if ( result != ERROR_SUCCESS )
	{
		result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\VoodooShader", 0, KEY_QUERY_VALUE, &VoodooPathKey);

		if ( result != ERROR_SUCCESS )
		{
			char error[4096];
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, result, 0, error, 4096, NULL);
			MessageBoxA(NULL, error, "Voodoo DX9 Hook Error 1", MB_ICONERROR);
			return NULL;
			//Throw("Voodoo DX9 Hook: Unable to find core location.", NULL);
		}
	}

	DWORD valueType, valueSize;
	char valuePath[MAX_PATH];
	result = RegQueryValueEx(VoodooPathKey, "Path", NULL, &valueType, (BYTE*)valuePath, &valueSize);

	if ( result != ERROR_SUCCESS )
	{
		char error[4096];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, result, 0, error, 4096, NULL);
		MessageBoxA(NULL, error, "Voodoo DX9 Hook Error 2", MB_ICONERROR);
		return NULL;
		//Throw("Voodoo DX9 Hook: Unable to find path.", NULL);
	}

	strcat(valuePath, "\\bin\\");

	SetDllDirectory(valuePath);

	char libraryFile[MAX_PATH];
	strcpy(libraryFile, valuePath);
#ifdef _DEBUG
	strcat(libraryFile, "Voodoo_DX9_d.dll");
#else
	strcat(libraryFile, "Voodoo_DX9.dll");
#endif

	HMODULE library = LoadLibrary(libraryFile);

	if ( !library )
	{
		char error[4096];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, result, 0, error, 4096, NULL);
		MessageBoxA(NULL, error, "Voodoo DX9 Hook Error 3", MB_ICONERROR);
		return NULL;
		//Throw("Voodoo DX9 Hook: Unable to load library.", NULL);
	}

	typedef void * (__stdcall *InitFunc)(unsigned int);
	InitFunc initFunc = (InitFunc)GetProcAddress(library, "Direct3DCreate9");

	void * obj = (*initFunc)(sdkVersion);

	return obj;
}
*/