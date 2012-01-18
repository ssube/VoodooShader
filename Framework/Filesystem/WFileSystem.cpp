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

#include "FileSystem.hpp"

#include "Filesystem_Version.hpp"

#include "Format.hpp"

#include "Support.inl"

// The MS shlobj header contains a few functions that cause errors in analysis under /W4 (and cause the build to fail
// under /WX). This disables the warning for only that header.
#pragma warning(push)
#pragma warning(disable : 6387)
#include <shlobj.h>
#pragma warning(pop)
#include <strsafe.h>

namespace VoodooShader
{
    namespace VoodooWFS
    {
        static const Version version = VOODOO_VERSION_STRUCT(FILESYSTEM);
        static const wchar_t * name_VSWFileSystem = VSTR("VSWFileSystem");
        static const Uuid clsid_VSWFileSystem = CLSID_VSWFileSystem;

        const Version * VOODOO_CALLTYPE API_ModuleVersion()
        {
            return &version;
        }

        const uint32_t VOODOO_CALLTYPE API_ClassCount()
        {
            return 1;
        }

        const wchar_t * VOODOO_CALLTYPE API_ClassInfo(_In_ const uint32_t index, _Out_ Uuid * pUuid)
        {
            if (!pUuid)
            {
                return nullptr;
            }

            if (index == 0)
            {
                *pUuid = clsid_VSWFileSystem;
                return name_VSWFileSystem;
            }

            return nullptr;
        }

        IObject * VOODOO_CALLTYPE API_ClassCreate(const uint32_t index, _In_ ICore * pCore)
        {
            if (index == 0)
            {
                return new VSWFileSystem(pCore);
            }
            else
            {
                return nullptr;
            }
        }

        VSWFileSystem::VSWFileSystem(_In_ ICore * pCore) :
            m_Core(pCore)
        {
            using namespace pugi;
            // Create builtin vars
            IParser* parser = m_Core->GetParser();
            wchar_t cvar[MAX_PATH];

            if (SHGetFolderPath(nullptr, CSIDL_PERSONAL, nullptr, SHGFP_TYPE_CURRENT, cvar) == S_OK)
            {
                PathCombine(cvar, cvar, VSTR("My Games"));
                PathAddBackslash(cvar);
                parser->Add(VSTR("games"), cvar, VT_System);
            }

            if (SHGetFolderPath(nullptr, CSIDL_SYSTEM, nullptr, SHGFP_TYPE_CURRENT, cvar) == S_OK)
            {
                PathAddBackslash(cvar);
                parser->Add(VSTR("system"), cvar, VT_System);
            }

            // Init DevIL
            ilInit();

            // Load paths from the config
            xml_document *config = (xml_document *) m_Core->GetConfig();
            xpath_query pathsQuery(L"/VoodooConfig/Classes/Class[@Name = \"VSWFileSystem\"]/SearchPaths/Path");
            xpath_query pathValueQuery(L"./text()");
            xpath_node_set pathNodes = pathsQuery.evaluate_node_set(*config);
            xpath_node_set::const_iterator pathIter = pathNodes.begin();

            while (pathIter != pathNodes.end())
            {
                String path = pathValueQuery.evaluate_string(*pathIter).c_str();

                this->AddPath(path);

                ++pathIter;
            }
        }

        VSWFileSystem::~VSWFileSystem()
        {
            m_Directories.clear();
        }

        uint32_t VSWFileSystem::AddRef() const
        {
            return SAFE_INCREMENT(m_Refs);
        }

        uint32_t VSWFileSystem::Release() const
        {
            if (SAFE_DECREMENT(m_Refs) == 0)
            {
                delete this;
                return 0;
            }
            else
            {
                return m_Refs;
            }
        }

