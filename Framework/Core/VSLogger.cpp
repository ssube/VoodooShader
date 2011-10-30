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

#include "VSLogger.hpp"

namespace VoodooShader
{
    VSLogger::VSLogger(ICore * pCore) : 
        m_Core(pCore)
    { }

    VSLogger::~VSLogger(void)
    { }

    int32_t VSLogger::AddRef() const
    {
        return ++m_Refs;
    }

    int32_t VSLogger::Release() const
    {
        if (--m_Refs == 0)
        {
            delete this;
            return 0;
        } else {
            return m_Refs;
        }
    }

    String VSLogger::ToString(void) const
    {
        return L"VSLogger";
    }

    ICore * VSLogger::GetCore(void) const
    {
        return m_Core;
    }

    bool VSLogger::Open(_In_ const IFile * pFile, _In_ bool append)
    {
        UNREFERENCED_PARAMETER(pFile);
        UNREFERENCED_PARAMETER(append);

        return true;
    }

    void VSLogger::Close(void)
    { }

    void VSLogger::Flush(void)
    { }

    void VSLogger::SetLogLevel(_In_ LogLevel level)
    {
        UNREFERENCED_PARAMETER(level);
    }

    LogLevel VSLogger::GetLogLevel(void) const
    {
        return LL_Unknown;
    }

    void VSLogger::LogModule(_In_ const Version * pModule)
    {
        UNREFERENCED_PARAMETER(pModule);
    }

    void VSLogger::Log(_In_ LogLevel level, _In_ const String & module, _In_ _Printf_format_string_ const String & msg, ...)
    {
        UNREFERENCED_PARAMETER(level);
        UNREFERENCED_PARAMETER(module);
        UNREFERENCED_PARAMETER(msg);
    }

    void VSLogger::SetFlags(_In_ LogFlags flush)
    {
        UNREFERENCED_PARAMETER(flush);
    }

    LogFlags VSLogger::GetFlags(void) const
    {
        return LF_Unknown;
    }
}
