/**
 * This file is part of the Voodoo Shader Framework, a comprehensive shader support library. 
 * 
 * Copyright (c) 2010-2011 by Sean Sube 
 * 
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation; either version 2 of the License, or (at your 
 * option) any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License along with this program; if not, write to 
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 US 
 * 
 * Support and more information may be found at 
 *   http://www.voodooshader.com
 * or by contacting the lead developer at 
 *   peachykeen@voodooshader.com
 */

#include "Converter.hpp"
#include "Exception.hpp"
#include "Xml.hpp"

#include "IAdapter.hpp"
#include "ICore.hpp"
#include "IFilesystem.hpp"
#include "IHookManager.hpp"
#include "ILogger.hpp"
#include "IModule.hpp"
#include "IParameter.hpp"
#include "IParser.hpp"
#include "IShader.hpp"
#include "ITexture.hpp"

namespace VoodooShader
{
    void Voodoo_CgErrorHandler_Func(CGcontext Context, CGerror Error, void *pCore)
    {
        if (pCore)
        {
            ICore *pSrcCore = reinterpret_cast<ICore *>(pCore);
            pSrcCore->CgErrorHandler(Context, Error);
        }
    }

    ICore * CreateCore(_In_ const InitParams * const pInitParams, Bool CatchErrors)
    {
        ICore *pCore = nullptr;

        try
        {
            pCore = new ICore(pInitParams);
            return pCore;
        }
        catch(std::exception & exc)
        {
            if (!CatchErrors)
            {
                throw exc;
            }
            else
            {
                if (pCore)
                {
                    delete pCore;
                }

                return nullptr;
            }
        }
    }

