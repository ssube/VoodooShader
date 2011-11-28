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

#include "VSModule.hpp"
#include "VSParameter.hpp"
#include "VSParser.hpp"
#include "VSShader.hpp"
#include "VSTexture.hpp"

#include "IAdapter.hpp"
#include "ICore.hpp"
#include "IFilesystem.hpp"
#include "IHookManager.hpp"
#include "ILogger.hpp"
#include "ITexture.hpp"

#include "Converter.hpp"
#include "Exception.hpp"
#include "Version.hpp"

#include "shellapi.h"

#include <boost/lexical_cast.hpp>

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

    ICore * VOODOO_CALLTYPE CreateCore(_In_ const InitParams * const pInitParams, bool catchErrors)
    {
        static VSCore * pCore = nullptr;

        if (!pCore)
        {
            try
            {
                pCore = new VSCore();
                pCore->Initialize(pInitParams);
            } catch(const std::exception & exc) {
                UNREFERENCED_PARAMETER(exc);

                pCore = nullptr;

                if (!catchErrors)
                {
                    throw exc;
                }
            }
        }

        return pCore;
    }

    VSCore::VSCore() :
        m_Refs(0), m_ConfigFile(nullptr), m_CgContext(nullptr)
    { 

#ifdef VOODOO_DEBUG_MEMORY
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
        _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif

        // Set up the internal objects
        m_ModuleManager = new VSModuleManager(this);
        m_Parser = new VSParser(this);
    };

    VSCore::~VSCore()
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

#ifdef VOODOO_DEBUG_MEMORY
        _CrtDumpMemoryLeaks();
