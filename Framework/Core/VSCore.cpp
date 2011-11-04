/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2011 by Sean Sube
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

#include "VSCore.hpp"

#include <boost/lexical_cast.hpp>

#include "Converter.hpp"
#include "Exception.hpp"
#include "Version.hpp"

#include "IAdapter.hpp"
#include "ICore.hpp"
#include "IFilesystem.hpp"
#include "IHookManager.hpp"
#include "ILogger.hpp"
#include "ITexture.hpp"

#include "VSModule.hpp"
#include "VSParameter.hpp"
#include "VSParser.hpp"
#include "VSShader.hpp"

namespace VoodooShader
{
    void Voodoo_CgErrorHandler_Func(_In_ CGcontext pContext, _In_ CGerror error, _In_opt_ void * pCore)
    {
        if (pCore)
        {
            VSCore * pSrcCore = static_cast<VSCore *>(pCore);
            pSrcCore->CgErrorHandler(pContext, error);
        }
    }

    ICore * VOODOO_CALL CreateCore(_In_ const InitParams * const pInitParams, bool catchErrors)
    {
        ICore * pCore = nullptr;

        if (!catchErrors)
        {
            pCore = new VSCore(pInitParams);
        } else {
            try
            {
                pCore = new VSCore(pInitParams);
            } catch(const std::exception & exc) {
                UNREFERENCED_PARAMETER(exc);

                if (pCore)
                {
                    delete pCore;
                    pCore = nullptr;
                }
            }
        }

        return pCore;
    }

