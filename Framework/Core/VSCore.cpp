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

#include "VSCore.hpp"

#include "VSModuleManager.hpp"
#include "VSParameter.hpp"
#include "VSParser.hpp"
#include "VSShader.hpp"

#include "Support.inl"

#include "shellapi.h"

namespace VoodooShader
{
    #define VOODOO_DEBUG_TYPE VSCore
    DeclareDebugCache();
    HMODULE gCoreHandle = nullptr;

    void Voodoo_CgErrorHandler_Func(_In_ CGcontext pContext, _In_ CGerror error, _In_opt_ void * pCore)
    {
        if (pCore)
        {
            VSCore * pSrcCore = static_cast<VSCore *>(pCore);
            pSrcCore->CgErrorHandler(pContext, error);
        }
    }

    ICore * VOODOO_CALLTYPE CreateCore(_In_ uint32_t version)
    {
        UNREFERENCED_PARAMETER(version);

        static VSCore * pCore = nullptr;

        if (!pCore)
        {
            try
            {
                pCore = new VSCore(version);
            }
            catch (const std::exception & exc)
            {
                UNREFERENCED_PARAMETER(exc);
                pCore = nullptr;
            }
        }

        return pCore;
    }

    VSCore::VSCore(uint32_t version) :
        m_Refs(0), m_Version(version), m_ConfigFile(nullptr), m_CgContext(nullptr)
    {
#if defined(VOODOO_DEBUG_MEMORY)
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRT0DBG_LEAK_CHECK_DF);
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
        _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif

        // Set up the internal objects
        m_ModuleManager = new VSModuleManager(this);
        m_Parser = new VSParser(this);

