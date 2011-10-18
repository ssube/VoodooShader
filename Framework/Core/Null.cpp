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
#include "Null.hpp"

#include "IPass.hpp"
#include "ITexture.hpp"

namespace VoodooShader
{
    namespace VoodooNull
    {
        NullAdapter::NullAdapter(ICore *core) :
            m_Core(core)
        { }

        NullAdapter::~NullAdapter(void)
        { }

        ICore *NullAdapter::GetCore(void)
        {
            return m_Core;
        }

        String NullAdapter::ToString(void)
        {
            return "NullAdapter";
        }

        bool NullAdapter::LoadPass(_In_ IPass* pass)
        {
            UNREFERENCED_PARAMETER(pass);

            return false;
        }

        bool NullAdapter::UnloadPass(_In_ IPass* pass)
        {
            UNREFERENCED_PARAMETER(pass);

            return false;
        }

        void NullAdapter::SetPass(_In_ IPass* pass)
        {
            UNREFERENCED_PARAMETER(pass);
        }

        IPass* NullAdapter::GetPass(void)
        {
            return nullptr;
        }

        Bool NullAdapter::SetTarget(_In_ UInt32 index, _In_ ITexture* target)
        {
            UNREFERENCED_PARAMETER(index);
            UNREFERENCED_PARAMETER(target);
        }

        ITexture* NullAdapter::GetTarget(_In_ UInt32 index)
        {
            UNREFERENCED_PARAMETER(index);

            return nullptr;
        }

        ITexture* NullAdapter::CreateTexture(_In_ String Name, _In_ TextureDesc Desc)
        {
            UNREFERENCED_PARAMETER(Name);
            UNREFERENCED_PARAMETER(Desc);

            return nullptr;
        }

        ITexture* NullAdapter::LoadTexture(_In_ String Name, TextureRegion Region)
        {
            UNREFERENCED_PARAMETER(Name);
            UNREFERENCED_PARAMETER(Region);

            return nullptr;
        }

        void NullAdapter::DrawGeometry(int Vertexes, VertexStruct *pVertexData)
        {
            UNREFERENCED_PARAMETER(Vertexes);
            UNREFERENCED_PARAMETER(pVertexData);
        }

        void NullAdapter::DrawShader(_In_ IShader* shader)
        {
            UNREFERENCED_PARAMETER(shader);
        }

        void NullAdapter::ApplyParameter(_In_ IParameter* param)
        {
            UNREFERENCED_PARAMETER(param);
        }

        void NullAdapter::SetProperty(String Property, String Value)
        {
            UNREFERENCED_PARAMETER(Property);
            UNREFERENCED_PARAMETER(Value);
        }

        String NullAdapter::GetProperty(String Property)
        {
            UNREFERENCED_PARAMETER(Property);

            return String();
        }

        bool NullAdapter::ConnectTexture(_In_ IParameter* param, _In_ ITexture* texture)
        {
            UNREFERENCED_PARAMETER(param);
            UNREFERENCED_PARAMETER(texture);

            return false;
        }

        void NullAdapter::HandleError(_In_ CGcontext context, _In_ int error)
        {
            UNREFERENCED_PARAMETER(context);
            UNREFERENCED_PARAMETER(error);
        }

        NullLogger::NullLogger(ICore *core) 
            : m_Core(core)
        { }

        NullLogger::~NullLogger(void)
        { }

        ICore *NullLogger::GetCore(void)
        {
            return m_Core;
        }

        String NullLogger::ToString(void)
        {
            return "NullLogger";
        }

        bool NullLogger::Open(_In_ const IFile * pFile, _In_ bool Append)
        {
            UNREFERENCED_PARAMETER(Filename);
            UNREFERENCED_PARAMETER(Append);

            return false;
        }

        void NullLogger::Close(void)
        { }

        void NullLogger::Flush(void)
        { }

        void NullLogger::SetLogLevel(_In_ LogLevel level)
        {
            UNREFERENCED_PARAMETER(level);
        }

        LogLevel NullLogger::GetLogLevel(void)
        {
            return LL_Unknown;
        }

        void NullLogger::LogModule(_In_ Version module)
        {
            UNREFERENCED_PARAMETER(module);
        }

        void NullLogger::Log(_In_ LogLevel level, _In_ const char *module, _In_ _Printf_format_string_ const char *msg, ...)
        {
            UNREFERENCED_PARAMETER(level);
            UNREFERENCED_PARAMETER(module);
            UNREFERENCED_PARAMETER(msg);
        }

        void NullLogger::SetFlags(_In_ LogFlags flush)
        {
            UNREFERENCED_PARAMETER(flush);
        }

        LogFlags NullLogger::GetFlags(void)
        {
            return LF_Unknown;
        }

        NullHookManager::NullHookManager(_In_ ICore *core) :
        m_Core(core)
        { }

        NullHookManager::~NullHookManager(void)
        { }

        String NullHookManager::ToString(void)
        {
            return "NullHookManager";
        }

        ICore *NullHookManager::GetCore(void)
        {
            return m_Core;
        }

        bool NullHookManager::Add(_In_ String name, _In_ void *src, _In_ void *dest)
        {
            UNREFERENCED_PARAMETER(name);
            UNREFERENCED_PARAMETER(src);
            UNREFERENCED_PARAMETER(dest);

            return false;
        }

        bool NullHookManager::Remove(_In_ String name)
        {
            UNREFERENCED_PARAMETER(name);

            return false;
        }

        void NullHookManager::RemoveAll(void)
        { }

        NullFileSystem::NullFileSystem(_In_ ICore *core) :
        m_Core(core)
        { }

        NullFileSystem::~NullFileSystem(void)
        { }

        String NullFileSystem::ToString(void)
        {
            return "NullFileSystem";
        }

        ICore *NullFileSystem::GetCore(void)
        {
            return m_Core;
        }

        void NullFileSystem::AddDirectory(_In_ String dir)
        {
            UNREFERENCED_PARAMETER(dir);
        }

        void NullFileSystem::RemoveDirectory(_In_ String dir)
        {
            UNREFERENCED_PARAMETER(dir);
        }

        IFile* NullFileSystem::FindFile(_In_ String name)
        {
            UNREFERENCED_PARAMETER(name);

            return nullptr;
        }
    }
}
