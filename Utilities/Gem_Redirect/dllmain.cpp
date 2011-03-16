
#include <stdio.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <strsafe.h>

/**
 * Reads the Adapter's name from the Voodoo config file and loads it, if present.
 */
HMODULE LoadAdapter(const char * configFile)
{
    FILE * config = NULL;
    HKEY VoodooPathKey = NULL;
    char VoodooPath[MAX_PATH];
    char AdapterModule[MAX_PATH];

    errno_t error = fopen_s(&config, configFile, "r");

    if ( error != 0 || config == NULL )
    {
        MessageBoxA(NULL, "Voodoo Loader: Unable to open config file.", "Loader Error", MB_ICONERROR|MB_OK);
        exit(1);
    }

    if ( !fgets(AdapterModule, MAX_PATH, config) )
    {
        MessageBoxA(NULL, "Voodoo Loader: Unable to read adapter from config file.", "Loader Error", MB_ICONERROR|MB_OK);
        exit(1);
    }

    LONG keyOpen = RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\VoodooShader", 0, KEY_QUERY_VALUE, &VoodooPathKey);

    if ( keyOpen != ERROR_SUCCESS )
    {
        keyOpen = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\VoodooShader", 0, KEY_QUERY_VALUE, &VoodooPathKey);

        if ( keyOpen != ERROR_SUCCESS )
        {
            MessageBoxA(NULL, "Voodoo Loader: Unable to find Voodoo registry key.", "Loader Error", MB_ICONERROR|MB_OK);
            exit(1);
        }
    }

    // Key is open
    DWORD valueType = REG_NONE;
    DWORD valueSize = MAX_PATH;
    LONG valueQuery = RegQueryValueExA(VoodooPathKey, "Path", NULL, &valueType, (BYTE*)VoodooPath, &valueSize);

    if ( valueQuery != ERROR_SUCCESS || valueType == REG_NONE )
    {
        MessageBoxA(NULL, "Voodoo Loader: Unable to retrieve path from registry.", "Loader Error", MB_ICONERROR|MB_OK);
        exit(1);
    }

    StringCchCatA(VoodooPath, MAX_PATH, "\\bin\\");
    StringCchCatA(VoodooPath, MAX_PATH, AdapterModule);

    HMODULE adapter = LoadLibraryExA(VoodooPath, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

    return adapter;
}

// Support functions
void * WINAPI VoodooDXCreateGeneric(UINT sdkVersion, const char * ep)
{
    typedef void * (__stdcall * InitFunc)(UINT);

    HMODULE adapter = LoadAdapter("VoodooConfig.xml");

    if ( !adapter )
    {
        MessageBoxA(NULL, "Voodoo Loader: Unable to load adapter DLL.", "Loader Error", MB_ICONERROR|MB_OK);
        exit(1);
    }

    InitFunc initFunc = (InitFunc)GetProcAddress(adapter, ep);

    if ( !initFunc )
    {
        MessageBoxA(NULL, "Voodoo Loader: Unable to find adapter EP.", "Loader Error", MB_ICONERROR|MB_OK);
        exit(1);
    }

    return (*initFunc)(sdkVersion);
}

// Various adapter-loading entry points

// Direct3D 8
void * WINAPI Voodoo3DCreate8(UINT sdkVersion)
{
    return VoodooDXCreateGeneric(sdkVersion, "Direct3DCreate8");
}

// Direct3D 9
void * WINAPI Voodoo3DCreate9(UINT sdkVersion)
{
    return VoodooDXCreateGeneric(sdkVersion, "Direct3DCreate9");
}

// DInput (8a)
void * WINAPI VoodooInputCreateA(UINT sdkVersion)
{
    return VoodooDXCreateGeneric(sdkVersion, "DInputCreateA");
}