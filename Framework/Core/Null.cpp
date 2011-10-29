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
        NullAdapter::NullAdapter(ICore * pCore) :
            m_Core(pCore)
        { }

        NullAdapter::~NullAdapter(void)
        { }

        int32_t NullAdapter::AddRef() const
        {
            return ++m_Refs;
        }

        int32_t NullAdapter::Release() const
        {
            if (--m_Refs == 0)
            {
                delete this;
                return 0;
            } else {
                return m_Refs;
            }
        }

        String NullAdapter::ToString(void) const
        {
            return "NullAdapter";
        }

        ICore * NullAdapter::GetCore(void) const
        {
            return m_Core;
        }

        bool NullAdapter::LoadPass(_In_ IPass * pPass)
        {
            UNREFERENCED_PARAMETER(pPass);

            return true;
        }

        bool NullAdapter::UnloadPass(_In_ IPass * pPass)
        {
            UNREFERENCED_PARAMETER(pPass);

            return true;
        }

        void NullAdapter::SetPass(_In_ IPass * pPass)
        {
            UNREFERENCED_PARAMETER(pPass);
        }

        IPass * NullAdapter::GetPass(void) const
        {
            return nullptr;
        }

        bool NullAdapter::SetTarget(_In_ uint32_t index, _In_ ITexture * pTarget)
        {
            UNREFERENCED_PARAMETER(index);
            UNREFERENCED_PARAMETER(pTarget);

            return true;
        }

        ITexture * NullAdapter::GetTarget(_In_ uint32_t index) const
        {
            UNREFERENCED_PARAMETER(index);

            return nullptr;
        }

        ITexture * NullAdapter::CreateTexture(_In_ const String & name, _In_ const TextureDesc * pDesc)
        {
            UNREFERENCED_PARAMETER(name);
            UNREFERENCED_PARAMETER(pDesc);

            return nullptr;
        }

        ITexture * NullAdapter::LoadTexture(_In_ const String & name, TextureRegion region)
        {
            UNREFERENCED_PARAMETER(name);
            UNREFERENCED_PARAMETER(region);

            return nullptr;
        }

        void NullAdapter::DrawGeometry(int32_t count, const VertexStruct * const pVertexData)
        {
            UNREFERENCED_PARAMETER(count);
            UNREFERENCED_PARAMETER(pVertexData);
        }

        void NullAdapter::DrawShader(_In_ IShader * const pShader)
        {
            UNREFERENCED_PARAMETER(pShader);
        }

        void NullAdapter::ApplyParameter(_In_ IParameter * const pParam)
        {
            UNREFERENCED_PARAMETER(pParam);
        }

        void NullAdapter::SetProperty(const String & property, const String & value)
        {
            UNREFERENCED_PARAMETER(property);
            UNREFERENCED_PARAMETER(value);
        }

        String NullAdapter::GetProperty(const String & property) const
        {
            UNREFERENCED_PARAMETER(property);

            return String();
        }

        bool NullAdapter::ConnectTexture(_In_ IParameter * const pParam, _In_ ITexture * const pTexture)
        {
            UNREFERENCED_PARAMETER(pParam);
            UNREFERENCED_PARAMETER(pTexture);

            return true;
        }

        void NullAdapter::HandleError(_In_ CGcontext const pContext, _In_ int error)
        {
            UNREFERENCED_PARAMETER(pContext);
            UNREFERENCED_PARAMETER(error);
        }

        NullLogger::NullLogger(ICore * pCore) : 
            m_Core(pCore)
        { }

        NullLogger::~NullLogger(void)
        { }

        int32_t NullLogger::AddRef() const
        {
            return ++m_Refs;
        }

        int32_t NullLogger::Release() const
        {
            if (--m_Refs == 0)
            {
                delete this;
                return 0;
            } else {
                return m_Refs;
            }
        }

        String NullLogger::ToString(void) const
        {
            return L"NullLogger";
        }

        ICore * NullLogger::GetCore(void) const
        {
            return m_Core;
        }

        bool NullLogger::Open(_In_ const IFile * pFile, _In_ bool append)
        {
            UNREFERENCED_PARAMETER(pFile);
            UNREFERENCED_PARAMETER(append);

            return true;
        }

        void NullLogger::Close(void)
        { }

        void NullLogger::Flush(void)
        { }

        void NullLogger::SetLogLevel(_In_ LogLevel level)
        {
            UNREFERENCED_PARAMETER(level);
        }

        LogLevel NullLogger::GetLogLevel(void) const
        {
            return LL_Unknown;
        }

        void NullLogger::LogModule(_In_ const Version * pModule)
        {
            UNREFERENCED_PARAMETER(pModule);
        }

        void NullLogger::Log(_In_ LogLevel level, _In_ const String & module, _In_ _Printf_format_string_ const String & msg, ...)
        {
            UNREFERENCED_PARAMETER(level);
            UNREFERENCED_PARAMETER(module);
            UNREFERENCED_PARAMETER(msg);
        }

        void NullLogger::SetFlags(_In_ LogFlags flush)
        {
            UNREFERENCED_PARAMETER(flush);
        }

        LogFlags NullLogger::GetFlags(void) const
        {
            return LF_Unknown;
        }

        NullHookManager::NullHookManager(_In_ ICore * pCore) 
            : m_Core(pCore)
        { }

        NullHookManager::~NullHookManager(void)
        { }

        int32_t NullHookManager::AddRef() const
        {
            return ++m_Refs;
        }

        int32_t NullHookManager::Release() const
        {
            if (--m_Refs == 0)
            {
                delete this;
                return 0;
            } else {
                return m_Refs;
            }
        }

        String NullHookManager::ToString(void) const
        {
            return L"NullHookManager";
        }

        ICore * NullHookManager::GetCore(void) const
        {
            return m_Core;
        }

        bool NullHookManager::Add(_In_ const String & name, _In_ void * pSrc, _In_ void * pDest)
        {
            UNREFERENCED_PARAMETER(name);
            UNREFERENCED_PARAMETER(pSrc);
            UNREFERENCED_PARAMETER(pDest);

            return true;
        }

        bool NullHookManager::Remove(_In_ const String & name)
        {
            UNREFERENCED_PARAMETER(name);

            return true;
        }

        void NullHookManager::RemoveAll(void)
        { }

        NullFileSystem::NullFileSystem(_In_ ICore * pCore) : 
            m_Core(pCore)
        { }

        NullFileSystem::~NullFileSystem(void)
        { }

        int32_t NullFileSystem::AddRef() const
        {
            return ++m_Refs;
        }

        int32_t NullFileSystem::Release() const
        {
            if (--m_Refs == 0)
            {
                delete this;
                return 0;
            } else {
                return m_Refs;
            }
        }

        String NullFileSystem::ToString(void) const
        {
            return L"NullFileSystem";
        }

        ICore * NullFileSystem::GetCore(void) const
        {
            return m_Core;
        }

        void NullFileSystem::AddPath(_In_ const String & dir)
        {
            UNREFERENCED_PARAMETER(dir);
        }

        void NullFileSystem::RemovePath(_In_ const String & dir)
        {
            UNREFERENCED_PARAMETER(dir);
        }

        IFile * NullFileSystem::FindFile(_In_ const String & name) const
        {
            UNREFERENCED_PARAMETER(name);

            return nullptr;
        }
    }
}
