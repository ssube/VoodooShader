
#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void * WINAPI Voodoo3DCreate8(UINT sdkVersion)
{	
#ifdef _DEBUG
	bool debug = true;
#else
	bool debug = false;
#endif

	HANDLE debugFile = CreateFileA("VOODOO_DEBUG", FILE_WRITE_DATA, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if ( debugFile != INVALID_HANDLE_VALUE )
	{
		debug = true;
		MessageBoxA(NULL, "Debug indicator file found. The hook will load in debug mode.", "Voodoo Gem Hook", MB_ICONWARNING);
		//CloseHandle(debugFile);
	}

	bool valueFound = false;
	DWORD valueSize = 4096;
	DWORD valueType = REG_NONE;
	char valuePath[4096];

	// Get the Voodoo location from the registry and load the core
	HKEY VoodooPathKey;

	LONG result = RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\VoodooShader", 0, KEY_QUERY_VALUE, &VoodooPathKey);

	if ( result == ERROR_SUCCESS )
	{
		if ( debug )
		{
			DWORD written;
			char msg[] = "Found HKCU\\SOFTWARE\\VoodooShader.\n";
			WriteFile(debugFile, msg, strlen(msg), &written, NULL);
		}

		result = RegQueryValueExA(VoodooPathKey, "Path", NULL, &valueType, (BYTE*)valuePath, &valueSize);

		if ( result == ERROR_SUCCESS )
		{
			if ( debug )
			{
				DWORD written;
				char msg[4096];
				sprintf_s(msg, "Loaded key from HKCU, value: %s\n", valuePath);
				WriteFile(debugFile, msg, strlen(msg), &written, NULL);
			}

			valueFound = true;
		} else {
			if ( debug )
			{
				DWORD written;
				char msg[4096];
				sprintf_s(msg, "Unable to load key from HKCU, error %d.\n", result);
				WriteFile(debugFile, msg, strlen(msg), &written, NULL);
			}
		}
	} else {
		if ( debug )
		{
			DWORD written;
			char msg[4096];
			sprintf_s(msg, "Unable to find key in HKCU, error %d.\n", valuePath);
			WriteFile(debugFile, msg, strlen(msg), &written, NULL);
		}
	}

	if ( !valueFound )
	{
		result = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\VoodooShader", 0, KEY_QUERY_VALUE, &VoodooPathKey);

		if ( result == ERROR_SUCCESS )
		{
			if ( debug )
			{
				DWORD written;
				char msg[] = "Found HKLM\\SOFTWARE\\VoodooShader.\n";
				WriteFile(debugFile, msg, strlen(msg), &written, NULL);
			}

			result = RegQueryValueExA(VoodooPathKey, "Path", NULL, &valueType, (BYTE*)valuePath, &valueSize);

			if ( result == ERROR_SUCCESS )
			{
				if ( debug )
				{
					DWORD written;
					char msg[4096];
					sprintf_s(msg, "Loaded key from HKLM, value: %s\n", valuePath);
					WriteFile(debugFile, msg, strlen(msg), &written, NULL);
				}

				valueFound = true;
			} else {
				if ( debug )
				{
					DWORD written;
					char msg[4096];
					sprintf_s(msg, "Unable to load key from HKLM, error %d.\n", result);
					WriteFile(debugFile, msg, strlen(msg), &written, NULL);
				}
			}
		} else {
			if ( debug )
			{
				DWORD written;
				char msg[4096];
				sprintf_s(msg, "Unable to find key in HKLM, error %d.\n", valuePath);
				WriteFile(debugFile, msg, strlen(msg), &written, NULL);
			}
		}
	}

	if ( !valueFound )
	{
		strcat_s(valuePath, MAX_PATH, "C:\\VoodooShader\\");
		if ( debug )
		{
			DWORD written;
			char msg[] = "Using default path.\n";
			WriteFile(debugFile, msg, strlen(msg), &written, NULL);
		}
	}

	strcat_s(valuePath, MAX_PATH, "\\bin\\");

	/*
	if ( SetDllDirectory(valuePath) )
	{
		if ( debug )
		{
			DWORD written;
			char msg[4096];
			sprintf_s(msg, "SetDllDirectory to %s successfully.\n", valuePath);
			WriteFile(debugFile, msg, strlen(msg), &written, NULL);
		}
	} else {
		if ( debug )
		{
			DWORD written;
			char msg[4096];
			sprintf_s(msg, "SetDllDirectory to %s failed.\n", valuePath);
			WriteFile(debugFile, msg, strlen(msg), &written, NULL);
		}

		CloseHandle(debugFile);
		return NULL;
	}
	*/

	char libraryFile[MAX_PATH];
	strcpy_s(libraryFile, MAX_PATH, valuePath);

	if ( debug )
	{
		strcat_s(libraryFile, MAX_PATH, "Voodoo_DX89_d.dll");
	} else {
		strcat_s(libraryFile, MAX_PATH, "Voodoo_DX89.dll");
	}

	//HMODULE library = LoadLibrary(libraryFile);
	HMODULE library = LoadLibraryEx(libraryFile, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

	if ( !library )
	{
		DWORD written;
		char error[4096];
		sprintf_s(error, 4096, "Could not load DLL: %s", libraryFile);
		MessageBoxA(NULL, error, "Voodoo Gem Hook Error 3", MB_ICONERROR);

		WriteFile(debugFile, error, strlen(error), &written, NULL);
		CloseHandle(debugFile);

		return NULL;
	}

	typedef void * (__stdcall * InitFuncType)(UINT);
	InitFuncType initFunc = (InitFuncType)GetProcAddress(library, "?Voodoo3DCreate8@@YGPAXI@Z");

	if ( !initFunc )
	{
		DWORD written;
		char msg[] = "Could not find init func.\n";

		MessageBoxA(NULL, msg, "Voodoo Gem Hook Error 4", MB_ICONERROR);

		WriteFile(debugFile, msg, strlen(msg), &written, NULL);
		CloseHandle(debugFile);

		return NULL;
	}

	void * obj = (*initFunc)(sdkVersion);

	if ( debug )
	{
		CloseHandle(debugFile);
	}

	return obj;
}

#pragma comment(linker, "/export:Direct3DCreate8=?Voodoo3DCreate8@@YGPAXI@Z")