        AddThisToDebugCache();
    };

    VSCore::~VSCore()
    {
        RemoveThisFromDebugCache();

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

#if defined(VOODOO_DEBUG_MEMORY)
        _CrtDumpMemoryLeaks();
        //! @todo Dump debug object caches as well.
#endif
    }

    bool VOODOO_METHODTYPE VSCore::Initialize(_In_ const wchar_t * const config)
    {
        if (config)
        {
            m_Parser->Add(VSTR("config"), config, VT_System);
        }
        else
        {
            m_Parser->Add(VSTR("config"), VSTR("VoodooShader.xml"), VT_System);
        }

        // Load variables, built-in first
        {
            wchar_t buffer[MAX_PATH];

            // Global path
            GetVoodooPath(buffer);
            m_Parser->Add(VSTR("path"), buffer, VT_System);

            // Bin prefix
            GetVoodooBinPrefix(buffer);
            m_Parser->Add(VSTR("prefix"), buffer, VT_System);

            // Framework bin path
            GetVoodooBinPath(buffer);
            m_Parser->Add(VSTR("binpath"), buffer, VT_System);

            // Target
            HMODULE pTarget = GetModuleHandle(NULL);
            GetModuleFileName(pTarget, buffer, MAX_PATH);
            m_Parser->Add(VSTR("target"), buffer, VT_System);

            // Local path
            PathRemoveFileSpec(buffer);
            PathAddBackslash(buffer);
            m_Parser->Add(VSTR("local"), buffer, VT_System);

            // Startup path
            GetCurrentDirectory(MAX_PATH, buffer);
            PathAddBackslash(buffer);
            m_Parser->Add(VSTR("startup"), buffer, VT_System);

            GetModuleFileName(gCoreHandle, buffer, MAX_PATH);
            m_Parser->Add(VSTR("core"), buffer, VT_System);
        }

        // Command line processing
        LPWSTR cmdline = GetCommandLine();
        m_Parser->Add(VSTR("args"), cmdline, VT_System);

        int cmdargc = 0;
        LPWSTR * cmdargv = CommandLineToArgvW(cmdline, &cmdargc);
        m_Parser->Add(VSTR("argc"), Format(VSTR("%d")) << cmdargc, VT_System);
        for (int i = 0; i < cmdargc; ++i)
        {
            m_Parser->Add(Format(VSTR("argv_%d")) << i, cmdargv[i], VT_System);
        }

        // Load the config
        try
        {
            m_ConfigFile = new pugi::xml_document();

            // Try loading the config file from each major location
            String configPath = m_Parser->Parse(VSTR("$(config)"), PF_PathCanon);
            pugi::xml_parse_result result = m_ConfigFile->load_file(configPath.GetData());

            if (!result)
            {
                configPath = m_Parser->Parse(VSTR("$(startup)\\$(config)"), PF_PathCanon);
                result = m_ConfigFile->load_file(configPath.GetData());
                if (!result)
                {
                    configPath = m_Parser->Parse(VSTR("$(local)\\$(config)"), PF_PathCanon);
                    result = m_ConfigFile->load_file(configPath.GetData());
                    if (!result)
                    {
                        configPath = m_Parser->Parse(VSTR("$(path)\\$(config)"), PF_PathCanon);
                        result = m_ConfigFile->load_file(configPath.GetData());
                        if (!result)
                        {
                            Throw(VOODOO_CORE_NAME, VSTR("Unable to find or parse config file."), nullptr);
                        }
                    }
                }
            }

            // Start setting things up
            pugi::xml_node configRoot = static_cast<pugi::xml_node>(*m_ConfigFile);
            pugi::xml_node globalNode = configRoot.select_single_node(L"/VoodooConfig/Global").node();
            if (!globalNode)
            {
                Throw(VOODOO_CORE_NAME, VSTR("Could not find global config node."), nullptr);
            }

            // Create query for node text, used multiple times
            pugi::xpath_query xpq_getName(L"./@name");
            pugi::xpath_query xpq_getText(L"./text()");

            // Load variables
            {
                pugi::xpath_query xpq_getVariables(L"./Variables/Variable");
                pugi::xpath_node_set nodes = xpq_getVariables.evaluate_node_set(globalNode);
                pugi::xpath_node_set::const_iterator iter = nodes.begin();

                while (iter != nodes.end())
                {
                    String name = xpq_getName.evaluate_string(*iter).c_str();
                    String value = xpq_getText.evaluate_string(*iter).c_str();

                    m_Parser->Add(name, value);

                    ++iter;
                }
            }

            // Load plugins, starting with the core
            m_ModuleManager->LoadFile(VSTR("$(core)"));

            {
                pugi::xpath_query xpq_getPluginPaths(L"./Plugins/Path");
                pugi::xpath_query xpq_getFilter(L"./@filter");
                pugi::xpath_node_set nodes = xpq_getPluginPaths.evaluate_node_set(globalNode);
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
                pugi::xpath_query xpq_getPluginFiles(L"./Plugins/File");
                pugi::xpath_node_set nodes = xpq_getPluginFiles.evaluate_node_set(globalNode);
                pugi::xpath_node_set::const_iterator iter = nodes.begin();

                while (iter != nodes.end())
                {
                    String file = xpq_getText.evaluate_string(*iter);

                    m_ModuleManager->LoadFile(file);

                    ++iter;
                }
            }

            // Lookup classes
            pugi::xpath_query logQuery(L"./Classes/Logger/text()");
            pugi::xpath_query fsQuery(L"./Classes/FileSystem/text()");
            pugi::xpath_query hookQuery(L"./Classes/HookManager/text()");
            pugi::xpath_query adpQuery(L"./Classes/Adapter/text()");
            String logClass = m_Parser->Parse(logQuery.evaluate_string(globalNode).c_str());
            String fsClass = m_Parser->Parse(fsQuery.evaluate_string(globalNode).c_str());
            String hookClass = m_Parser->Parse(hookQuery.evaluate_string(globalNode).c_str());
            String adpClass = m_Parser->Parse(adpQuery.evaluate_string(globalNode).c_str());

            // Make sure a logger was loaded
            m_Logger = dynamic_cast<ILogger*>(m_ModuleManager->CreateObject(logClass));
            if (!m_Logger)
            {
                String error = Format(VSTR("Unable to create logger (class %1%).")) << logClass;
                Throw(VOODOO_CORE_NAME, error.GetData(), this);
            }

            pugi::xpath_query logfQuery(L"./Log/File/text()");
            pugi::xpath_query logaQuery(L"./Log/Append/text()");
            pugi::xpath_query loglQuery(L"./Log/Level/text()");

            String logFile  = m_Parser->Parse(logfQuery.evaluate_string(globalNode));
            String logLevelStr = m_Parser->Parse(loglQuery.evaluate_string(globalNode));
            String logAppendStr = m_Parser->Parse(logaQuery.evaluate_string(globalNode));

            LogLevel logLevel = LL_Default;
            try
            {
                logLevel = (LogLevel)stoi(logLevelStr.ToString());
            }
            catch (const std::exception & exc)
            {
                UNREFERENCED_PARAMETER(exc);
                logLevel = LL_Default;
            }

            bool logAppend = logAppendStr.Compare(VSTR("true"), false) || logAppendStr == VSTR("1");

            m_Logger->Open(logFile, logAppend);
            m_Logger->SetFilter(logLevel);

            // Log extended build information
            String configMsg = m_Parser->Parse(VSTR("Config loaded from '$(config)'."));
            m_Logger->LogMessage(LL_CoreNotice, VOODOO_CORE_NAME, configMsg);
            m_Logger->LogMessage(LL_CoreNotice, VOODOO_CORE_NAME, VOODOO_GLOBAL_COPYRIGHT_FULL);

            Version vsver = VOODOO_VERSION_STRUCT(VC);
            Version cgver = VOODOO_VERSION_STRUCT(CG);
            m_Logger->LogMessage(LL_CoreNotice, VOODOO_CORE_NAME, Format("Loaded module: %1%") << vsver);
            m_Logger->LogMessage(LL_CoreNotice, VOODOO_CORE_NAME, Format("Loaded module: %1%") << cgver);

            // Load less vital classes
            m_FileSystem = dynamic_cast<IFileSystem*>(m_ModuleManager->CreateObject(fsClass));
            if (!m_FileSystem)
            {
                Format fmt(VSTR("Unable to create file system (class %1%).")); fmt << fsClass;
                Throw(VOODOO_CORE_NAME, fmt, this);
            }

            m_HookManager = dynamic_cast<IHookManager*>(m_ModuleManager->CreateObject(hookClass));
            if (!m_HookManager)
            {
                Format fmt(VSTR("Unable to create hook manager (class %1%).")); fmt << hookClass;
                Throw(VOODOO_CORE_NAME, fmt, this);
            }

            m_Adapter = dynamic_cast<IAdapter*>(m_ModuleManager->CreateObject(adpClass));
            if (!m_Adapter)
            {
                Format fmt(VSTR("Unable to create adapter (class %1%).")); fmt << adpClass;
                Throw(VOODOO_CORE_NAME, fmt, this);
            }

            // ICore done loading
            m_Logger->LogMessage(LL_CoreInfo, VOODOO_CORE_NAME, VSTR("Core initialization complete."));

            // Return
        }
        catch (const Exception & exc)
        {
            if (m_Logger)
            {
                m_Logger->LogMessage(LL_CoreError, VOODOO_CORE_NAME, Format(VSTR("Exception during Core creation: %1%")) << exc.strwhat());
            } else {
                GlobalLog(VSTR("Unlogged exception during core creation: %s"), exc.what());
            }

            return false;
        }
        catch (const std::exception & exc)
        {
            if (m_Logger)
            {
                m_Logger->LogMessage(LL_CoreError, VOODOO_CORE_NAME, Format(VSTR("Error during Core creation: %1%")) << exc.what());
            }

            return false;
        }

        return true;
    }

    bool VOODOO_METHODTYPE VSCore::Reset()
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);
        //! @todo Actually reset stuff.
        return true;
    }

    uint32_t VOODOO_METHODTYPE VSCore::AddRef() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);
        return SAFE_INCREMENT(m_Refs);
    }

    uint32_t VOODOO_METHODTYPE VSCore::Release() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);
        uint32_t count = SAFE_DECREMENT(m_Refs);
        if (count == 0)
        {
            delete this;
        }
        return count;
    }

    bool VOODOO_METHODTYPE VSCore::QueryInterface(_In_ Uuid refid, _Deref_out_opt_ const void ** ppOut) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);
        if (!ppOut)
        {
            if (clsid.is_nil())
            {
                clsid = IID_ICore;
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
            else if (clsid == IID_ICore)
            {
                *ppOut = static_cast<const ICore*>(this);
            }
            else if (clsid == CLSID_VSCore)
            {
                *ppOut = static_cast<const VSCore*>(this);
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

    String VOODOO_METHODTYPE VSCore::ToString() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);
        return Format("VSCore(%1%)") << m_Version;
    }

    ICore * VOODOO_METHODTYPE VSCore::GetCore() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);
        return nullptr;
    }

    IParser * VOODOO_METHODTYPE VSCore::GetParser() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);
        return m_Parser.get();
    }

    IHookManager * VOODOO_METHODTYPE VSCore::GetHookManager() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);
        return m_HookManager.get();
    }

    IFileSystem * VOODOO_METHODTYPE VSCore::GetFileSystem() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);
        return m_FileSystem.get();
    }

    IAdapter * VOODOO_METHODTYPE VSCore::GetAdapter() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);
        return m_Adapter.get();
    }

    ILogger * VOODOO_METHODTYPE VSCore::GetLogger() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);
        return m_Logger.get();
    }

    XmlDocument VOODOO_METHODTYPE VSCore::GetConfig() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);
        return m_ConfigFile;
    }

    CGcontext VOODOO_METHODTYPE VSCore::GetCgContext() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);
        return m_CgContext;
    }

    bool VOODOO_METHODTYPE VSCore::SetCgContext(CGcontext pContext)
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);
        if (pContext == nullptr)
        {
            m_Logger->LogMessage(LL_CoreDebug, VOODOO_CORE_NAME, VSTR("Setting Cg context to nullptr."));

            m_Parameters.clear();
            m_Textures.clear();
            m_LastPass = nullptr;
            m_LastShader = nullptr;

            m_CgContext = pContext;

            return true;
        }
        else if (m_CgContext != nullptr)
        {
            m_Logger->LogMessage
            (
                LL_CoreError, VOODOO_CORE_NAME,
                Format(VSTR("Error: Attempting to set Cg context %1% over existing context %2%.")) << pContext << m_CgContext
            );

            return false;
        }
        else
        {
            m_CgContext = pContext;
            cgSetErrorHandler(Voodoo_CgErrorHandler_Func, this);
            m_Logger->LogMessage
            (
                LL_CoreDebug, VOODOO_CORE_NAME, 
                Format(VSTR("Set Cg context (%1%).")) << m_CgContext
            );
            return true;
        }
    }

    IShader * VOODOO_METHODTYPE VSCore::CreateShader(_In_ const IFile * pFile, _In_opt_ const char **ppArgs)
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);
        if (!pFile || !m_CgContext)
        {
            return nullptr;
        }

        String fullpath = pFile->GetPath();

        IShaderRef shader = nullptr;

        try
        {
            shader = new VSShader(this, fullpath, ppArgs);
            m_Logger->LogMessage
            (
                LL_CoreDebug, VOODOO_CORE_NAME, 
                Format(VSTR("Successfully created shader from %1%.")) << fullpath
            );
        }
        catch (const std::exception & exc)
        {
            m_Logger->LogMessage
            (
                LL_CoreError, VOODOO_CORE_NAME,
                Format(VSTR("Error creating shader from %1%: %2%")) << fullpath << exc.what()
            );
        }

        if (shader)
        {
            m_Shaders.push_back(shader);
        }

        return shader.get();
    }

    IParameter * VOODOO_METHODTYPE VSCore::CreateParameter(const String & name, const ParameterType type)
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);
        if (!m_CgContext)
        {
            return nullptr;
        }

        ParameterMap::iterator paramEntry = m_Parameters.find(name);

        if (paramEntry != m_Parameters.end())
        {
            m_Logger->LogMessage(LL_CoreWarning, VOODOO_CORE_NAME, VSTR("Trying to create a parameter with a duplicate name."));
            return nullptr;
        }
        else
        {
            IParameter * parameter = nullptr;

            try
            {
                parameter = new VSParameter(this, name, type);

                m_Parameters[name] = parameter;

                m_Logger->LogMessage
                (
                    LL_CoreDebug, VOODOO_CORE_NAME,
                    Format(VSTR("Created parameter %1% with type %2%.")) << parameter << type
                );
            }
            catch (const std::exception & exc)
            {
                m_Logger->LogMessage
                (
                    LL_CoreDebug, VOODOO_CORE_NAME,
                    Format(VSTR("Error creating parameter %1% with type %2%: %3%")) << name << type << exc.what()
                );
            }

            return parameter;
        }
    }

    ITexture * VOODOO_METHODTYPE VSCore::CreateTexture(_In_ const String & name, _In_ const TextureDesc pDesc)
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);
        if (!m_CgContext)
        {
            return nullptr;
        }

        TextureMap::iterator textureEntry = m_Textures.find(name);

        if (textureEntry != m_Textures.end())
        {
            m_Logger->LogMessage(LL_CoreWarning, VOODOO_CORE_NAME, Format(VSTR("Trying to create texture with a duplicate name: %1%.")) << name);
            return nullptr;
        }
        else
        {
            ITexture * texture = m_Adapter->CreateTexture(name, pDesc);

            m_Textures[name] = texture;

            m_Logger->LogMessage(LL_CoreDebug, VOODOO_CORE_NAME, Format(VSTR("Created texture %1%.")) << name);

            return texture;
        }
    }

    IParameter * VOODOO_METHODTYPE VSCore::GetParameter(const String & name, const ParameterType type) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);
        ParameterMap::const_iterator parameter = m_Parameters.find(name);

        if (parameter != m_Parameters.end())
        {
            m_Logger->LogMessage(LL_CoreDebug, VOODOO_CORE_NAME, Format(VSTR("Got parameter %1%.")) << name);

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
            m_Logger->LogMessage(LL_CoreWarning, VOODOO_CORE_NAME, Format(VSTR("Unable to find parameter %1%.")) << name);
            return nullptr;
        }
    }

    ITexture * VOODOO_METHODTYPE VSCore::GetTexture(const String & name) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);
        TextureMap::const_iterator textureEntry = m_Textures.find(name);

        if (textureEntry != m_Textures.end())
        {
            m_Logger->LogMessage
            (
                LL_CoreDebug, VOODOO_CORE_NAME, 
                Format(VSTR("Got texture %1%.")) << name
            );

            return textureEntry->second.get();
        }
        else
        {
            m_Logger->LogMessage(LL_CoreDebug, VOODOO_CORE_NAME, Format(VSTR("Unable to find texture %1%.")) << name);

            return nullptr;
        }
    }

    bool VOODOO_METHODTYPE VSCore::RemoveParameter(_In_ const String & name)
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);
        ParameterMap::iterator parameter = m_Parameters.find(name);

        if (parameter != m_Parameters.end())
        {
            m_Parameters.erase(parameter);
            m_Logger->LogMessage(LL_CoreDebug, VOODOO_CORE_NAME, Format(VSTR("Removed parameter %1%.")) << name);
            return true;
        }
        else
        {
            m_Logger->LogMessage(LL_CoreDebug, VOODOO_CORE_NAME, Format(VSTR("Unable to find parameter %1%.")) << name);
            return false;
        }
    }

    bool VOODOO_METHODTYPE VSCore::RemoveTexture(_In_ const String & name)
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);
        TextureMap::iterator texture = m_Textures.find(name);

        if (texture != m_Textures.end())
        {
            m_Textures.erase(texture);
            m_Logger->LogMessage(LL_CoreDebug, VOODOO_CORE_NAME, Format(VSTR("Removed texture %1%.")) << name);
            return true;
        }
        else
        {
            m_Logger->LogMessage(LL_CoreDebug, VOODOO_CORE_NAME, Format(VSTR("Unable to find texture %1%.")) << name);
            return false;
        }
    }

    ITexture * VOODOO_METHODTYPE VSCore::GetStageTexture(_In_ const TextureStage stage) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);
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

    void VOODOO_METHODTYPE VSCore::SetStageTexture(_In_ const TextureStage Stage, _In_opt_ ITexture * const pTexture)
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);
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

    void VSCore::CgErrorHandler(CGcontext context, CGerror error) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);
        if (!m_Adapter || !m_Adapter->HandleError(context, error))
        {
            const char * errorString = error ? cgGetErrorString(error) : nullptr;

            if (errorString)
            {
                this->GetLogger()->LogMessage(LL_CoreError, VOODOO_CG_NAME, Format(VSTR("Cg core reported error %1%: %2%")) << error << errorString);
                if (context && error != CG_INVALID_CONTEXT_HANDLE_ERROR)
                {
                    // Print any compiler errors or other details we can find
                    const char *listing = cgGetLastListing(context);

                    if (listing)
                    {
                        this->GetLogger()->LogMessage(LL_CoreError, VOODOO_CG_NAME, Format(VSTR("Cg error details: %1%")) << listing);
                    }
                }
                else
                {
                    this->GetLogger()->LogMessage(LL_CoreError, VOODOO_CG_NAME, VSTR("Invalid context for error, no further data available."));
                }
            }
            else
            {
                this->GetLogger()->LogMessage(LL_CoreError, VOODOO_CG_NAME, Format(VSTR("Cg core reported an unknown error (%1%).")) << error);
            }
        }
    }
}
