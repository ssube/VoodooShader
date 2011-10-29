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
#pragma once

#include "Includes.hpp"

#include "IAdapter.hpp"
#include "IFilesystem.hpp"
#include "IHookManager.hpp"
#include "ILogger.hpp"

/**
 * @file Null.hpp 
 * 
 * Contains null implementations of all the core plugins. 
 * 
 * @note These are not exposed to other code or visible by other code. In order to use them, the core library must be loaded 
 *     as a plugin.
 */
namespace VoodooShader
{
    namespace VoodooNull
    {
        class NullAdapter :
            public IAdapter
        {
        public:
            NullAdapter(ICore * pCore);
            virtual ~NullAdapter(void);

            virtual int32_t AddRef() const;
            virtual int32_t Release() const;
            virtual String ToString(void) const;
            virtual ICore * GetCore(void) const;

            virtual bool LoadPass(_In_ IPass * const pPass);
            virtual bool UnloadPass(_In_ IPass * const pPass);
            virtual void SetPass(_In_opt_ IPass * const pPass);
            virtual IPass * GetPass(void) const;
            virtual bool SetTarget(_In_ uint32_t index, _In_opt_ ITexture * const pTarget);
            virtual ITexture * GetTarget(_In_ uint32_t index) const;
            virtual ITexture * CreateTexture(_In_ const String & name, _In_ const TextureDesc * pDesc);
            virtual ITexture * LoadTexture(_In_ const String & name, _In_ TextureRegion region);
            virtual void DrawGeometry(_In_ int32_t vertexes, _In_ const VertexStruct * const pVertexData);
            virtual void DrawShader(_In_ IShader * const pShader);
            virtual void ApplyParameter(_In_ IParameter * const pParam);
            virtual void SetProperty(const String & property, const String & value);
            virtual String GetProperty(const String & property) const;
            virtual bool ConnectTexture(_In_ IParameter * const pParam, _In_opt_ ITexture * const pTexture);
            virtual void HandleError(_In_ CGcontext const pContext, _In_ int32_t error);

        private:
            mutable int32_t m_Refs;
            ICore * m_Core;
        };

        class NullFileSystem :
            public IFileSystem
        {
        public:
            NullFileSystem(_In_ ICore * pCore);
            virtual ~NullFileSystem(void);

            virtual int32_t AddRef() const;
            virtual int32_t Release() const;
            virtual String ToString(void) const;
            virtual ICore * GetCore(void) const;

            virtual void AddPath(_In_ const String & dir);
            virtual void RemovePath(_In_ const String & dir);
            virtual IFile * FindFile(_In_ const String & name) const;

        private:
            mutable int32_t m_Refs;
            ICore * m_Core;
        };

        class NullHookManager :
            public IHookManager
        {
        public:
            NullHookManager(_In_ ICore * pCore);
            virtual ~NullHookManager(void);

            virtual int32_t AddRef() const;
            virtual int32_t Release() const;
            virtual String ToString(void) const;
            virtual ICore * GetCore(void) const;

            bool Add(_In_ const String & name, _In_ void * pSrc, _In_ void * pDest);
            bool Remove(_In_ const String & name);
            void RemoveAll(void);

        private:
            mutable int32_t m_Refs;
            ICore * m_Core;
        };

        class NullLogger :
            public ILogger
        {
        public:
            NullLogger(ICore * pCore);
            virtual ~NullLogger(void);

            virtual int32_t AddRef() const;
            virtual int32_t Release() const;
            virtual String ToString(void) const;
            virtual ICore * GetCore(void) const;

            virtual bool Open(_In_ const IFile * pFile, _In_ bool append);
            virtual void Close(void);
            virtual void Flush(void);
            virtual void SetLogLevel(_In_ LogLevel level);
            virtual LogLevel GetLogLevel(void) const;
            virtual void LogModule(_In_ const Version * pModule);
            virtual void Log
            (
                _In_ LogLevel level,
                _In_ const String & module,
                _In_ _Printf_format_string_ const String & msg,
                ...
            );
            virtual void SetFlags(_In_ LogFlags flush);
            virtual LogFlags GetFlags(void) const;

        private:
            mutable int32_t m_Refs;
            ICore * m_Core;
        };
    }
}
