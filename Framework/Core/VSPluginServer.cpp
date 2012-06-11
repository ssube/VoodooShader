/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2012 by Sean Sube
 *
 * The Voodoo Shader Framework is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option)
 * any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this program; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 US
 *
 * Support and more information may be found at
 *   http://www.voodooshader.com
 * or by contacting the lead developer at
 *   peachykeen@voodooshader.com
 */

#include "VSPluginServer.hpp"
// Voodoo Core
#include "VSPlugin.hpp"
// System
#pragma warning(push,3)
#include <shlwapi.h>
#pragma warning(pop)

namespace VoodooShader
{
    #define VOODOO_DEBUG_TYPE VSPluginServer
    DeclareDebugCache();

    IPluginServer * VOODOO_CALLTYPE CreateServer()
    {
        static VSPluginServer * pServer = nullptr;

        if (!pServer)
        {
            try
            {
                pServer = new VSPluginServer();
            }
            catch (const std::exception & exc)
            {
                UNREFERENCED_PARAMETER(exc);
                pServer = nullptr;
            }
        }

        return pServer;
    }

    VSPluginServer::VSPluginServer() :
        m_Refs(0)
    {
        m_Parser = CreateParser();
        m_Logger = CreateLogger();

        AddThisToDebugCache();
    }

    VSPluginServer::~VSPluginServer()
    {
        RemoveThisFromDebugCache();

        m_Parser = nullptr;
        m_Logger = nullptr;

        m_Classes.clear();
        m_Plugins.clear();
    }

