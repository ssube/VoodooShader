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

#include "ILogger.hpp"

namespace VoodooShader
{
    class VSLogger :
        public ILogger
    {
    public:
        VSLogger(ICore * pCore);
        virtual ~VSLogger(void);

        virtual uint32_t AddRef() const;
        virtual uint32_t Release() const;
        virtual String ToString(void) const;
        virtual ICore * GetCore(void) const;
        
        virtual bool Open(_In_ const String & filename, _In_ const bool append) throw();
        virtual bool Open(_In_ IFile * const pFile, _In_ const bool append) throw();
        virtual void Close(void);
        virtual void Flush(void);
        virtual void SetLogLevel(_In_ const LogLevel level);
        virtual const LogLevel GetLogLevel(void) const;
        virtual void LogModule(_In_ const Version * const pModule);
        virtual void Log
        (
            _In_ const LogLevel level,
            _In_ const String & module,
            _In_ _Printf_format_string_ const String & msg,
            ...
        );
        virtual void SetFlags(_In_ const LogFlags flush);
        virtual const LogFlags GetFlags(void) const;

    private:
        mutable uint32_t m_Refs;
        ICore * m_Core;
    };
}
