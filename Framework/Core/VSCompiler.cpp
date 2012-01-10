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

#include "VSCompiler.hpp"
#include "VSShader.hpp"
#include "VSTechnique.hpp"
#include "VSPass.hpp"
#include "VSProgram.hpp"
#include "VSParameter.hpp"

#include <fstream>

namespace VoodooShader
{
    VoodooResult VOODOO_METHODTYPE VSCompiler::CreateEffectFromFile(ICore * pCore, IFile * pFile, CompileFlags flags, VSShader ** ppShader)
    {
        if (!pFile || !ppShader) return VSFERR_INVALIDPARAMS;

        // Cache core objects
        ILoggerRef  logger  = pCore->GetLogger();
        IAdapterRef adapter = pCore->GetAdapter();

        if (!logger || !adapter) return VSFERR_INVALIDPARAMS;

        pugi::xml_document vsfxDoc;

        std::wstring filepath = pFile->GetPath().ToString();
        pugi::xml_parse_result result = vsfxDoc.load_file(filepath.c_str());
        if (!result)
        {
            logger->LogMessage(LL_CoreError, VOODOO_CORE_NAME, Format("Unable to parse VSFX file '%1%'.") << filepath.c_str());
            return VSFERR_INVALIDXML;
        }

        pugi::xml_node shaderNode = vsfxDoc.child(VSTR("Shader"));
        if (!shaderNode)
        {
            logger->LogMessage(LL_CoreError, VOODOO_CORE_NAME, Format("Unable to find shader root node in VSFX file '%1%'.") << filepath.c_str());
            return VSFERR_INVALIDXML;
        }

        // Do that bit where the shader gets made
        boost::intrusive_ptr<VSShader> shader = new VSShader();

        // Get all definitions out of the shader
        std::vector<std::pair<std::string, std::string>> defines;

        pugi::xml_node defineNode = shaderNode.child(VSTR("Define"));
        while (defineNode)
        {
            pugi::xml_attribute name = defineNode.attribute(VSTR("name"));
            if (name)
            {
                String defname  = name.value();
                String defvalue = defineNode.value();
                logger->LogMessage(LL_CoreDebug, VOODOO_CORE_NAME, Format("Found preprocessor define '%1%' with value '%2%'.") << defname << defvalue);

                shader->m_Defines.push_back(StringPair(defname, defvalue));
                defines.push_back(std::pair<std::string, std::string>(defname.ToStringA(), defvalue.ToStringA()));
            }

            defineNode = defineNode.next_sibling();
        }

        // Get the source code
        pugi::xml_node sourceNode = shaderNode.child(VSTR("Source"));
        pugi::xml_node externalNode = shaderNode.child(VSTR("External"));
        if (!sourceNode && !externalNode)
        {
            return VSFERR_INVALIDXML;
        }

        std::string source;

        if (externalNode)
        {
            pugi::xml_attribute file = externalNode.attribute(VSTR("file"));
            IFileRef pSourceFile = pCore->GetFileSystem()->GetFile(file.value());
            if (!pSourceFile)
            {
                return VSFERR_INVALIDXML;
            }

            String sourcePath = pSourceFile->GetPath();
            try
            {
                std::ifstream ifs(sourcePath.GetData());
                source.append((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
            } 
            catch (std::exception & exc)
            {
                // Error reading source code
            }
        }

        if (sourceNode)
        {
            String wsource = sourceNode.value();
            source.append(wsource.ToStringA());
        }

        // Start compiling techniques; we'll cache the sampler from the first program
        pugi::xml_node techNode = shaderNode.child(VSTR("Technique"));
        while (techNode)
        {
            String name = techNode.attribute(VSTR("name")).value();
            boost::intrusive_ptr<VSTechnique> technique = new VSTechnique(name);

            // Start compiling passes
            pugi::xml_node passNode = techNode.child(VSTR("Pass"));
            while (passNode)
            {
                String name = passNode.attribute(VSTR("name")).value();
                boost::intrusive_ptr<VSPass> pass = new VSPass(name);

                // Start setting up uniforms
                pugi::xml_node uniformNode = passNode.child(VSTR("Uniform"));
                while (uniformNode)
                {
                    String name    = uniformNode.attribute(VSTR("name")).value();
                    String typeStr = uniformNode.attribute(VSTR("type")).value();
                    String link    = uniformNode.attribute(VSTR("link")).value();

                    if (!name.IsEmpty() && !typeStr.IsEmpty())
                    {
                        ParameterType type = Converter::ToParameterType(typeStr.GetData());
                        boost::intrusive_ptr<VSParameter> uniform = new VSParameter(name, type);

                        if (!link.IsEmpty())
                        {
                            IParameter * linkParam = pCore->GetParameter(link, type);
                            if (linkParam)
                            {
                                linkParam->AttachParameter(uniform.get());
                            }
                        }

                        pass->m_Uniforms[name.value()] = uniform;
                    }

                    uniformNode = uniformNode.next_sibling();
                }

                // Set up targets
                pugi::xml_node targetNode = passNode.child(VSTR("Target"));
                while (targetNode)
                {
                    ITextureRef target = pCore->GetTexture(targetNode.value());
                    pass->m_Targets.push_back(target);
                    targetNode = targetNode.next_sibling();
                }

                // Compile programs
                pugi::xml_node vertexNode = passNode.child(VSTR("Vertex"));
                if (vertexNode)
                {
                    String profile = vertexNode.attribute(VSTR("profile")).value();
                    String function = vertexNode.attribute(VSTR("function")).value();

                    VSProgram * program = nullptr;
                    VoodooResult vr = VSCompiler::CompileProgramDX9(pCore, source, defines, function.ToStringA(), profile.ToStringA(), flags, &program);

                    if (VSUCCESS(vr))
                    {
                        pass->m_PixelShader = program;
                    }
                }

                pugi::xml_node pixelNode = passNode.child(VSTR("Pixel"));
                if (pixelNode)
                {
                    String profile = pixelNode.attribute(VSTR("profile")).value();
                    String function = pixelNode.attribute(VSTR("function")).value();

                    VSProgram * program = nullptr;
                    VoodooResult vr = VSCompiler::CompileProgramDX9(pCore, source, defines, function.ToStringA(), profile.ToStringA(), flags, &program);

                    if (VSUCCESS(vr))
                    {
                        pass->m_PixelShader = program;
                    }
                }

                passNode = passNode.next_sibling();
            }

            shader->m_Techniques.push_back(technique.get());
            techNode = techNode.next_sibling();
        }

        // Get all the samplers out of the shader


        // Pass the shader back
        (*ppShader) = shader.get();
        (*ppShader)->AddRef();

        return VSF_OK;
    }

    VoodooResult VOODOO_METHODTYPE VSCompiler::CompileProgramDX9
    (
        ICore * pCore, const std::string & source,
        const std::vector<std::pair<std::string, std::string>> & defines, 
        const std::string & function, const std::string & profile, CompileFlags flags,
        VSProgram ** ppProgram
    )
    {
        // Build the flags
        DWORD d3dxflags = 0;
        if ((flags & CF_AvoidFlow) > 0)     d3dxflags |= D3DXSHADER_AVOID_FLOW_CONTROL;
        if ((flags & CF_PreferFlow) > 0)    d3dxflags |= D3DXSHADER_PREFER_FLOW_CONTROL;
        if ((flags & CF_Debug) > 0)         d3dxflags |= D3DXSHADER_DEBUG;
        if ((flags & CF_NoOpt) > 0)         d3dxflags |= D3DXSHADER_SKIPOPTIMIZATION;

        // Set up the defines buffer
        std::vector<D3DXMACRO> d3dxdefines(defines.size());
        std::for_each
        (
            defines.begin(), defines.end(), 
            [&d3dxdefines]
            (std::pair<std::string, std::string> define)
            {
                D3DXMACRO macro = {define.first.c_str(), define.second.c_str()};
                d3dxdefines.push_back(macro);
            }
        );

        // Create the buffer pointers
        LPD3DXBUFFER buffer = NULL;
        LPD3DXBUFFER errors = NULL;
        LPD3DXCONSTANTTABLE constants = NULL;

        // Compile
        HRESULT hr = D3DXCompileShader
        (
            source.c_str(), source.length(), 
            &d3dxdefines[0], NULL, function.c_str(), profile.c_str(), d3dxflags,
            &buffer, &errors, &constants
        );

        if (FAILED(hr))
        {
            if (errors)
            {
                pCore->GetLogger()->LogMessage(LL_CoreError, VOODOO_CORE_NAME, Format("Error compiling program:\n%1%") << (LPCSTR)errors->GetBufferPointer());
                errors->Release();
            } else {
                pCore->GetLogger()->LogMessage(LL_CoreError, VOODOO_CORE_NAME, Format("Unknown error while compiling program %1%") << function);
            }

            if (buffer)     buffer->Release();
            if (constants)  constants->Release();
            return VSFERR_COMPILEFAILED;
        }

        return VSF_OK;
    }
}