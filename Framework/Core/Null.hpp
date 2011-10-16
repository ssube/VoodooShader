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
            NullAdapter(ICore *core);
            virtual ~NullAdapter(void);

            virtual ICore *GetCore(void);
            virtual String ToString(void);

            virtual bool LoadPass(_In_ IPass* pass);
            virtual bool UnloadPass(_In_ IPass* pass);
            virtual void SetPass(_In_ IPass* pass);
            virtual IPass* GetPass(void);
            virtual Bool SetTarget(_In_ UInt32 index, _In_ ITexture* target);
            virtual ITexture* GetTarget(_In_ UInt32 index);
            virtual ITexture* CreateTexture(_In_ String Name, _In_ TextureDesc Desc);
            virtual ITexture* LoadTexture(_In_ String Name, TextureRegion Region);
            virtual void DrawGeometry(int Vertexes, VertexStruct *pVertexData);
            virtual void DrawShader(_In_ IShader* shader);
            virtual void ApplyParameter(_In_ IParameter* param);
            virtual void SetProperty(String Property, String Value);
            virtual String GetProperty(String Property);
            virtual bool ConnectTexture(_In_ IParameter* param, _In_ ITexture* texture);
            virtual void HandleError(_In_ CGcontext context, _In_ int error);

        private:
            ICore *m_Core;
        };

        class NullLogger :
            public ILogger
        {
        public:
            NullLogger(ICore *core);
            virtual ~NullLogger(void);

            virtual ICore *GetCore(void);
            virtual String ToString(void);

            virtual bool Open(_In_ String Filename, _In_ bool Append);
            virtual void Close(void);
            virtual void Flush(void);
            virtual void SetLogLevel(_In_ LogLevel level);
            virtual LogLevel GetLogLevel(void);
            virtual void LogModule(_In_ Version module);
            virtual void Log
            (
                _In_ LogLevel level,
                _In_ const char *module,
                _In_ _Printf_format_string_ const char *msg,
                ...
            );
            virtual void SetFlags(_In_ LogFlags flush);
            virtual LogFlags GetFlags(void);

        private:
            ICore *m_Core;
        };

        class NullHookManager :
            public IHookManager
        {
        public:
            NullHookManager(_In_ ICore *core);
            virtual ~NullHookManager(void);

            virtual String ToString(void);
            virtual ICore *GetCore(void);

            bool Add(_In_ String name, _In_ void *src, _In_ void *dest);
            bool Remove(_In_ String name);
            void RemoveAll(void);

        private:
            ICore *m_Core;
        };

        class NullFileSystem :
            public IFileSystem
        {
        public:
            NullFileSystem(_In_ ICore *core);
            virtual ~NullFileSystem(void);

            virtual String ToString(void);
            virtual ICore *GetCore(void);

            virtual void AddDirectory(_In_ String dir);
            virtual void RemoveDirectory(_In_ String dir);
            virtual IFile* FindFile(_In_ String name);

        private:
            ICore *m_Core;
        };
    }
}
