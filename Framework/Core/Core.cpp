#include "Core.hpp"

#include "IAdapter.hpp"
#include "IFilesystem.hpp"
#include "IHookManager.hpp"
#include "ILogger.hpp"

#include "Converter.hpp"
#include "Exception.hpp"
#include "Module.hpp"
#include "Parameter.hpp"
#include "Parser.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include "pugixml.hpp"

namespace VoodooShader
{
    void Voodoo_CgErrorHandler_Func(CGcontext context, CGerror error, void * core)
    {
        Core * me = reinterpret_cast<Core*>(core);
        if (me)
        {
            me->CgErrorHandler(context, error);
        }
    }

    Core * CreateCore(_In_ InitParams * initParams)
    {
        return new Core(initParams);
    }

    void DestroyCore(_In_ Core * core)
    {
        delete core;
    }

    Core::Core(_In_ InitParams * initParams)
            : m_Adapter(NULL), m_HookManager(NULL), m_Logger(NULL), m_CgContext(NULL), m_ConfigFile(NULL)
    {
        using namespace pugi;

#ifdef _DEBUG_FULL
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG|_CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
        _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG|_CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif

        m_GlobalRoot    = initParams->GlobalRoot    + "\\";
        m_LocalRoot     = initParams->LocalRoot     + "\\";
        m_RunRoot       = initParams->RunRoot       + "\\";
        m_Target        = initParams->Target;
        m_Loader        = initParams->Loader;
        m_Config        = initParams->Config;

        // Set up the internal objects
        m_ModuleManager = ModuleManagerRef(new ModuleManager(this));
        m_Parser = ParserRef(new Parser(this));

        try
        {
            xml_document * config = new xml_document();

            // Try loading the config file from each major location
            String ConfigPath = m_RunRoot + m_Config;
            xml_parse_result result = config->load_file(ConfigPath.c_str());

            if (!result)
            {
                ConfigPath = m_LocalRoot + m_Config;
                result = config->load_file(ConfigPath.c_str());

                if (!result)
                {
                    ConfigPath = m_GlobalRoot + m_Config;
                    result = config->load_file(ConfigPath.c_str());

                    if (!result)
                    {
                        throw std::exception("Unable to find or parse config file.");
                    }
                }
            }

            // Store the config file, in case modules need it
            m_ConfigFile = (void *)config;

            // Start setting things up
            xml_node coreNode = config->select_single_node("/VoodooConfig/Core").node();

            if (!coreNode)
            {
                throw std::exception("Could not find Core node.");
            }

            // Create query for node text, used multiple times
            xpath_query nodeTextQuery("./text()");

            // Load variables, built-in first
            m_Parser->Add("globalroot", m_GlobalRoot, VT_System);
            m_Parser->Add("localroot", m_LocalRoot, VT_System);
            m_Parser->Add("runroot", m_RunRoot, VT_System);
            m_Parser->Add("target", m_Target, VT_System);

            xpath_query varNodesQuery("/VoodooConfig/Variables/Variable");
            xpath_query varNodeNameQuery("./@name");

            xpath_node_set varNodes = varNodesQuery.evaluate_node_set(*config);
            xpath_node_set::const_iterator varNodeIter = varNodes.begin();
            while (varNodeIter != varNodes.end())
            {
                String name = varNodeNameQuery.evaluate_string(*varNodeIter);
                String value = nodeTextQuery.evaluate_string(*varNodeIter);
                m_Parser->Add(name, value);

                ++varNodeIter;
            }

            // Load plugins
            {
                xpath_query pluginQuery("/VoodooConfig/Plugins/Path");
                xpath_node_set pluginNodes = pluginQuery.evaluate_node_set(*config);
                xpath_node_set::const_iterator pluginNodeIter = pluginNodes.begin();
                while (pluginNodeIter != pluginNodes.end())
                {
                    String path = nodeTextQuery.evaluate_string(*pluginNodeIter);
                    path = m_Parser->Parse(path);
                    m_ModuleManager->LoadPath(path);

                    ++pluginNodeIter;
                }
            }

            {
                xpath_query pluginQuery("/VoodooConfig/Plugins/File");
                xpath_node_set pluginNodes = pluginQuery.evaluate_node_set(*config);
                xpath_node_set::const_iterator pluginNodeIter = pluginNodes.begin();
                while (pluginNodeIter != pluginNodes.end())
                {
                    String file = nodeTextQuery.evaluate_string(*pluginNodeIter);
                    file = m_Parser->Parse(file);
                    m_ModuleManager->LoadFile(file);

                    ++pluginNodeIter;
                }
            }

            // Lookup classes
            xpath_query logQuery("/VoodooConfig/Core/Class/Logger/text()");
            xpath_query fsQuery("/VoodooConfig/Core/Class/FileSystem/text()");
            xpath_query hookQuery("/VoodooConfig/Core/Class/HookManager/text()");
            xpath_query adpQuery("/VoodooConfig/Core/Class/Adapter/text()");
            xpath_query logfQuery("/VoodooConfig/Core/Class/LogFile/text()");

            String logClass  = m_Parser->Parse(logQuery.evaluate_string(*config));
            String fsClass   = m_Parser->Parse(fsQuery.evaluate_string(*config));
            String hookClass = m_Parser->Parse(hookQuery.evaluate_string(*config));
            String adpClass  = m_Parser->Parse(adpQuery.evaluate_string(*config));
            String logFile   = m_Parser->Parse(logfQuery.evaluate_string(*config));

            // Make sure a logger was loaded
            m_Logger = m_ModuleManager->CreateClass<ILogger>(logClass);
            if (m_Logger.get() == nullptr)
            {
                throw std::exception("Unable to create Logger object (class not found).");
            }

            // Log extended build information
            m_Logger->Log(LL_Info, VOODOO_CORE_NAME, "Config loaded from \"%s\".", ConfigPath.c_str());
            m_Logger->Log(LL_Info, VOODOO_CORE_NAME, VOODOO_GLOBAL_COPYRIGHT_FULL);

            Version vfver = VOODOO_META_VERSION_STRUCT(CORE);
            Version vsver = VOODOO_META_VERSION_STRUCT(VC);
            Version cgver = VOODOO_META_VERSION_STRUCT(CG);

            m_Logger->LogModule(vfver);
            m_Logger->LogModule(vsver);
            m_Logger->LogModule(cgver);

            // Load less vital classes
            m_FileSystem = m_ModuleManager->CreateClass<IFileSystem>(fsClass);
            if (m_FileSystem.get() == nullptr)
            {
                throw std::exception("Unable to create FileSystem object.");
            }

            m_HookManager = m_ModuleManager->CreateClass<IHookManager>(hookClass);
            if (m_HookManager.get() == nullptr)
            {
                throw std::exception("Unable to create HookManager object.");
            }

            m_Adapter = m_ModuleManager->CreateClass<IAdapter>(adpClass);
            if (m_Adapter.get() == nullptr)
            {
                throw std::exception("Unable to create Adapter object.");
            }

            // Core done loading
            m_Logger->Log(LL_Info, VOODOO_CORE_NAME, "Core initialization complete.");

            // Return
        }
        catch (std::exception & exc)
        {
            if (this->m_Logger.get())
            {
                m_Logger->Log(LL_Error, VOODOO_CORE_NAME, "Error during Core creation: %s", exc.what());
            }

            throw exc;
        }
    }