#endif
    }

    bool VOODOO_METHODCALLTYPE VSCore::Initialize(_In_ const InitParams * const pInitParams)
    {
        if (!pInitParams)
        {
            return false;
        }

        // Load variables, built-in first
        m_Parser->Add(L"globalroot", pInitParams->GlobalRoot, VT_System);
        m_Parser->Add(L"localroot", pInitParams->LocalRoot, VT_System);
        m_Parser->Add(L"runroot", pInitParams->RunRoot, VT_System);
        m_Parser->Add(L"target", pInitParams->Target, VT_System);
        m_Parser->Add(L"loader", pInitParams->Loader, VT_System);

        // Command line processing
        LPWSTR cmdline = GetCommandLine();
        m_Parser->Add(L"cmdline", cmdline, VT_System);

        int cmdargc = 0;
        LPWSTR * cmdargv = CommandLineToArgvW(cmdline, &cmdargc);
        m_Parser->Add("cmd_count", String::Format("%d", cmdargc), VT_System);
        for (int i = 0; i < cmdargc; ++i)
        {
            m_Parser->Add(String::Format("cmd_%d"), cmdargv[i], VT_System);
        }

        // Load the config
        String Config = m_Parser->Parse(pInitParams->Config);
        m_Parser->Add(L"config", Config, VT_System);

        try
        {
            m_ConfigFile = new pugi::xml_document();

            // Try loading the config file from each major location
            String configPath = m_Parser->Parse(L"$(runroot)\\$(config)", PF_PathCanon);
            pugi::xml_parse_result result = m_ConfigFile->load_file(configPath.GetData());

            if (!result)
            {
                configPath = m_Parser->Parse(L"$(localroot)\\$(config)");
                result = m_ConfigFile->load_file(configPath.GetData());
                if (!result)
                {
                    configPath = m_Parser->Parse(L"$(globalroot)\\$(config)");
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
                Throw(VOODOO_CORE_NAME, L"Could not find core config node.", nullptr);
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
                    String filter = xpq_getFilter.evaluate_string(*iter);
                    String path = xpq_getText.evaluate_string(*iter);

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
                    String file = xpq_getText.evaluate_string(*iter);

                    m_ModuleManager->LoadFile(file);

                    ++iter;
                }
            }

            // Lookup classes
            pugi::xpath_query logQuery(L"/VoodooConfig/Core/Class/Logger/text()");
            pugi::xpath_query fsQuery(L"/VoodooConfig/Core/Class/FileSystem/text()");
            pugi::xpath_query hookQuery(L"/VoodooConfig/Core/Class/HookManager/text()");
            pugi::xpath_query adpQuery(L"/VoodooConfig/Core/Class/Adapter/text()");
            String logClass = m_Parser->Parse(logQuery.evaluate_string(configRoot).c_str());
            String fsClass = m_Parser->Parse(fsQuery.evaluate_string(configRoot).c_str());
            String hookClass = m_Parser->Parse(hookQuery.evaluate_string(configRoot).c_str());
            String adpClass = m_Parser->Parse(adpQuery.evaluate_string(configRoot).c_str());

            // Make sure a logger was loaded
            m_Logger = dynamic_cast<ILogger*>(m_ModuleManager->CreateObject(logClass));
            if (m_Logger.get() == nullptr)
            {
                Throw(VOODOO_CORE_NAME, L"Unable to create logger.", nullptr);
            }

            pugi::xpath_query logfQuery(L"/VoodooConfig/Logger/File/text()");
            pugi::xpath_query loglQuery(L"/VoodooConfig/Logger/Level/text()");

            String logFile  = m_Parser->Parse(logfQuery.evaluate_string(configRoot).c_str());
            String logLevelStr = m_Parser->Parse(loglQuery.evaluate_string(configRoot).c_str());

            LogLevel logLevel = LL_Initial;
            try
            {
                logLevel = (LogLevel)boost::lexical_cast<int32_t>(logLevelStr.GetData());
            } catch (const boost::bad_lexical_cast & exc) {
                UNREFERENCED_PARAMETER(exc);
            }

            m_Logger->Open(logFile, false);
            m_Logger->SetLogLevel(logLevel);

            // Log extended build information
            m_Logger->Log(LL_CoreInfo, VOODOO_CORE_NAME, L"Config loaded from '%s'.", Config.GetData());
            m_Logger->Log(LL_CoreInfo, VOODOO_CORE_NAME, VOODOO_GLOBAL_COPYRIGHT_FULL);

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
                Throw(VOODOO_CORE_NAME, L"Unable to create file system.", nullptr);
            }

            m_HookManager = dynamic_cast<IHookManager*>(m_ModuleManager->CreateObject(hookClass));
            if (!m_HookManager)
            {
                Throw(VOODOO_CORE_NAME, L"Unable to create hook manager.", nullptr);
            }

            m_Adapter = dynamic_cast<IAdapter*>(m_ModuleManager->CreateObject(adpClass));
            if (!m_Adapter)
            {
                Throw(VOODOO_CORE_NAME, L"Unable to create adapter.", nullptr);
            }

            // ICore done loading
            m_Logger->Log(LL_CoreInfo, VOODOO_CORE_NAME, L"Core initialization complete.");

            // Return
        }
        catch(const std::exception & exc)
        {
            if (m_Logger.get())
            {
                m_Logger->Log(LL_CoreError, VOODOO_CORE_NAME, L"Error during Core creation: %S", exc.what());
            }

            return false;
        }

        return true;
    }

    bool VOODOO_METHODCALLTYPE VSCore::Reset()
    {
        //! @todo Actually reset stuff.
        return true;
    }

    uint32_t VOODOO_METHODCALLTYPE VSCore::AddRef() CONST
    {
        return SAFE_INCREMENT(m_Refs);
    }

    uint32_t VOODOO_METHODCALLTYPE VSCore::Release() CONST
    {
        uint32_t count = SAFE_DECREMENT(m_Refs);
        if (count == 0)
        {
            delete this;
        }
        return count;
    }

    bool VOODOO_METHODCALLTYPE VSCore::QueryInterface(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST
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

    String VOODOO_METHODCALLTYPE VSCore::ToString() CONST
    {
        return L"VSCore()";
    }

    ICore * VOODOO_METHODCALLTYPE VSCore::GetCore() CONST
    {
        return nullptr;
    }

    IParser * VOODOO_METHODCALLTYPE VSCore::GetParser() CONST
    {
        return m_Parser.get();
    }

    IHookManager * VOODOO_METHODCALLTYPE VSCore::GetHookManager() CONST
    {
        return m_HookManager.get();
    }

    IFileSystem * VOODOO_METHODCALLTYPE VSCore::GetFileSystem() CONST
    {
        return m_FileSystem.get();
    }

    IAdapter * VOODOO_METHODCALLTYPE VSCore::GetAdapter() CONST
    {
        return m_Adapter.get();
    }

    ILogger * VOODOO_METHODCALLTYPE VSCore::GetLogger() CONST
    {
        return m_Logger.get();
    }

    XmlDocument VOODOO_METHODCALLTYPE VSCore::GetConfig() CONST
    {
        return m_ConfigFile;
    }

    CGcontext VOODOO_METHODCALLTYPE VSCore::GetCgContext() CONST
    {
        return m_CgContext;
    }

    bool VOODOO_METHODCALLTYPE VSCore::SetCgContext(CGcontext pContext)
    {
        if (pContext == nullptr)
        {
            m_Logger->Log(LL_CoreDebug, VOODOO_CORE_NAME, L"Setting Cg context to nullptr.");

            m_Parameters.clear();
            m_Textures.clear();
            m_LastPass = nullptr;
            m_LastShader = nullptr;

            m_CgContext = pContext;

            return true;
        }
        else if (m_CgContext != nullptr)
        {
            m_Logger->Log(LL_CoreError, VOODOO_CORE_NAME,
                L"Error: Attempting to set Cg context (%p) over existing context (%p).", pContext, m_CgContext);

            return false;
        }
        else
        {
            m_CgContext = pContext;
            cgSetErrorHandler(Voodoo_CgErrorHandler_Func, this);
            m_Logger->Log(LL_CoreDebug, VOODOO_CORE_NAME, L"Set Cg context (%p).", pContext);
            return true;
        }
    }

    IShader * VOODOO_METHODCALLTYPE VSCore::CreateShader(_In_ const IFile * pFile, _In_opt_ const char **ppArgs)
    {
        if (!pFile || !m_CgContext)
        {
            return nullptr;
        }

        String fullpath = pFile->GetPath();

        IShader * shader = nullptr;

        try
        {
            shader = new VSShader(this, fullpath, ppArgs);
            m_Logger->Log(LL_CoreDebug, VOODOO_CORE_NAME, L"Successfully created shader from '%s'.", fullpath.GetData());
        }
        catch (const std::exception & exc)
        {   
            m_Logger->Log
            (
                LL_CoreError, VOODOO_CORE_NAME, 
                L"Error creating shader from '%s': %S",
                fullpath.GetData(), exc.what()
            );
        }

        return shader;
    }

    IParameter * VOODOO_METHODCALLTYPE VSCore::CreateParameter(const String & name, const ParameterType type)
    {
        if (!m_CgContext)
        {
            return nullptr;
        }

        ParameterMap::iterator paramEntry = m_Parameters.find(name);

        if (paramEntry != m_Parameters.end())
        {
            m_Logger->Log(LL_CoreWarn, VOODOO_CORE_NAME, L"Trying to create a parameter with a duplicate name.");
            return nullptr;
        }
        else
        {
            IParameterRef parameter = nullptr;

            try
            {
                parameter = new VSParameter(this, name, type);

                m_Parameters[name] = parameter;

                m_Logger->Log
                (
                    LL_CoreDebug, VOODOO_CORE_NAME,
                    L"Created parameter named %s with type %s, returning shared pointer to %p.", name.GetData(),
                    Converter::ToString(type), parameter.get()
                );
            } catch (const std::exception & exc) {
                m_Logger->Log
                (
                    LL_CoreDebug, VOODOO_CORE_NAME,
                    L"Error creating parameter %s with type %s: %S", name.GetData(),
                    Converter::ToString(type), exc.what()
                );
            }

            return parameter.get();
        }
    }

    ITexture * VOODOO_METHODCALLTYPE VSCore::CreateTexture(_In_ const String & name, _In_ const TextureDesc * pDesc)
    {
        if (!m_CgContext)
        {
            return nullptr;
        }

        TextureMap::iterator textureEntry = m_Textures.find(name);

        if (textureEntry != m_Textures.end())
        {
            m_Logger->Log(LL_CoreWarn, VOODOO_CORE_NAME, L"Trying to create a texture with a duplicate name.");
            return nullptr;
        }
        else
        {
            ITexture * texture = new VSTexture(this, name, pDesc, nullptr);

            m_Adapter->CreateTexture(name, pDesc, texture);

            m_Textures[name] = texture;

            m_Logger->Log(LL_CoreDebug, VOODOO_CORE_NAME, L"Added texture %s, returning shared pointer to %p.",
                name.GetData(), texture);

            return texture;
        }
    }

    IParameter * VOODOO_METHODCALLTYPE VSCore::GetParameter(const String & name, const ParameterType type) CONST
    {
        ParameterMap::const_iterator parameter = m_Parameters.find(name);

        if (parameter != m_Parameters.end())
        {
            m_Logger->Log(LL_CoreDebug, VOODOO_CORE_NAME, L"Got parameter %s, returning shared pointer to %p.",
                name.GetData(), parameter->second.get());

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
            m_Logger->Log(LL_CoreDebug, VOODOO_CORE_NAME, L"Unable to find parameter %s.", name.GetData());
            return nullptr;
        }
    }

    ITexture * VOODOO_METHODCALLTYPE VSCore::GetTexture(const String & name) CONST
    {
        TextureMap::const_iterator textureEntry = m_Textures.find(name);

        if (textureEntry != m_Textures.end())
        {
            m_Logger->Log(LL_CoreDebug, VOODOO_CORE_NAME, L"Got texture %s, returning shared pointer to %p.",
                name.GetData(), textureEntry->second.get());

            return textureEntry->second.get();
        }
        else
        {
            m_Logger->Log(LL_CoreDebug, VOODOO_CORE_NAME, L"Unable to find texture %s.", name.GetData());

            return nullptr;
        }
    }

    bool VOODOO_METHODCALLTYPE VSCore::RemoveParameter(_In_ const String & name)
    {
        ParameterMap::iterator parameter = m_Parameters.find(name);

        if (parameter != m_Parameters.end())
        {
            m_Logger->Log(LL_CoreDebug, VOODOO_CORE_NAME, L"Got parameter %s, erasing.", name.GetData());

            m_Parameters.erase(parameter);
            return true;
        }
        else
        {
            m_Logger->Log(LL_CoreDebug, VOODOO_CORE_NAME, L"Unable to find parameter %s.", name.GetData());

            return false;
        }
    }

    bool VOODOO_METHODCALLTYPE VSCore::RemoveTexture(_In_ const String & Name)
    {
        TextureMap::iterator texture = m_Textures.find(Name);

        if (texture != m_Textures.end())
        {
            m_Logger->Log(LL_CoreDebug, VOODOO_CORE_NAME, L"Got texture %s, returning shared pointer to %p.", Name.GetData(),
                texture->second.get());

            m_Textures.erase(texture);
            return true;
        }
        else
        {
            m_Logger->Log(LL_CoreDebug, VOODOO_CORE_NAME, L"Unable to find texture %s.", Name.GetData());
            return false;
        }
    }

    ITexture * VOODOO_METHODCALLTYPE VSCore::GetStageTexture(_In_ const TextureStage stage) CONST
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

    void VOODOO_METHODCALLTYPE VSCore::SetStageTexture(_In_ const TextureStage Stage, _In_opt_ ITexture * const pTexture)
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

    void VSCore::CgErrorHandler(CGcontext context, int error) CONST
    {        
        if (!m_Adapter || !m_Adapter->HandleError(context, error))
        {
            const char * errorString = error ? cgGetErrorString((CGerror) error) : nullptr;

            if (errorString)
            {
                this->GetLogger()->Log(LL_CoreError, VOODOO_CG_NAME, L"Cg core reported error %d: %S", error, errorString);
                if (context && error != CG_INVALID_CONTEXT_HANDLE_ERROR)
                {
                    // Print any compiler errors or other details we can find
                    const char *listing = cgGetLastListing(context);

                    if (listing)
                    {
                        this->GetLogger()->Log(LL_CoreError, VOODOO_CG_NAME, L"Cg error details: %S", listing);
                    }
                }
                else
                {
                    this->GetLogger()->Log(LL_CoreError, VOODOO_CG_NAME,
                        L"Invalid context for error, no further data available.");
                }
            }
            else
            {
                this->GetLogger()->Log(LL_CoreError, VOODOO_CG_NAME, L"Cg core reported an unknown error (%d).", error);
            }
        }
    }
}