    ICore::ICore(_In_ const InitParams * const pInitParams) 
        : m_CgContext(nullptr), m_ConfigFile(nullptr)
    {
        if (!pInitParams)
        {
            throw std::exception("No init parameters provided.");
        }

#ifdef VSF_DEBUG_MEMORY
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
        _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif

        // Set up the internal objects
        m_ModuleManager = new IModuleManager(this);
        m_Parser = new IParser(this);

        // Load variables, built-in first
        m_Parser->Add("globalroot", pInitParams->GlobalRoot + "\\", VT_System);
        m_Parser->Add("localroot", pInitParams->LocalRoot + "\\", VT_System);
        m_Parser->Add("runroot", pInitParams->RunRoot + "\\", VT_System);
        m_Parser->Add("target", pInitParams->Target, VT_System);
        m_Parser->Add("loader", pInitParams->Loader, VT_System);

        String Config = m_Parser->Parse(pInitParams->Config);

        m_Parser->Add("config", Config, VT_System);

        try
        {
            m_ConfigFile = new Xml::Document();

            // Try loading the config file from each major location
            String ConfigPath = m_Parser->Parse("$(runroot)$(config)");
            Bool result = m_ConfigFile->LoadFile(ConfigPath);

            if (!result)
            {
                ConfigPath = m_Parser->Parse("$(localroot)$(config)");
                result = m_ConfigFile->LoadFile(ConfigPath);
                if (!result)
                {
                    ConfigPath = m_Parser->Parse("$(globalroot)$(config)");
                    result = m_ConfigFile->LoadFile(ConfigPath);
                    if (!result)
                    {
                        ConfigPath = m_Parser->Parse("$(config)");
                        result = m_ConfigFile->LoadFile(ConfigPath);
                        if (!result)
                        {
                            throw std::exception("Unable to find or parse config file.");
                        }
                    }
                }
            }

            // Start setting things up
            Xml::Node * configRoot = m_ConfigFile->GetRoot();
            Xml::Node * coreNode = configRoot->GetSingleNode("/VoodooConfig/ICore");
            if (!coreNode)
            {
                throw std::exception("Could not find ICore node.");
            }

            // Create query for node text, used multiple times
            XPath::Query xpq_getName("./@name");
            XPath::Query xpq_getText("./text()");

            // Load variables
            {
                XPath::Query xpq_getVariables("/VoodooConfig/Variables/Variable");
                XPath::NodeSet nodes = xpq_getVariables.EvaluateNodeSet(configRoot);
                XPath::NodeSetIter iter = nodes.begin();

                while (iter != nodes.end())
                {
                    String name = xpq_getName.evaluate_string(*iter);
                    String value = xpq_getText.evaluate_string(*iter);

                    m_Parser->Add(name, value);

                    ++iter;
                }
            }

            // Load plugins
            {
                xpath_query xpq_getPluginPaths("/VoodooConfig/Plugins/Path");
                xpath_query xpq_getFilter("./@filter");
                xpath_node_set nodes = xpq_getPluginPaths.evaluate_node_set(*config);
                xpath_node_set::const_iterator iter = nodes.begin();

                while (iter != nodes.end())
                {
                    String filter = m_Parser->Parse(xpq_getFilter.evaluate_string(*iter));
                    String path = m_Parser->Parse(xpq_getText.evaluate_string(*iter));

                    m_ModuleManager->LoadPath(path, filter);

                    ++iter;
                }
            }

            {
                xpath_query xpq_getPluginFiles("/VoodooConfig/Plugins/File");
                xpath_node_set nodes = xpq_getPluginFiles.evaluate_node_set(*config);
                xpath_node_set::const_iterator iter = nodes.begin();

                while (iter != nodes.end())
                {
                    String file = m_Parser->Parse(xpq_getText.evaluate_string(*iter));

                    m_ModuleManager->LoadFile(file);

                    ++iter;
                }
            }

            // Lookup classes
            xpath_query logQuery("/VoodooConfig/ICore/Class/Logger/text()");
            xpath_query fsQuery("/VoodooConfig/ICore/Class/FileSystem/text()");
            xpath_query hookQuery("/VoodooConfig/ICore/Class/HookManager/text()");
            xpath_query adpQuery("/VoodooConfig/ICore/Class/Adapter/text()");
            xpath_query logfQuery("/VoodooConfig/ICore/Class/Logger/@file");
            xpath_query loglQuery("/VoodooConfig/ICore/Class/Logger/@level");
            String logClass = m_Parser->Parse(logQuery.evaluate_string(*config));
            String fsClass = m_Parser->Parse(fsQuery.evaluate_string(*config));
            String hookClass = m_Parser->Parse(hookQuery.evaluate_string(*config));
            String adpClass = m_Parser->Parse(adpQuery.evaluate_string(*config));
            String logFile = m_Parser->Parse(logfQuery.evaluate_string(*config));
            LogLevel logLevel = (LogLevel) (int) loglQuery.evaluate_number(*config);

            // Make sure a logger was loaded
            m_Logger = m_ModuleManager->CreateClass<ILogger>(logClass);
            if (m_Logger.get() == nullptr)
            {
                throw std::exception("Unable to create Logger object (class not found).");
            }

            m_Logger->Open(logFile, false);
            m_Logger->SetLogLevel(logLevel);

            // Log extended build information
            m_Logger->Log(LL_Info, VOODOO_CORE_NAME, "Config loaded from '%s'.", Config.c_str());
            m_Logger->Log(LL_Info, VOODOO_CORE_NAME, VOODOO_GLOBAL_COPYRIGHT_FULL);

            Version vfver = VOODOO_META_VERSION_STRUCT(CORE);
            Version vsver = VOODOO_META_VERSION_STRUCT(VC);
            Version cgver = VOODOO_META_VERSION_STRUCT(CG);

            m_Logger->LogModule(vfver);
            m_Logger->LogModule(vsver);
            m_Logger->LogModule(cgver);

            // Load less vital classes
            m_FileSystem = m_ModuleManager->CreateClass<IFileSystem>(fsClass);
            if (!m_FileSystem)
            {
                throw std::exception("Unable to create FileSystem object.");
            }

            m_HookManager = m_ModuleManager->CreateClass<IHookManager>(hookClass);
            if (!m_HookManager)
            {
                throw std::exception("Unable to create HookManager object.");
            }

            m_Adapter = m_ModuleManager->CreateClass<IAdapter>(adpClass);
            if (!m_Adapter)
            {
                throw std::exception("Unable to create Adapter object.");
            }

            // ICore done loading
            m_Logger->Log(LL_Info, VOODOO_CORE_NAME, "ICore initialization complete.");

            // Return
        }
        catch(std::exception & exc)
        {
            if (this->m_Logger.get())
            {
                m_Logger->Log(LL_Error, VOODOO_CORE_NAME, "Error during ICore creation: %s", exc.what());
            }

            throw exc;
        }
    }

