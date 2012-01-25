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
// Voodoo Core
#include "VSModuleManager.hpp"
#include "VSParser.hpp"
// Voodoo Utility
#include "Support.inl"
// System
#pragma warning(push)
#pragma warning(disable: 4668)
#include <shellapi.h>
#pragma warning(pop)

namespace VoodooShader
{
    #define VOODOO_DEBUG_TYPE VSCore
    DeclareDebugCache();
    HMODULE gCoreHandle = nullptr;

    _Check_return_ ICore * VOODOO_CALLTYPE CreateCore(uint32_t version)
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
        m_Refs(0), m_Version(version), m_ConfigFile(nullptr)
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

        m_LastPass = nullptr;
        m_LastShader = nullptr;

        m_Parameters.clear();
        m_Textures.clear();

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

    VOODOO_METHODDEF_(uint32_t, VSCore::AddRef)() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);
        return SAFE_INCREMENT(m_Refs);
    }

    VOODOO_METHODDEF_(uint32_t, VSCore::Release)() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);
        uint32_t count = SAFE_DECREMENT(m_Refs);
        if (count == 0)
        {
            delete this;
        }
        return count;
    }

    VoodooResult VOODOO_METHODTYPE VSCore::QueryInterface(_In_ Uuid refid, _Deref_out_opt_ IObject ** ppOut)
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
            else if (refid == IID_ICore)
            {
                *ppOut = static_cast<ICore*>(this);
            }
            else if (refid == CLSID_VSCore)
            {
                *ppOut = static_cast<VSCore*>(this);
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

    String VOODOO_METHODTYPE VSCore::ToString() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        return StringFormat("VSCore(%1%)") << m_Version;
    }

    ICore * VOODOO_METHODTYPE VSCore::GetCore() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        return nullptr;
    }

    VOODOO_METHODDEF(VSCore::Init)(_In_ CONST wchar_t * config)
    {
        if (config)
        {
            m_Parser->Add(VSTR("config"), config, VSVar_System);
        }
        else
        {
            m_Parser->Add(VSTR("config"), VSTR("VoodooShader.xml"), VSVar_System);
        }

        // Load variables, built-in first
        {
            wchar_t buffer[MAX_PATH];

            // Global path
            GetVoodooPath(buffer);
            m_Parser->Add(VSTR("path"), buffer, VSVar_System);

            // Bin prefix
            GetVoodooBinPrefix(buffer);
            m_Parser->Add(VSTR("prefix"), buffer, VSVar_System);

            // Framework bin path
            GetVoodooBinPath(buffer);
            m_Parser->Add(VSTR("binpath"), buffer, VSVar_System);

            // Target
            HMODULE pTarget = GetModuleHandle(NULL);
            GetModuleFileName(pTarget, buffer, MAX_PATH);
            m_Parser->Add(VSTR("target"), buffer, VSVar_System);

            // Local path
            PathRemoveFileSpec(buffer);
            PathAddBackslash(buffer);
            m_Parser->Add(VSTR("local"), buffer, VSVar_System);

            // Startup path
            GetCurrentDirectory(MAX_PATH, buffer);
            PathAddBackslash(buffer);
            m_Parser->Add(VSTR("startup"), buffer, VSVar_System);

            GetModuleFileName(gCoreHandle, buffer, MAX_PATH);
            m_Parser->Add(VSTR("core"), buffer, VSVar_System);
        }

        // Command line processing
        LPWSTR cmdline = GetCommandLine();
        m_Parser->Add(VSTR("args"), cmdline, VSVar_System);

        int cmdargc = 0;
        LPWSTR * cmdargv = CommandLineToArgvW(cmdline, &cmdargc);
        m_Parser->Add(VSTR("argc"), StringFormat(VSTR("%d")) << cmdargc, VSVar_System);
        for (int i = 0; i < cmdargc; ++i)
        {
            m_Parser->Add(StringFormat(VSTR("argv_%d")) << i, cmdargv[i], VSVar_System);
        }

        // Load the config
        try
        {
            m_ConfigFile = new pugi::xml_document();

            // Try loading the config file from each major location
            String configPath = m_Parser->Parse(VSTR("$(config)"), VSParse_PathCanon);
            pugi::xml_parse_result result = m_ConfigFile->load_file(configPath.GetData());

            if (!result)
            {
                configPath = m_Parser->Parse(VSTR("$(startup)\\$(config)"), VSParse_PathCanon);
                result = m_ConfigFile->load_file(configPath.GetData());
                if (!result)
                {
                    configPath = m_Parser->Parse(VSTR("$(local)\\$(config)"), VSParse_PathCanon);
                    result = m_ConfigFile->load_file(configPath.GetData());
                    if (!result)
                    {
                        configPath = m_Parser->Parse(VSTR("$(path)\\$(config)"), VSParse_PathCanon);
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
            String logClass = m_Parser->Parse(logQuery.evaluate_string(globalNode).c_str());
            String fsClass = m_Parser->Parse(fsQuery.evaluate_string(globalNode).c_str());
            String hookClass = m_Parser->Parse(hookQuery.evaluate_string(globalNode).c_str());

            // Make sure a logger was loaded
            IObject * coreplugin = m_ModuleManager->CreateObject(logClass);
            ILogger * plogger = nullptr;
            if (coreplugin && SUCCEEDED(coreplugin->QueryInterface(IID_ILogger, (IObject**)&plogger)) && plogger)
            {
                m_Logger = plogger;
            }
            else
            {
                String error = StringFormat(VSTR("Unable to create logger (class %1%).")) << logClass;
                Throw(VOODOO_CORE_NAME, error.GetData(), this);
            }

            pugi::xpath_query logfQuery(L"./Log/File/text()");
            pugi::xpath_query logaQuery(L"./Log/Append/text()");
            pugi::xpath_query loglQuery(L"./Log/Level/text()");

            String logFile  = m_Parser->Parse(logfQuery.evaluate_string(globalNode));
            String logLevelStr = m_Parser->Parse(loglQuery.evaluate_string(globalNode));
            String logAppendStr = m_Parser->Parse(logaQuery.evaluate_string(globalNode));

            LogLevel logLevel = VSLog_Default;
            try
            {
                logLevel = (LogLevel)stoi(logLevelStr.ToString());
            }
            catch (const std::exception & exc)
            {
                UNREFERENCED_PARAMETER(exc);
                logLevel = VSLog_Default;
            }

            bool logAppend = logAppendStr.Compare(VSTR("true"), false) || logAppendStr.StartsWith("1");

            m_Logger->Open(logFile, logAppend);
            m_Logger->SetFilter(logLevel);

            // Log extended build information
            String configMsg = m_Parser->Parse(VSTR("Config loaded from '$(config)'."));
            m_Logger->LogMessage(VSLog_CoreNotice, VOODOO_CORE_NAME, configMsg);
            m_Logger->LogMessage(VSLog_CoreNotice, VOODOO_CORE_NAME, VOODOO_GLOBAL_COPYRIGHT_FULL);

            // Load less vital classes
            coreplugin = m_ModuleManager->CreateObject(hookClass);
            IHookManager * phm = nullptr;
            if (coreplugin && SUCCEEDED(coreplugin->QueryInterface(IID_IHookManager, (IObject**)&phm)) && phm)
            {
                m_HookManager = phm;
                phm->Release();
            }
            else
            {
                StringFormat fmt(VSTR("Unable to create hook manager (class %1%).")); fmt << hookClass;
                Throw(VOODOO_CORE_NAME, fmt, this);
            }

            coreplugin = m_ModuleManager->CreateObject(fsClass);
            IFileSystem * pfs = nullptr;
            if (coreplugin && SUCCEEDED(coreplugin->QueryInterface(IID_IFileSystem, (IObject**)&pfs)) && pfs)
            {
                m_FileSystem = pfs;
                pfs->Release();
            }
            else
            {
                StringFormat fmt(VSTR("Unable to create file system (class %1%).")); fmt << fsClass;
                Throw(VOODOO_CORE_NAME, fmt, this);
            }

            // ICore done loading
            m_Logger->LogMessage(VSLog_CoreInfo, VOODOO_CORE_NAME, VSTR("Core initialization complete."));

            // Call finalization events
            this->CallEvent(EventIds::Finalize, 0, nullptr);

            // Return
            return VSF_OK;
        }
        catch (const Exception & exc)
        {
            if (m_Logger)
            {
                m_Logger->LogMessage(VSLog_CoreError, VOODOO_CORE_NAME, 
                    StringFormat(VSTR("Exception during Core creation: %1%")) << exc.strwhat());
            } else {
                GlobalLog(VSTR("Unlogged exception during core creation: %S"), exc.what());
            }

            return VSF_FAIL;
        }
        catch (const std::exception & exc)
        {
            if (m_Logger)
            {
                m_Logger->LogMessage(VSLog_CoreError, VOODOO_CORE_NAME, 
                    StringFormat(VSTR("Error during Core creation: %1%")) << exc.what());
            }

            return VSF_FAIL;
        }
    }

    VOODOO_METHODDEF(VSCore::Bind)(CompilerProfile profile, uint32_t count, Variant * pParams)
    {
        IModule * compiler = nullptr;

        if (profile == VSProfile_D3D9)
        {
            // Load D3D9 compiler
            if (FAILED(m_ModuleManager->LoadFile(VSTR("$(binpath)\\Voodoo_D3D9.dll"))))
            {
                return VSFERR_INVALIDCALL;
            }

            compiler = m_ModuleManager->GetModule(VSTR("Voodoo_D3D9"));
            if (!compiler)
            {
                return VSF_FAIL;
            }
        }
        else
        {
            return VSFERR_INVALIDPARAMS;
        }

        IObject * pCompObj = compiler->CreateClass(0, this);
        if (!pCompObj)
        {
            return VSF_FAIL;
        }

        IBinding * pBinding = nullptr;
        if (FAILED(pCompObj->QueryInterface(IID_IBinding, (IObject**)&pBinding)) || !pBinding)
        {
            pBinding->Release();
            return VSF_FAIL;
        }

        m_Binding = pBinding;
        pBinding->Release();

        return m_Binding->Init(count, pParams);
    }

    VOODOO_METHODDEF(VSCore::Reset)()
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);
        //! @todo Actually reset stuff.
        return VSF_OK;
    }

    VOODOO_METHODDEF(VSCore::AddEvent)(Uuid event, Functions::CallbackFunc func)
    {
        try
        {
            if (m_Events.find(event) == m_Events.end())
            {
                EventCallbacks set;
                m_Events.insert(std::pair<Uuid, EventCallbacks>(event, set));
            }

            m_Events[event].insert(func);

            return VSF_OK;
        }
        catch (std::exception & exc)
        {
            if (m_Logger)
            {
                m_Logger->LogMessage(VSLog_CoreError, VOODOO_CORE_NAME, 
                    StringFormat("Unable to register callback %1% for event %2%, exception: %3%") << func << event << exc.what());
            }

            return VSF_FAIL;
        }
    }

    VOODOO_METHODDEF(VSCore::DropEvent)(Uuid event, Functions::CallbackFunc func)
    {
        try
        {
            if (m_Events.find(event) == m_Events.end())
            {
                return VSF_OK;
            }

            m_Events[event].erase(func);

            return VSF_OK;
        }
        catch (std::exception & exc)
        {
            if (m_Logger)
            {
                m_Logger->LogMessage(VSLog_CoreError, VOODOO_CORE_NAME, 
                    StringFormat("Unable to register callback %1% for event %2%, exception: %3%") << func << event << exc.what());
            }

            return VSF_FAIL;
        }
    }

    VOODOO_METHODDEF(VSCore::CallEvent)(Uuid event, uint32_t count, Variant * pArgs)
    {
        try
        {
            std::map<Uuid, EventCallbacks>::iterator eventMapIter = m_Events.find(event);
            if (eventMapIter == m_Events.end())
            {
                return VSFERR_INVALIDPARAMS;
            }

            EventCallbacks::iterator eventCallIter = eventMapIter->second.begin();
            while (eventCallIter != eventMapIter->second.end())
            {
                VoodooResult vr = (*eventCallIter)(this, count, pArgs);
                if (FAILED(vr))
                {
                    return vr;
                }
                ++eventCallIter;
            }

            return VSF_OK;
        }
        catch (std::exception & exc)
        {
            if (m_Logger)
            {
                m_Logger->LogMessage(VSLog_CoreError, VOODOO_CORE_NAME, 
                    StringFormat("Unable to call event %1%, exception: %2%") << event << exc.what());
            }

            return VSF_FAIL;
        }
    }

    IParser * VOODOO_METHODTYPE VSCore::GetParser() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        return m_Parser.get();
    }

    IFileSystem * VOODOO_METHODTYPE VSCore::GetFileSystem() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        return m_FileSystem.get();
    }

    IHookManager * VOODOO_METHODTYPE VSCore::GetHookManager() CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        return m_HookManager.get();
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

    IEffect * VOODOO_METHODTYPE VSCore::CreateEffect(_In_ IFile * pFile)
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        if (!m_Binding) return nullptr;
        if (!pFile) return nullptr;

        IEffect * effect = nullptr;

        try
        {
            effect = m_Binding->CreateEffectFromFile(pFile);
            m_Logger->LogMessage
            (
                VSLog_CoreDebug, VOODOO_CORE_NAME, 
                StringFormat(VSTR("Successfully created effect from %1%.")) << pFile->GetPath()
            );
        }
        catch (const std::exception & exc)
        {
            m_Logger->LogMessage
            (
                VSLog_CoreError, VOODOO_CORE_NAME,
                StringFormat(VSTR("Error creating effect from %1%: %2%")) << pFile->GetPath() << exc.what()
            );
        }

        if (effect)
        {
            m_Effects.push_back(effect);
        }

        return effect;
    }

    IParameter * VOODOO_METHODTYPE VSCore::CreateParameter(const String & name, const ParameterDesc desc)
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        if (!m_Binding) return nullptr;

        ParameterMap::iterator paramEntry = m_Parameters.find(name);

        if (paramEntry != m_Parameters.end())
        {
            m_Logger->LogMessage(VSLog_CoreWarning, VOODOO_CORE_NAME, 
                VSTR("Trying to create a parameter with a duplicate name."));
            return nullptr;
        }
        else
        {
            IParameter * parameter = nullptr;

            try
            {
                parameter = m_Binding->CreateParameter(name, desc);

                m_Parameters[name] = parameter;

                m_Logger->LogMessage
                (
                    VSLog_CoreDebug, VOODOO_CORE_NAME,
                    StringFormat(VSTR("Created parameter %1% with type %2%.")) << parameter << desc
                );
            }
            catch (const std::exception & exc)
            {
                m_Logger->LogMessage
                (
                    VSLog_CoreDebug, VOODOO_CORE_NAME,
                    StringFormat(VSTR("Error creating parameter %1% with type %2%: %3%")) << name << desc << exc.what()
                );
            }

            return parameter;
        }
    }

    ITexture * VOODOO_METHODTYPE VSCore::CreateTexture(_In_ const String & name, _In_ const TextureDesc pDesc)
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        if (!m_Binding) return nullptr;

        TextureMap::iterator textureEntry = m_Textures.find(name);

        if (textureEntry != m_Textures.end())
        {
            m_Logger->LogMessage(VSLog_CoreWarning, VOODOO_CORE_NAME, 
                StringFormat(VSTR("Trying to create texture with a duplicate name: %1%.")) << name);
            return textureEntry->second.get();
        }
        else
        {
            ITexture * texture = m_Binding->CreateTexture(name, pDesc);

            m_Textures[name] = texture;

            m_Logger->LogMessage(VSLog_CoreDebug, VOODOO_CORE_NAME, StringFormat(VSTR("Created texture %1%.")) << name);

            return texture;
        }
    }

    ITexture * VOODOO_METHODTYPE VSCore::LoadTexture(_In_ const String & name, _In_ IFile * pFile)
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);
        
        if (!m_Binding) return nullptr;
        if (!pFile) return nullptr;

        TextureMap::iterator textureEntry = m_Textures.find(name);

        if (textureEntry != m_Textures.end())
        {
            m_Logger->LogMessage(VSLog_CoreWarning, VOODOO_CORE_NAME, 
                StringFormat(VSTR("Trying to create texture with a duplicate name: %1%.")) << name);
            return textureEntry->second.get();
        }
        else
        {
            ITexture * texture = m_Binding->CreateTextureFromFile(name, pFile);

            m_Textures[name] = texture;

            m_Logger->LogMessage(VSLog_CoreDebug, VOODOO_CORE_NAME, StringFormat(VSTR("Created texture %1%.")) << name);

            return texture;
        }
    }

    IParameter * VOODOO_METHODTYPE VSCore::GetParameter(const String & name, const ParameterDesc desc) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        if (!m_Binding) return nullptr;

        ParameterMap::const_iterator paramIter = m_Parameters.find(name);

        if (paramIter == m_Parameters.end())
        {
            m_Logger->LogMessage(VSLog_CoreWarning, VOODOO_CORE_NAME, 
                StringFormat(VSTR("Unable to find parameter %1%.")) << name);
            return nullptr;
        }

        m_Logger->LogMessage(VSLog_CoreDebug, VOODOO_CORE_NAME, StringFormat(VSTR("Got parameter %1%.")) << name);
        ParameterRef param = paramIter->second;
        ParameterDesc pdesc = param->GetDesc();

        if (desc.Type != VSPT_Unknown && desc.Type != pdesc.Type) return nullptr;
        if (desc.Rows != 0 && desc.Rows != pdesc.Rows) return nullptr;
        if (desc.Columns != 0 && desc.Columns != pdesc.Columns) return nullptr;
        if (desc.Elements != 0 && desc.Elements != pdesc.Elements) return nullptr;

        return param.get();
    }

    ITexture * VOODOO_METHODTYPE VSCore::GetTexture(const String & name) CONST
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        if (!m_Binding) return nullptr;

        TextureMap::const_iterator textureEntry = m_Textures.find(name);
        if (textureEntry != m_Textures.end())
        {
            m_Logger->LogMessage
            (
                VSLog_CoreDebug, VOODOO_CORE_NAME, 
                StringFormat(VSTR("Got texture %1%.")) << name
            );

            return textureEntry->second.get();
        }
        else
        {
            m_Logger->LogMessage(VSLog_CoreDebug, VOODOO_CORE_NAME, StringFormat(VSTR("Unable to find texture %1%.")) << name);

            return nullptr;
        }
    }

    VoodooResult VOODOO_METHODTYPE VSCore::RemoveParameter(_In_ const String & name)
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        if (!m_Binding) return VSFERR_INVALIDCALL;

        ParameterMap::iterator parameter = m_Parameters.find(name);
        if (parameter != m_Parameters.end())
        {
            m_Parameters.erase(parameter);
            m_Logger->LogMessage(VSLog_CoreDebug, VOODOO_CORE_NAME, StringFormat(VSTR("Removed parameter %1%.")) << name);
            return VSF_OK;
        }
        else
        {
            m_Logger->LogMessage(VSLog_CoreDebug, VOODOO_CORE_NAME, StringFormat(VSTR("Unable to find parameter %1%.")) << name);
            return VSFERR_INVALIDPARAMS;
        }
    }

    VoodooResult VOODOO_METHODTYPE VSCore::RemoveTexture(_In_ const String & name)
    {
        VOODOO_DEBUG_FUNCLOG(m_Logger);

        if (!m_Binding) return VSFERR_INVALIDCALL;

        TextureMap::iterator texture = m_Textures.find(name);
        if (texture != m_Textures.end())
        {
            m_Textures.erase(texture);
            m_Logger->LogMessage(VSLog_CoreDebug, VOODOO_CORE_NAME, StringFormat(VSTR("Removed texture %1%.")) << name);
            return VSF_OK;
        }
        else
        {
            m_Logger->LogMessage(VSLog_CoreDebug, VOODOO_CORE_NAME, StringFormat(VSTR("Unable to find texture %1%.")) << name);
            return VSFERR_INVALIDPARAMS;
        }
    }
}