    uint32_t VOODOO_METHODTYPE VSPluginServer::AddRef() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        return SAFE_INCREMENT(m_Refs);
    }

    uint32_t VOODOO_METHODTYPE VSPluginServer::Release() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        uint32_t count = SAFE_DECREMENT(m_Refs);
        if (count == 0)
        {
            delete this;
        }
        return count;
    }

    VoodooResult VOODOO_METHODTYPE VSPluginServer::QueryInterface(Uuid refid, IObject ** ppOut)
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        if (!ppOut)
        {
            return VSFERR_INVALIDPARAMS;
        }
        else
        {
            if (refid == IID_IObject)
            {
                *ppOut = static_cast<IObject*>(this);
            }
            else if (refid == IID_IPluginServer)
            {
                *ppOut = static_cast<IPluginServer*>(this);
            }
            else if (refid == CLSID_VSPluginServer)
            {
                *ppOut = static_cast<VSPluginServer*>(this);
            }
            else
            {
                *ppOut = nullptr;
                return VSFERR_INVALIDUUID;
            }

            (*ppOut)->AddRef();
            return VSF_OK;
        }
    }

    String VOODOO_METHODTYPE VSPluginServer::ToString() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        return VSTR("VSPluginServer()");
    }

    ICore * VOODOO_METHODTYPE VSPluginServer::GetCore() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        return nullptr;
    }

    IPlugin * VOODOO_METHODTYPE VSPluginServer::GetPlugin(CONST String & name) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        StrongNameMap::const_iterator module = m_PluginNames.find(name);
        if (module != m_PluginNames.end())
        {
            Uuid libid = module->second;
            return this->GetPlugin(libid);
        }
        else
        {
            return nullptr;
        }
    }

    IPlugin * VOODOO_METHODTYPE VSPluginServer::GetPlugin(CONST Uuid libid) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        StrongPluginMap::const_iterator iter = m_Plugins.find(libid);
        if (iter != m_Plugins.end())
        {
            PluginRef plugin = iter->second;
            return plugin.get();
        }
        else
        {
            return nullptr;
        }
    }

    bool VOODOO_METHODTYPE VSPluginServer::IsLoaded(const String & name) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        return (m_PluginNames.find(name) != m_PluginNames.end());
    }

    bool VOODOO_METHODTYPE VSPluginServer::IsLoaded(const Uuid & libid) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        return (m_Plugins.find(libid) != m_Plugins.end());
    }

    VoodooResult VOODOO_METHODTYPE VSPluginServer::LoadPath(ICore * pCore, const String & path, const String & filter)
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        String mask = m_Parser->Parse(path) + VSTR("\\*");

        WIN32_FIND_DATA findFile;
        HANDLE searchHandle = FindFirstFile(mask.GetData(), &findFile);

        if (searchHandle == INVALID_HANDLE_VALUE)
        {
            DWORD error = GetLastError();

            if (error == ERROR_FILE_NOT_FOUND)
            {
                m_Logger->LogMessage
                (
                    VSLog_CoreWarning, VOODOO_CORE_NAME,
                    StringFormat("No plugins found in directory '%1%'.") << path
                );

                return VSFERR_FILENOTFOUND;
            }
            else
            {
                m_Logger->LogMessage 
                ( 
                    VSLog_CoreWarning, VOODOO_CORE_NAME,  
                    StringFormat("Error searching directory '%1%'.") << path 
                ); 
                 
                return VSF_FAIL; 
            }
        }

        Regex compfilter;
        if (filter.IsEmpty())
        {
            compfilter.SetExpr(VSTR(".*\\.dll"));
        }
        else
        {
            compfilter.SetExpr(filter);
        }

        do
        {
            String module = findFile.cFileName;

            this->LoadPlugin(pCore, module);
        } while (FindNextFile(searchHandle, &findFile) != 0);

        return VSF_OK;
    }

    VoodooResult VOODOO_METHODTYPE VSPluginServer::LoadPlugin(ICore * pCore, const String & filename)
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        String fullname = m_Parser->Parse(filename, VSParse_PathCanon);

        // Check for relative
        if (PathIsRelative(fullname.GetData()))
        {
            fullname = m_Parser->Parse(String(VSTR("$(path)\\")) + fullname, VSParse_PathCanon);
        }

        // Check for already loaded
        if (m_PluginNames.find(fullname) != m_PluginNames.end())
        {
            return VSF_OK;
        }

        // Create struct and load functions
        PluginRef module = VSPlugin::Load(this, fullname);

        if (module == nullptr)
        {
            if (m_Logger)
            {
                m_Logger->LogMessage
                (
                    VSLog_CoreError, VOODOO_CORE_NAME, 
                    StringFormat("Unable to load module '%1%'.") << filename
                );
            }

            return VSFERR_INVALIDPARAMS;
        }

        // Register classes from module
        const Version * moduleversion = module->PluginInit(pCore);

        if (!moduleversion)
        {
            if (m_Logger)
            {
                m_Logger->LogMessage(VSLog_CoreError, VOODOO_CORE_NAME, 
                    StringFormat("Null version returned by module '%1%'.") << fullname);
            }
            return VSFERR_INVALIDPARAMS;
        }

        m_Plugins[moduleversion->LibId] = module;
        m_PluginNames[moduleversion->Name] = moduleversion->LibId;

        if (moduleversion->Debug != VOODOO_DEBUG_BOOL && m_Logger)
        {
            m_Logger->LogMessage
            (
                VSLog_CoreWarning, VOODOO_CORE_NAME,
                StringFormat("Debug build mismatch with module '%1%'.") << moduleversion->Name
            );
        }

        if (m_Logger)
        {
            m_Logger->LogMessage(VSLog_CoreNotice, VOODOO_CORE_NAME, 
                StringFormat("Loaded module: %1%") << moduleversion->Name);
        }

        uint32_t classCount = module->ClassCount();

        for (uint32_t curClass = 0; curClass < classCount; ++curClass)
        {
            Uuid clsid;
            ZeroMemory(&clsid, sizeof(Uuid));

            const wchar_t * classname = module->ClassInfo(curClass, &clsid);

            if (classname)
            {
                m_Classes.insert(std::pair<Uuid, ClassSource>(clsid, ClassSource(module, curClass)));
                m_ClassNames.insert(std::pair<String, Uuid>(classname, clsid));
            }
        }

        return VSF_OK;
    }


    VoodooResult VOODOO_METHODTYPE VSPluginServer::LoadPlugin(ICore * pCore, const IFile * pFile)
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        if (pFile)
        {
            return this->LoadPlugin(pCore, pFile->GetPath());
        }
        else
        {
            return VSFERR_INVALIDPARAMS;
        }
    }

    VoodooResult VOODOO_METHODTYPE VSPluginServer::UnloadPlugin(ICore * pCore, CONST String & name)
    {
        StrongNameMap::iterator module = m_PluginNames.find(name);
        if (module != m_PluginNames.end())
        {
            VoodooResult result = this->UnloadPlugin(pCore, module->second);
            if (SUCCEEDED(result))
            {
                m_PluginNames.erase(module);
            }
            return result;
        }
        else
        {
            return VSFERR_INVALIDPARAMS;
        }
    }

    VoodooResult VOODOO_METHODTYPE VSPluginServer::UnloadPlugin(ICore * pCore, CONST Uuid libid)
    {
        StrongPluginMap::iterator module = m_Plugins.find(libid);
        if (module != m_Plugins.end())
        {
            PluginRef plugin = module->second;
            plugin->PluginReset(pCore);

            for (StrongNameMap::iterator iter = m_PluginNames.begin(); iter != m_PluginNames.end(); ++iter)
            {
                if (iter->second == libid)
                {
                    m_PluginNames.erase(iter++);
                }
                else
                {
                    ++iter;
                }
            }

            m_Plugins.erase(module);
            plugin.reset();

            return VSF_OK;
        }
        else
        {
            return VSFERR_INVALIDPARAMS;
        }
    }

    bool VOODOO_METHODTYPE VSPluginServer::ClassExists(const Uuid clsid) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        return (m_Classes.find(clsid) != m_Classes.end());
    }

    bool VOODOO_METHODTYPE VSPluginServer::ClassExists(const String & name) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        Uuid clsid;
        if (!name.ToUuid(&clsid))
        {
            StrongNameMap::const_iterator classiter = m_ClassNames.find(name);
            if (classiter != m_ClassNames.end())
            {
                clsid = classiter->second;
            }
            else
            {
                return false;
            }
        }

        return this->ClassExists(clsid);
    }

    IObject * VOODOO_METHODTYPE VSPluginServer::CreateObject(ICore * pCore, const Uuid clsid) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        ClassMap::const_iterator classiter = m_Classes.find(clsid);

        if (classiter != m_Classes.end())
        {
            PluginRef module = classiter->second.first;
            uint32_t index = classiter->second.second;

            IObject * object = module->CreateClass(index, pCore);

            if (object == nullptr)
            {
                if (m_Logger)
                {
                    m_Logger->LogMessage
                    (
                        VSLog_CoreError, VOODOO_CORE_NAME,
                        StringFormat("Error creating instance of class %1%.") << clsid
                    );
                }
            }

            return object;
        }
        else
        {
            if (m_Logger)
            {
                m_Logger->LogMessage
                (
                    VSLog_CoreError, VOODOO_CORE_NAME, 
                    StringFormat("Class %1% not found.") << clsid
                );
            }

            return nullptr;
        }
    }

    IObject * VOODOO_METHODTYPE VSPluginServer::CreateObject(_In_ ICore * pCore, _In_ CONST String & name) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        Uuid clsid;
        i
			f (!name.ToUuid(&clsid))
        {
            StrongNameMap::const_iterator classiter = m_ClassNames.find(name);
            if (classiter != m_ClassNames.end())
            {
                clsid = classiter->second;
            }
            else
            {
                return nullptr;
            }
        }

        return this->CreateObject(pCore, clsid);
    }
}