    Core::~Core()
    {
        this->SetCgContext(nullptr);

        // Destroy adapter (not sure what it depends on, so handle first)
        this->m_Adapter = nullptr;

        this->m_HookManager = nullptr;
        this->m_FileSystem = nullptr;
        this->m_Logger = nullptr;

        this->m_ModuleManager = nullptr;

#ifdef _DEBUG_FULL
        _CrtDumpMemoryLeaks();
#endif
    }

    ParserRef Core::GetParser()
    {
        return m_Parser;
    }

    IHookManagerRef Core::GetHookManager()
    {
        return m_HookManager;
    }

    IFileSystemRef Core::GetFileSystem()
    {
        return m_FileSystem;
    }

    IAdapterRef Core::GetAdapter()
    {
        return m_Adapter;
    }

    ILoggerRef Core::GetLogger()
    {
        return m_Logger;
    }

    void * Core::GetConfig()
    {
        return m_ConfigFile;
    }

    CGcontext Core::GetCgContext()
    {
        return m_CgContext;
    }

    bool Core::SetCgContext(CGcontext Context)
    {
        if (Context == nullptr)
        {
            m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, "Setting Cg context to null.");

            m_Shaders.clear();
            m_Parameters.clear();
            m_Textures.clear();
            m_LastPass = nullptr;
            m_LastShader = nullptr;

            return true;
        }
        else if (m_CgContext != nullptr)
        {
            m_Logger->Log
            (
                LL_Error, VOODOO_CORE_NAME, "Error: Attempting to set Cg context (%p) over existing context (%p).",
                Context, m_CgContext
            );

            return false;
        }
        else
        {
            m_CgContext = Context;

            m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, "Set Cg context (%p).", Context);