    VSCore::VSCore(_In_ const InitParams * const pInitParams) :
        m_Refs(0), m_CgContext(nullptr), m_ConfigFile(nullptr)
    {
        if (!pInitParams)
        {
            Throw(VOODOO_CORE_NAME, L"No init parameters provided.", nullptr);
        }

#ifdef VSF_DEBUG_MEMORY
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
        _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif

        // Set up the internal objects
        m_ModuleManager = new VSModuleManager(this);
        m_Parser = new VSParser(this);

        // Load variables, built-in first
        m_Parser->Add(L"globalroot", pInitParams->GlobalRoot, VT_System);
        m_Parser->Add(L"localroot", pInitParams->LocalRoot, VT_System);
        m_Parser->Add(L"runroot", pInitParams->RunRoot, VT_System);
        m_Parser->Add(L"target", pInitParams->Target, VT_System);
        m_Parser->Add(L"loader", pInitParams->Loader, VT_System);

        String Config = m_Parser->Parse(pInitParams->Config);

        m_Parser->Add(L"config", Config, VT_System);

        try
        {
            m_ConfigFile = new pugi::xml_document();

            // Try loading the config file from each major location
            String configPath = m_Parser->Parse(L"$(runroot)$(config)");
            pugi::xml_parse_result result = m_ConfigFile->load_file(configPath.GetData());

            if (!result)
            {
                configPath = m_Parser->Parse(L"$(localroot)$(config)");
                result = m_ConfigFile->load_file(configPath.GetData());
                if (!result)
                {
                    configPath = m_Parser->Parse(L"$(globalroot)$(config)");
                    result = m_ConfigFile->load_file(configPath.GetData());
                    if (!result)
                    {
                        configPath = m_Parser->Parse(L"$(config)");
                        result = m_ConfigFile->load_file(configPath.GetData());
                        if (!result)
                        {
                            Throw(VOODOO_CORE_NAME, L"Unable to find or parse config file.", nullptr);
                        }
                    }
                }
            }

            // Start setting things up
            pugi::xml_node configRoot = static_cast<pugi::xml_node>(*m_ConfigFile);
            pugi::xml_node coreNode = configRoot.select_single_node(L"/VoodooConfig/Core").node();
            if (!coreNode)
            {
                Throw(VOODOO_CORE_NAME, L"Could not find ICore node.", nullptr);
            }

            // Create query for node text, used multiple times
            pugi::xpath_query xpq_getName(L"./@name");
            pugi::xpath_query xpq_getText(L"./text()");

            // Load variables
            {
                pugi::xpath_query xpq_getVariables(L"/VoodooConfig/Variables/Variable");
                pugi::xpath_node_set nodes = xpq_getVariables.evaluate_node_set(configRoot);
                pugi::xpath_node_set::const_iterator iter = nodes.begin();

                while (iter != nodes.end())
                {
                    String name = xpq_getName.evaluate_string(*iter).c_str();
                    String value = xpq_getText.evaluate_string(*iter).c_str();

                    m_Parser->Add(name, value);

                    ++iter;
                }
            }

            // Load plugins
            {
                pugi::xpath_query xpq_getPluginPaths(L"/VoodooConfig/Plugins/Path");
                pugi::xpath_query xpq_getFilter(L"./@filter");
                pugi::xpath_node_set nodes = xpq_getPluginPaths.evaluate_node_set(configRoot);
                pugi::xpath_node_set::const_iterator iter = nodes.begin();

                while (iter != nodes.end())
                {
                    String filter = m_Parser->Parse(xpq_getFilter.evaluate_string(*iter).c_str());
                    String path = m_Parser->Parse(xpq_getText.evaluate_string(*iter).c_str());

                    m_ModuleManager->LoadPath(path, filter);

                    ++iter;
                }
            }

            {
                pugi::xpath_query xpq_getPluginFiles(L"/VoodooConfig/Plugins/File");
                pugi::xpath_node_set nodes = xpq_getPluginFiles.evaluate_node_set(configRoot);
                pugi::xpath_node_set::const_iterator iter = nodes.begin();

                while (iter != nodes.end())
                {
                    String file = m_Parser->Parse(xpq_getText.evaluate_string(*iter).c_str());

                    m_ModuleManager->LoadFile(file);

                    ++iter;
                }
            }

            // Lookup classes
            pugi::xpath_query logQuery(L"/VoodooConfig/Core/Class/Logger/text()");
            pugi::xpath_query fsQuery(L"/VoodooConfig/Core/Class/FileSystem/text()");
            pugi::xpath_query hookQuery(L"/VoodooConfig/Core/Class/HookManager/text()");
            pugi::xpath_query adpQuery(L"/VoodooConfig/Core/Class/Adapter/text()");
            pugi::xpath_query logfQuery(L"/VoodooConfig/Core/Class/Logger/@file");
            pugi::xpath_query loglQuery(L"/VoodooConfig/Core/Class/Logger/@level");
            String logClass = m_Parser->Parse(logQuery.evaluate_string(configRoot).c_str());
            String fsClass = m_Parser->Parse(fsQuery.evaluate_string(configRoot).c_str());
            String hookClass = m_Parser->Parse(hookQuery.evaluate_string(configRoot).c_str());
            String adpClass = m_Parser->Parse(adpQuery.evaluate_string(configRoot).c_str());
            String logFile = m_Parser->Parse(logfQuery.evaluate_string(configRoot).c_str());
            
            LogLevel logLevel = LL_Initial;
            try
            {
                logLevel = (LogLevel)boost::lexical_cast<int32_t>(loglQuery.evaluate_string(configRoot));
            } catch (const boost::bad_lexical_cast & exc) {
                UNREFERENCED_PARAMETER(exc);
            }

            // Make sure a logger was loaded
            m_Logger = dynamic_cast<ILogger*>(m_ModuleManager->CreateObject(logClass));
            if (m_Logger.get() == nullptr)
            {
                Throw(VOODOO_CORE_NAME, L"Unable to create Logger object (class not found).", nullptr);
            }

            m_Logger->Open(logFile, false);
            m_Logger->SetLogLevel(logLevel);

            // Log extended build information
            m_Logger->Log(LL_Info, VOODOO_CORE_NAME, L"Config loaded from '%s'.", Config.GetData());
            m_Logger->Log(LL_Info, VOODOO_CORE_NAME, VOODOO_GLOBAL_COPYRIGHT_FULL);

            Version vfver = VOODOO_META_VERSION_STRUCT(CORE);
            Version vsver = VOODOO_META_VERSION_STRUCT(VC);
            Version cgver = VOODOO_META_VERSION_STRUCT(CG);

            m_Logger->LogModule(&vfver);
            m_Logger->LogModule(&vsver);
            m_Logger->LogModule(&cgver);

            // Load less vital classes
            m_FileSystem = dynamic_cast<IFileSystem*>(m_ModuleManager->CreateObject(fsClass));
            if (!m_FileSystem)
            {
                Throw(VOODOO_CORE_NAME, L"Unable to create FileSystem object.", nullptr);
            }

            m_HookManager = dynamic_cast<IHookManager*>(m_ModuleManager->CreateObject(hookClass));
            if (!m_HookManager)
            {
                Throw(VOODOO_CORE_NAME, L"Unable to create HookManager object.", nullptr);
            }

            m_Adapter = dynamic_cast<IAdapter*>(m_ModuleManager->CreateObject(adpClass));
            if (!m_Adapter)
            {
                Throw(VOODOO_CORE_NAME, L"Unable to create Adapter object.", nullptr);
            }

            // ICore done loading
            m_Logger->Log(LL_Info, VOODOO_CORE_NAME, L"ICore initialization complete.");

            // Return
        }
        catch(std::exception & exc)
        {
            if (m_Logger.get())
            {
                m_Logger->Log(LL_Error, VOODOO_CORE_NAME, L"Error during ICore creation: %s", exc.what());
            }

            throw exc;
        }
    }

