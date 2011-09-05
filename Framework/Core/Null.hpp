/**************************************************************************************************\
 * This file is part of the Voodoo Shader Framework, a comprehensive shader support library.
 * Copyright (c) 2010-2011 by Sean Sube
 *
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program;
 * if  not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301 US
 *
 * Support and more information may be found at http://www.voodooshader.com, or by contacting the
 * developer at peachykeen@voodooshader.com
\**************************************************************************************************/

#pragma once

#include "Meta.hpp"

#include "IAdapter.hpp"
#include "IFilesystem.hpp"
#include "IHookManager.hpp"
#include "ILogger.hpp"

/**
 * @file Null.hpp
 *
 * Contains null implementations of all the core plugins.
 * @note These are not exposed to other code or visible by other code.
 *       In order to use them, the core library must be loaded as a plugin.
 */

namespace VoodooShader
{
    namespace VoodooNull
    {
        class NullAdapter
                    : public IAdapter
        {
        public:
            NullAdapter(Core * core);
            virtual ~NullAdapter();

            virtual Core * GetCore();
            virtual String GetName();

            virtual bool LoadPass(_In_ PassRef pass);
            virtual bool UnloadPass(_In_ PassRef pass);
            virtual void SetPass(_In_ PassRef pass);
            virtual PassRef GetPass();
            virtual void SetTarget(_In_ TextureRef target);
            virtual TextureRef GetTarget();
            virtual TextureRef CreateTexture(_In_ String Name, _In_ TextureDesc Desc);
            virtual TextureRef LoadTexture(_In_ String Name, TextureRegion Region);
            virtual void DrawGeometry(int Vertexes, VertexStruct * pVertexData);
            virtual void DrawShader(_In_ ShaderRef shader);
            virtual void ApplyParameter(_In_ ParameterRef param);
            virtual void SetProperty(String Property, String Value);
            virtual String GetProperty(String Property);
            virtual bool ConnectTexture(_In_ ParameterRef param, _In_ TextureRef texture);
            virtual void HandleError(_In_ CGcontext context, _In_ int error);

        private:
            Core * m_Core;
        };

        class NullLogger
                    : public ILogger
        {
        public:
            NullLogger(Core * core);
            virtual ~NullLogger();

            virtual Core * GetCore();
            virtual String GetName();

            virtual bool Open(_In_ String Filename, _In_ bool Append);
            virtual void Close();
            virtual void Flush();
            virtual void SetLogLevel(_In_ LogLevel level);
            virtual LogLevel GetLogLevel();
            virtual void LogModule(_In_ Version module);
            virtual void Log(_In_ LogLevel level, _In_ const char * module,  _In_ _Printf_format_string_ const char * msg, ...);
            virtual void SetFlags(_In_ LogFlags flush);
            virtual LogFlags GetFlags();

        private:
            Core * m_Core;
        };

        class NullHookManager
                    : public IHookManager
        {
        public:
            NullHookManager(_In_ Core * core);
            virtual ~NullHookManager();

            virtual String GetName();
            virtual Core * GetCore();

            bool Add(_In_ String name, _In_ void * src,  _In_ void * dest);
            bool Remove(_In_ String name);
            void RemoveAll();

        private:
            Core * m_Core;
        };

        class NullFileSystem
                    : public IFileSystem
        {
        public:
            NullFileSystem(_In_ Core * core);
            virtual ~NullFileSystem();

            virtual String GetName();
            virtual Core * GetCore();

            virtual void AddDirectory(_In_ String dir);
            virtual void RemoveDirectory(_In_ String dir);
            virtual IFileRef FindFile(_In_ String name);

        private:
            Core * m_Core;
        };
    }
}