            return true;
        }
    }

    ShaderRef Core::CreateShader
    (
        _In_ String Filename,
        _In_opt_ const char ** ppArgs
    )
    {
        IFileRef file = this->m_FileSystem->FindFile(Filename);
        if (file.get() == nullptr)
        {
            return nullptr;
        }

        String fullpath = file->GetPath();

        ShaderRef shader = Shader::Create(this, fullpath, ppArgs);

        m_Logger->Log
        (
            LL_Debug, VOODOO_CORE_NAME, "Created shader from %s, returning shared pointer to %p.",
            fullpath.c_str(), shader.get()
        );

        this->m_Shaders.push_back(shader);

        return shader;
    }

    ParameterRef Core::CreateParameter(String Name, ParameterType Type)
    {
        ParameterMap::iterator paramEntry = this->m_Parameters.find(Name);

        if (paramEntry != this->m_Parameters.end())
        {
            Throw(VOODOO_CORE_NAME, "Trying to create a parameter with a duplicate name.", this);
        }
        else
        {
            ParameterRef parameter(new Parameter(this, Name, Type));
            m_Parameters[Name] = parameter;

            m_Logger->Log
            (
                LL_Debug, VOODOO_CORE_NAME, "Created parameter named %s with type %s, returning shared pointer to %p.",
                Name.c_str(), Converter::ToString(Type), parameter.get()
            );

            return parameter;
        }
    }

    TextureRef Core::CreateTexture
    (
        _In_ String Name,
        _In_ TextureDesc Desc
    )
    {
        TextureMap::iterator textureEntry = this->m_Textures.find(Name);

        if (textureEntry != this->m_Textures.end())
        {
            Throw(VOODOO_CORE_NAME, "Trying to create a texture with a duplicate name.", this);
        }
        else
        {
            TextureRef texture = m_Adapter->CreateTexture(Name, Desc);

            this->m_Textures[Name] = texture;

            m_Logger->Log
            (
                LL_Debug, VOODOO_CORE_NAME, "Added texture %s with data %p, returning shared pointer to %p.",
                Name.c_str(), &Desc, texture.get()
            );

            return texture;
        }
    }

    ParameterRef Core::GetParameter(String Name, ParameterType Type)
    {
        ParameterMap::iterator parameter = this->m_Parameters.find(Name);

        if (parameter != this->m_Parameters.end())
        {
            m_Logger->Log
            (
                LL_Debug, VOODOO_CORE_NAME, "Got parameter %s, returning shared pointer to %p.",
                Name.c_str(), parameter->second.get()
            );

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

    TextureRef Core::GetTexture(_In_ String Name)
    {
        TextureMap::iterator textureEntry = this->m_Textures.find(Name);
        if (textureEntry != this->m_Textures.end())
        {
            m_Logger->Log
            (
                LL_Debug, VOODOO_CORE_NAME, "Got texture %s, returning shared pointer to %p.",
                Name.c_str(), textureEntry->second.get()
            );

            return textureEntry->second;
        }
        else
        {
            m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, "Unable to find texture %s.", Name.c_str());

            return nullptr;
        }
    }

    bool Core::RemoveParameter(_In_ String Name)
    {
        ParameterMap::iterator parameter = this->m_Parameters.find(Name);

        if (parameter != this->m_Parameters.end())
        {
            m_Logger->Log
            (
                LL_Debug, VOODOO_CORE_NAME, "Got parameter %s, erasing.",
                Name.c_str()
            );

            this->m_Parameters.erase(parameter);
            return true;
        }
        else
        {
            m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, "Unable to find parameter %s.", Name.c_str());

            return false;
        }
    }

    bool Core::RemoveTexture(_In_ String Name)
    {
        TextureMap::iterator texture = this->m_Textures.find(Name);
        if (texture != this->m_Textures.end())
        {
            m_Logger->Log
            (
                LL_Debug, VOODOO_CORE_NAME, "Got texture %s, returning shared pointer to %p.",
                Name.c_str(), texture->second.get()
            );

            this->m_Textures.erase(texture);
            return true;
        }
        else
        {
            m_Logger->Log(LL_Debug, VOODOO_CORE_NAME, "Unable to find texture %s.", Name.c_str());

            return false;
        }
    }

    TextureRef Core::GetStageTexture(_In_ TextureStage Stage)
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

    void Core::SetStageTexture(_In_ TextureStage Stage, _In_ TextureRef pTexture)
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

    void Core::CgErrorHandler(CGcontext context, int error)
    {
        const char * errorString = error ? cgGetErrorString((CGerror)error) : NULL;

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
                const char * listing = cgGetLastListing(context);

                while (listing)
                {
                    this->GetLogger()->Log(LL_Error, VOODOO_CG_NAME, "Cg error details: %s", listing);
                    listing = cgGetLastListing(context);
                }
            }
            else
            {
                this->GetLogger()->Log(LL_Error, VOODOO_CG_NAME, "Invalid context for error, no further data available.");
            }
        }
        else
        {
            this->GetLogger()->Log(LL_Error, VOODOO_CG_NAME, "Cg core reported an unknown error (%d).", error);
        }
    }
}
