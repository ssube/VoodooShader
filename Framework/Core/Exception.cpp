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

#include "VoodooFramework.hpp"

namespace VoodooShader
{
    static const wchar_t * ExceptionLogMsg = VSTR("Exception thrown from %1% in file %2% (line %3%): %4%");
    static const char * ExceptionFmtMsg = "VoodooShader::Exception thrown from %1% in module %2%, file %3% at %4% (line %5%): %6%";

    Exception::Exception
    (
        _In_z_ const wchar_t * file,
        _In_z_ const wchar_t * function,
        _In_ const int line,
        _In_ const wchar_t * module,
        _In_ const wchar_t * message,
        _In_opt_ ICore * pCore
    ) :
        m_Core(pCore)
    {
        StringFormat msg(ExceptionLogMsg);
        msg << file << function << line << message;
        m_What = msg;

        if (pCore)
        {
            LoggerRef logger = pCore->GetLogger();

            if (logger)
            {
                logger->LogMessage(VSLog_CoreException, module, msg);
            }
        }
    }

    Exception::Exception
    (
        _In_z_ const wchar_t * file,
        _In_z_ const wchar_t * function,
        _In_ const int line,
        _In_ const String module,
        _In_ const String message,
        _In_opt_ ICore * pCore
    ) :
            m_Core(pCore)
    {
        StringFormat msg(ExceptionLogMsg);
        msg << file << function << line << message;
        m_What = msg;

        if (pCore)
        {
            LoggerRef logger = pCore->GetLogger();

            if (logger)
            {
                logger->LogMessage(VSLog_CoreException, module, msg);
            }
        }
    }

    Exception::~Exception()
    {
    }

    const char * Exception::what() const
    {
        return (const char*)m_What.GetData();
    }

    const String Exception::strwhat() const
    {
        return m_What;
    }
}