        VoodooResult VSWFileSystem::QueryInterface(_In_ Uuid clsid, _Deref_out_opt_ const IObject ** ppOut) const
        {
            if (!ppOut)
            {
                if (clsid.is_nil())
                {
                    clsid = CLSID_VSWFileSystem;
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                if (clsid == IID_IObject)
                {
                    *ppOut = static_cast<const IObject*>(this);
                }
                else if (clsid == IID_IFileSystem)
                {
                    *ppOut = static_cast<const IFileSystem*>(this);
                }
                else if (clsid == CLSID_VSWFileSystem)
                {
                    *ppOut = static_cast<const VSWFileSystem*>(this);
                }
                else
                {
                    *ppOut = nullptr;
                    return false;
                }

                reinterpret_cast<const IObject*>(*ppOut)->AddRef();
                return true;
            }
        }

        String VSWFileSystem::ToString() const
        {
            return VSTR("VSWFileSystem()");
        }

        ICore * VSWFileSystem::GetCore() const
        {
            return m_Core;
        }

        VoodooResult VSWFileSystem::AddPath(const String & name)
        {
            uint32_t count = name.Split(VSTR(";"), 0, nullptr);

            std::vector<String> paths(count);
            name.Split(VSTR(";"), count, &paths[0]);
            for (std::vector<String>::iterator i = paths.begin(); i != paths.end(); ++i)
            {
                this->m_Directories.push_front(*i);
            }

            return VSF_OK;
        }

        VoodooResult VSWFileSystem::RemovePath(const String & name)
        {
            this->m_Directories.remove_if
            (
                [&](const String & current)
                {
                    return (current == name);
                }
            );

            return VSF_OK;
        }

        IFile * VSWFileSystem::GetFile(const String & name, const GetFileMode mode) const
        {
            m_Core->GetLogger()->LogMessage
            (
                VSLog_ModDebug, VOODOO_FILESYSTEM_NAME, 
                Format(VSTR("Searching for raw file '%1%'.")) << name.GetData()
            );

            String filename = m_Core->GetParser()->Parse(name);

            m_Core->GetLogger()->LogMessage
            (
                VSLog_ModDebug, VOODOO_FILESYSTEM_NAME,
                Format(VSTR("Searching for parsed file '%1%'.")) << filename
            );

            StringList::const_iterator curDir = m_Directories.begin();

            while (curDir != m_Directories.end())
            {
                // Try to find the file in each registered dir
                String fullname = m_Core->GetParser()->Parse(*curDir, VSParse_SlashTrail) + filename;

                m_Core->GetLogger()->LogMessage
                (
                    VSLog_ModDebug, VOODOO_CORE_NAME,
                    Format(VSTR("Checking file '%1%'.")) << fullname
                );

                HANDLE file = CreateFile(fullname.GetData(), 0, 0, nullptr, OPEN_EXISTING, 0, nullptr);

                if (file != INVALID_HANDLE_VALUE)
                {
                    CloseHandle(file);

                    m_Core->GetLogger()->LogMessage
                    (
                        VSLog_ModDebug, VOODOO_CORE_NAME,
                        Format(VSTR("File '%1%' found in directory '%2%'.")) << name << (*curDir)
                    );

                    return new VSWFile(m_Core, fullname);
                }

                ++curDir;
            }

            m_Core->GetLogger()->LogMessage
            (
                VSLog_ModWarning, VOODOO_CORE_NAME, 
                Format(VSTR("Unable to find file '%1%'.")) << name.GetData()
            );

            if ((mode & FF_CreateOnly) == FF_CreateOnly)
            {
                String fullname = m_Core->GetParser()->Parse(*m_Directories.begin(), VSParse_SlashTrail) + filename;

                m_Core->GetLogger()->LogMessage
                (
                    VSLog_ModInfo, VOODOO_CORE_NAME,
                    Format(VSTR("Creating file '%1%'.")) << fullname
                );

                HANDLE file = CreateFile(fullname.GetData(), 0, 0, nullptr, OPEN_ALWAYS, 0, nullptr);

                if (file != INVALID_HANDLE_VALUE)
                {
                    CloseHandle(file);

                    m_Core->GetLogger()->LogMessage
                    (
                        VSLog_ModDebug, VOODOO_CORE_NAME,
                        Format(VSTR("File '%1%' created in directory '%2%'.")) << name << (*curDir)
                    );

                    return new VSWFile(m_Core, fullname);
                }
            }

            return nullptr;
        }
    }
}
