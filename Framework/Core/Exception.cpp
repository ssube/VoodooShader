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

#include "Exception.hpp"

namespace VoodooShader
{
    Exception::Exception
    (
        _In_ char *module,
        _In_ char *message,
        _In_ ICore *core,
        _In_ char *file,
        _In_ char *function,
        _In_ int line
    ) :
    std::exception(message),
        m_FmtMsg(nullptr),
        m_Module(module),
        m_Message(message),
        m_Core(core),
        m_File(file),
        m_Function(function),
        m_Line(line)
    {
        if (core)
        {
            ILoggerRef logger(core->GetLogger());

            if (logger)
            {
                logger->Log(LL_Error, module, "Exception in %s at %s (%d): %s", file, function, line, message);
            }
        }
    }

    Exception::Exception
    (
        _In_ String module,
        _In_ String message,
        _In_ ICore *core,
        _In_ char *file,
        _In_ char *function,
        _In_ int line
    ) :
        std::exception(message.c_str()),
            m_FmtMsg(nullptr),
            m_Module(module),
            m_Message(message),
            m_Core(core),
            m_File(file),
            m_Function(function),
            m_Line(line)
    {
        if (core)
        {
            ILoggerRef logger(core->GetLogger());

            if (logger)
            {
                logger->Log
                (
                    LL_Error,
                    module.c_str(),
                    "Exception in %s at %s (%d): %s",
                    file,
                    function,
                    line,
                    message.c_str()
                );
            }
        }
    }

    Exception::~Exception(void)
    {
        if (m_FmtMsg)
        {
            delete m_FmtMsg;
        }
    }

    const char *Exception::what(void)
    {
        if (m_FmtMsg == nullptr)
        {
            m_FmtMsg = new char[1024];
            ZeroMemory(m_FmtMsg, 1024);

            sprintf_s
            (
                m_FmtMsg,
                1024,
                "VoodooShader::Exception in module %s, file %s at %s (line %d): %s",
                m_Module.c_str(),
                m_File,
                m_Function,
                m_Line,
                m_Message.c_str()
            );
        }

        return m_FmtMsg;
    }
}
