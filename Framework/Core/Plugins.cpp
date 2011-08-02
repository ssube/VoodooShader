#include "Plugins.hpp"

namespace VoodooShader
{
    bool PluginManager::LoadPath(_In_ String path)
    {
        String mask = path + "\\plugin_*";

        LPWIN32_FIND_DATA findFile;
        HANDLE searchHandle = FindFirstFile(mask.c_str(), &findFile);

        if ( searchHandle == INVALID_HANDLE_VALUE )
        {
            DWORD error = GetLastError();
            if ( error == ERROR_FILE_NOT_FOUND )
            {
                m_Core->GetLogger()->Log(LL_)
            }
        }
    }
}