    VSCore::~VSCore(void)
    {
        this->SetCgContext(nullptr);

        m_LastPass = nullptr;
        m_LastShader = nullptr;

        m_Parameters.clear();
        m_Textures.clear();

        m_Adapter = nullptr;
        m_HookManager = nullptr;
        m_FileSystem = nullptr;
        m_Logger = nullptr;

        m_ModuleManager = nullptr;

        if (m_ConfigFile) 
        {
            delete m_ConfigFile;
        }

#ifdef VSF_DEBUG_MEMORY
        _CrtDumpMemoryLeaks();
#endif
    }

    uint32_t VSCore::AddRef() const
    {
        return ++m_Refs;
    }

    uint32_t VSCore::Release() const
    {
        uint32_t count = --m_Refs;
        if (count == 0)
        {
            delete this;
        }
        return count;
    }

    bool VSCore::CheckedCast(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const
    {
        if (!ppOut)
        {
            if (clsid.is_nil())
            {
                clsid = IID_ICore;
                return true;
            } else {
                return false;
            }
        } else {
            if (clsid == IID_IObject)
            {
                *ppOut = static_cast<const IObject*>(this);
            } else if (clsid == IID_ICore) {
                *ppOut = static_cast<const ICore*>(this);
            } else if (clsid == CLSID_VSCore) {
                *ppOut = static_cast<const VSCore*>(this);
            } else {
                *ppOut = nullptr;
                return false;
            }

            reinterpret_cast<const IObject*>(*ppOut)->AddRef();
            return true;
        }
    }

    String VSCore::ToString() const
    {
        return L"VSCore";
    }

    ICore * VSCore::GetCore() const
    {
        return nullptr;
    }

    IParser* VSCore::GetParser(void) const
    {
        return m_Parser.get();
    }

    IHookManager* VSCore::GetHookManager(void) const
    {
        return m_HookManager.get();
    }

    IFileSystem* VSCore::GetFileSystem(void) const
    {
        return m_FileSystem.get();
    }

    IAdapter* VSCore::GetAdapter(void) const
    {
        return m_Adapter.get();
    }

    ILogger* VSCore::GetLogger(void) const
    {
        return m_Logger.get();
    }

    XmlDocument VSCore::GetConfig(void) const
    {
        return m_ConfigFile;
    }

    CGcontext VSCore::GetCgContext(void) const
    {
        return m_CgContext;
    }

    bool VSCore::SetCgContext(CGcontext pContext)
    {
        if (pContext == nullptr)
        {
            m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, L"Setting Cg context to nullptr.");

            m_Parameters.clear();
            m_Textures.clear();
            m_LastPass = nullptr;
            m_LastShader = nullptr;

            m_CgContext = pContext;

            return true;
        }
        else if (m_CgContext != nullptr)
        {
            m_Logger->Log(LL_Error, VOODOO_CORE_NAME,
                L"Error: Attempting to set Cg context (%p) over existing context (%p).", pContext, m_CgContext);

            return false;
        }
        else
        {
            m_CgContext = pContext;
            m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, L"Set Cg context (%p).", pContext);
            return true;
        }
    }

    IShader * VSCore::CreateShader(_In_ const IFile * pFile, _In_opt_ const char **ppArgs)
    {
        if (!pFile)
        {
            return nullptr;
        }

        String fullpath = pFile->GetPath();
        IShaderRef shader = new VSShader(this, fullpath, ppArgs);

        m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, L"Created shader from %s, returning shared pointer to %p.",
            fullpath.GetData(), shader.get());

        return shader.get();
    }

    IParameter * VSCore::CreateParameter(const String & name, const ParameterType type)
    {
        ParameterMap::iterator paramEntry = m_Parameters.find(name);

        if (paramEntry != m_Parameters.end())
        {
            m_Logger->Log(LL_Warning, VOODOO_CORE_NAME, L"Trying to create a parameter with a duplicate name.");
            return nullptr;
        }
        else
        {
            IParameterRef parameter = new VSParameter(this, name, type);

            m_Parameters[name] = parameter;

            m_Logger->Log
            (
                LL_Debug, VOODOO_CORE_NAME,
                L"Created parameter named %s with type %s, returning shared pointer to %p.", name.GetData(),
                Converter::ToString(type), parameter
            );

            return parameter.get();
        }
    }

    ITexture* VSCore::CreateTexture(_In_ const String & name, _In_ const TextureDesc * pDesc)
    {
        TextureMap::iterator textureEntry = m_Textures.find(name);

        if (textureEntry != m_Textures.end())
        {
            m_Logger->Log(LL_Warning, VOODOO_CORE_NAME, L"Trying to create a texture with a duplicate name.");
            return nullptr;
        }
        else
        {
            ITexture * texture = m_Adapter->CreateTexture(name, pDesc);

            m_Textures[name] = texture;

            m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, L"Added texture %s, returning shared pointer to %p.",
                name.GetData(), texture);

            return texture;
        }
    }

    IParameter* VSCore::GetParameter(const String & name, const ParameterType type) const
    {
        ParameterMap::const_iterator parameter = m_Parameters.find(name);

        if (parameter != m_Parameters.end())
        {
            m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, L"Got parameter %s, returning shared pointer to %p.",
                name.GetData(), parameter->second);

            if (type == PT_Unknown)
            {
                return parameter->second.get();
            }
            else if (parameter->second->GetType() == type)
            {
                return parameter->second.get();
            }
            else
            {
                return nullptr;
            }
        }
        else
        {
            m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, L"Unable to find parameter %s.", name.GetData());
            return nullptr;
        }
    }

    ITexture * VSCore::GetTexture(const String & name) const
    {
        TextureMap::const_iterator textureEntry = m_Textures.find(name);

        if (textureEntry != m_Textures.end())
        {
            m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, L"Got texture %s, returning shared pointer to %p.",
                name.GetData(), textureEntry->second);

            return textureEntry->second.get();
        }
        else
        {
            m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, L"Unable to find texture %s.", name.GetData());

            return nullptr;
        }
    }

    bool VSCore::RemoveParameter(_In_ const String & name)
    {
        ParameterMap::iterator parameter = m_Parameters.find(name);

        if (parameter != m_Parameters.end())
        {
            m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, L"Got parameter %s, erasing.", name.GetData());

            m_Parameters.erase(parameter);
            return true;
        }
        else
        {
            m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, L"Unable to find parameter %s.", name.GetData());

            return false;
        }
    }

    bool VSCore::RemoveTexture(_In_ const String & Name)
    {
        TextureMap::iterator texture = m_Textures.find(Name);

        if (texture != m_Textures.end())
        {
            m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, L"Got texture %s, returning shared pointer to %p.", Name.GetData(),
                texture->second);

            m_Textures.erase(texture);
            return true;
        }
        else
        {
            m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, L"Unable to find texture %s.", Name.GetData());
            return false;
        }
    }

    ITexture * VSCore::GetStageTexture(_In_ const TextureStage stage) const
    {
        switch (stage)
        {
        case TS_Shader:
            return m_LastShader.get();
        case TS_Pass:
            return m_LastPass.get();
        default:
            return nullptr;
        }
    }

    void VSCore::SetStageTexture(_In_ const TextureStage Stage, _In_ ITexture * const pTexture)
    {
        switch (Stage)
        {
        case TS_Shader:
            m_LastShader = pTexture;
            break;
        case TS_Pass:
            m_LastPass = pTexture;
            break;
        }
    }

    void VSCore::CgErrorHandler(CGcontext context, int error) const
    {
        const char *errorString = error ? cgGetErrorString((CGerror) error) : nullptr;

        if (errorString)
        {
            this->GetLogger()->Log(LL_Error, VOODOO_CG_NAME, L"Cg core reported error: %s", errorString);
            if (context && error != CG_INVALID_CONTEXT_HANDLE_ERROR)
            {
                if (m_Adapter)
                {
                    m_Adapter->HandleError(context, error);
                }

                // Print any compiler errors or other details we can find
                const char *listing = cgGetLastListing(context);

                while (listing)
                {
                    this->GetLogger()->Log(LL_Error, VOODOO_CG_NAME, L"Cg error details: %s", listing);
                    listing = cgGetLastListing(context);
                }
            }
            else
            {
                this->GetLogger()->Log(LL_Error, VOODOO_CG_NAME,
                    L"Invalid context for error, no further data available.");
            }
        }
        else
        {
            this->GetLogger()->Log(LL_Error, VOODOO_CG_NAME, L"Cg core reported an unknown error (%d).", error);
        }
    }
}
