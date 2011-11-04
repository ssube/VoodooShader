/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2011 by Sean Sube
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

#include "Exception.hpp"

#include "ICore.hpp"
#include "ILogger.hpp"

namespace VoodooShader
{
    Exception::Exception
    (
        _In_ wchar_t * module,
        _In_ wchar_t * message,
        _In_opt_ ICore * pCore,
        _In_ char * file,
        _In_ char * function,
        _In_ int line
    ) :
        m_Core(pCore),
        m_Module(module),
        m_Message(message),
        m_File(file),
        m_Function(function),
        m_Line(line),
        m_FmtMsg(nullptr)
    {
        if (pCore)
        {
            ILoggerRef logger = pCore->GetLogger();

            if (logger)
            {
                logger->Log(LL_Error, module, L"Exception in %S at %S (%d): %s", file, function, line, message);
            }
        }
    }

    Exception::Exception
    (
        _In_ String module,
        _In_ String message,
        _In_opt_ ICore * pCore,
        _In_ char *file,
        _In_ char *function,
        _In_ int line
    ) :
            m_Core(pCore),
            m_Module(module),
            m_Message(message),
            m_File(file),
            m_Function(function),
            m_Line(line),
            m_FmtMsg(nullptr)
    {
        if (pCore)
        {
            ILoggerRef logger = pCore->GetLogger();

            if (logger)
            {
                logger->Log
                (
                    LL_Error,
                    module.GetData(),
                    L"Exception in %S at %S (%d): %s",
                    file,
                    function,
                    line,
                    message.GetData()
                );
            }
        }
    }

    Exception::~Exception(void)
    {
        if (m_FmtMsg)
        {
            delete[] m_FmtMsg;
        }
    }

    const char * Exception::what(void)
    {
        if (m_FmtMsg == nullptr)
        {
            m_FmtMsg = new char[1024];
            ZeroMemory(m_FmtMsg, 1024);

            sprintf_s
            (
                m_FmtMsg,
                1024,
                "VoodooShader::Exception in module %S, file %s at %s (line %d): %S",
                m_Module.GetData(),
                m_File,
                m_Function,
                m_Line,
                m_Message.GetData()
            );
        }

        return m_FmtMsg;
    }
}