    ICore::~ICore(void)
    {
        this->SetCgContext(nullptr);

        this->m_Adapter = nullptr;
        this->m_HookManager = nullptr;
        this->m_FileSystem = nullptr;
        this->m_Logger = nullptr;

        this->m_ModuleManager = nullptr;

#ifdef VSF_DEBUG_MEMORY
        _CrtDumpMemoryLeaks();
#endif
    }

    IParser* ICore::GetParser(void)
    {
        return m_Parser;
    }

    IHookManager* ICore::GetHookManager(void)
    {
        return m_HookManager;
    }

    IFileSystem* ICore::GetFileSystem(void)
    {
        return m_FileSystem;
    }

    IAdapter* ICore::GetAdapter(void)
    {
        return m_Adapter;
    }

    ILogger* ICore::GetLogger(void)
    {
        return m_Logger;
    }

    XmlDocument ICore::GetConfig(void)
    {
        return m_ConfigFile;
    }

    CGcontext ICore::GetCgContext(void)
    {
        return m_CgContext;
    }

    bool ICore::SetCgContext(CGcontext Context)
    {
        if (Context == nullptr)
        {
            m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, "Setting Cg context to nullptr.");

            m_Shaders.clear();
            m_Parameters.clear();
            m_Textures.clear();
            m_LastPass = nullptr;
            m_LastShader = nullptr;

            return true;
        }
        else if (m_CgContext != nullptr)
        {
            m_Logger->Log(LL_Error, VOODOO_CORE_NAME,
                "Error: Attempting to set Cg context (%p) over existing context (%p).", Context, m_CgContext);

            return false;
        }
        else
        {
            m_CgContext = Context;
            m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, "Set Cg context (%p).", Context);
            return true;
        }
    }

    IShader* ICore::CreateShader(_In_ const IFile * pFile, _In_opt_ const char **ppArgs)
    {
        IFile* file = this->m_FileSystem->FindFile(Filename);

        if (!file)
        {
            return nullptr;
        }

        String fullpath = file->GetPath();
        IShaderRef shader = IShader(this, fullpath, ppArgs);

        m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, "Created shader from %s, returning shared pointer to %p.",
            fullpath.c_str(), shader.get());

        this->m_Shaders.push_back(shader);

        return shader;
    }

    IParameter* ICore::CreateParameter(String Name, ParameterType Type)
    {
        ParameterMap::iterator paramEntry = this->m_Parameters.find(Name);

        if (paramEntry != this->m_Parameters.end())
        {
            m_Logger->Log(LL_Warning, VOODOO_CORE_NAME, "Trying to create a parameter with a duplicate name.");
            return nullptr;
        }
        else
        {
            IParameter* parameter(new IParameter(this, Name, Type));

            m_Parameters[Name] = parameter;

            m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, 
                "Created parameter named %s with type %s, returning shared pointer to %p.", Name.c_str(),
                Converter::ToString(Type), parameter);

            return parameter;
        }
    }

    ITexture* ICore::CreateTexture(_In_ String Name, _In_ TextureDesc Desc)
    {
        TextureMap::iterator textureEntry = this->m_Textures.find(Name);

        if (textureEntry != this->m_Textures.end())
        {
            m_Logger->Log(LL_Warning, VOODOO_CORE_NAME, "Trying to create a texture with a duplicate name.");
            return nullptr;
        }
        else
        {
            ITexture* texture = m_Adapter->CreateTexture(Name, Desc);

            this->m_Textures[Name] = texture;

            m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, "Added texture %s, returning shared pointer to %p.",
                Name.c_str(), texture);

            return texture;
        }
    }

    IParameter* ICore::GetParameter(String Name, ParameterType Type)
    {
        ParameterMap::iterator parameter = this->m_Parameters.find(Name);

        if (parameter != this->m_Parameters.end())
        {
            m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, "Got parameter %s, returning shared pointer to %p.",
                Name.c_str(), parameter->second);

            if (Type == PT_Unknown)
            {
                return parameter->second;
            }
            else if (parameter->second->GetType() == Type)
            {
                return parameter->second;
            }
            else
            {
                return nullptr;
            }
        }
        else
        {
            m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, "Unable to find parameter %s.", Name.c_str());
            return nullptr;
        }
    }

    ITexture* ICore::GetTexture(_In_ String Name)
    {
        TextureMap::iterator textureEntry = this->m_Textures.find(Name);

        if (textureEntry != this->m_Textures.end())
        {
            m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, "Got texture %s, returning shared pointer to %p.", Name.c_str(),
                textureEntry->second);

            return textureEntry->second;
        }
        else
        {
            m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, "Unable to find texture %s.", Name.c_str());

            return nullptr;
        }
    }

    bool ICore::RemoveParameter(_In_ String Name)
    {
        ParameterMap::iterator parameter = this->m_Parameters.find(Name);

        if (parameter != this->m_Parameters.end())
        {
            m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, "Got parameter %s, erasing.", Name.c_str());

            this->m_Parameters.erase(parameter);
            return true;
        }
        else
        {
            m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, "Unable to find parameter %s.", Name.c_str());

            return false;
        }
    }

    bool ICore::RemoveTexture(_In_ String Name)
    {
        TextureMap::iterator texture = this->m_Textures.find(Name);

        if (texture != this->m_Textures.end())
        {
            m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, "Got texture %s, returning shared pointer to %p.", Name.c_str(),
                texture->second);

            this->m_Textures.erase(texture);
            return true;
        }
        else
        {
            m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, "Unable to find texture %s.", Name.c_str());
            return false;
        }
    }

    ITexture * ICore::GetStageTexture(_In_ TextureStage Stage)
    {
        switch (Stage)
        {
        case TS_Shader:
            return m_LastShader;
        case TS_Pass:
            return m_LastPass;
        default:
            return nullptr;
        }
    }

    void ICore::SetStageTexture(_In_ TextureStage Stage, _In_ ITexture * const pTexture)
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

    void ICore::CgErrorHandler(CGcontext context, int error)
    {
        const char *errorString = error ? cgGetErrorString((CGerror) error) : nullptr;

        if (errorString)
        {
            this->GetLogger()->Log(LL_Error, VOODOO_CG_NAME, "Cg core reported error: %s", errorString);
            if (context && error != CG_INVALID_CONTEXT_HANDLE_ERROR)
            {
                if (this->m_Adapter)
                {
                    this->m_Adapter->HandleError(context, error);
                }

                // Print any compiler errors or other details we can find
                const char *listing = cgGetLastListing(context);

                while (listing)
                {
                    this->GetLogger()->Log(LL_Error, VOODOO_CG_NAME, "Cg error details: %s", listing);
                    listing = cgGetLastListing(context);
                }
            }
            else
            {
                this->GetLogger()->Log(LL_Error, VOODOO_CG_NAME, 
                    "Invalid context for error, no further data available.");
            }
        }
        else
        {
            this->GetLogger()->Log(LL_Error, VOODOO_CG_NAME, "Cg core reported an unknown error (%d).", error);
        }
    }
}
