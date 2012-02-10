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

#include "VSLogger.hpp"
// System
#pragma warning(push,3)
#   include <sstream>
#   include <ios>
#pragma warning(pop)

namespace VoodooShader
{
    #define VOODOO_DEBUG_TYPE VSLogger
    DeclareDebugCache();

    _Check_return_ VOODOO_FUNCTION(ILogger *, CreateLogger)()
    {
        static LONG lock = 0;
        static VSLogger * pLogger = nullptr;

        if (InterlockedCompareExchange(&lock, 0, 1))
        {            try
            {
                pLogger = new VSLogger();
                lock = 0;
            }
            catch (const std::exception & exc)
            {
                UNREFERENCED_PARAMETER(exc);
                pLogger = nullptr;
            }
        }

        if (pLogger)
        {
            pLogger->AddRef();
        }

        return pLogger;
    }

    VSLogger::VSLogger() :
        m_Refs(0), m_Filter(VSLog_Default), m_Flags(VSLogFlag_Unknown)
    { 
        AddThisToDebugCache();
    }

    VSLogger::~VSLogger()
    { 
        RemoveThisFromDebugCache();
    }

    uint32_t VOODOO_METHODTYPE VSLogger::AddRef() CONST
    {
        return SAFE_INCREMENT(m_Refs);
    }

    uint32_t VOODOO_METHODTYPE VSLogger::Release() CONST
    {
        if (SAFE_DECREMENT(m_Refs) == 0)
        {
            delete this;
            return 0;
        }
        else
        {
            return m_Refs;
        }
    }

    VoodooResult VOODOO_METHODTYPE VSLogger::QueryInterface(_In_ Uuid refid, _Deref_out_opt_ IObject ** ppOut)
    {
        if (!ppOut)
        {
            return VSFERR_INVALIDPARAMS;
        }
        else
        {
            if (refid == IID_IObject)
            {
                *ppOut = static_cast<IObject*>(this);
            }
            else if (refid == IID_ILogger)
            {
                *ppOut = static_cast<ILogger*>(this);
            }
            else if (refid == CLSID_VSLogger)
            {
                *ppOut = static_cast<VSLogger*>(this);
            }
            else
            {
                *ppOut = nullptr;
                return VSFERR_INVALIDUUID;
            }

            (*ppOut)->AddRef();
            return VSF_OK;
        }
    }

    String VOODOO_METHODTYPE VSLogger::ToString() CONST
    {
        return VSTR("VSLogger()");
    }

    ICore * VOODOO_METHODTYPE VSLogger::GetCore() CONST
    {
        return nullptr;
    }

    VoodooResult VOODOO_METHODTYPE VSLogger::Open(_In_ const String & filename, _In_ const bool append)
    {
        if (this->m_LogFile.is_open())
        {
            this->Close();
        }

        unsigned int flags = std::ios_base::out;

        if (append)
        {
            flags |= std::ios_base::app;
        }
        else
        {
            flags |= std::ios_base::trunc;
        }

        ParserRef parser = CreateParser();
        if (parser)
        {
            this->m_LogFile.open(parser->Parse(filename).GetData(), flags);
        }
        else
        {
            this->m_LogFile.open(filename.GetData(), flags);
        }

        if (this->m_LogFile.is_open())
        {
#ifdef _DEBUG
            this->SetFlags(VSLogFlag_Flush);
#endif

            std::wstringstream logMsg;

            logMsg << VSTR("Voodoo Shader Log") << std::endl;
            logMsg << VSTR("Log opened on ") << String::Date() << VSTR(" at ") << String::Time() << VSTR(" (") << 
                String::Ticks() << VSTR(")") << std::endl;

#ifdef _DEBUG
            std::wcout << logMsg.str();
#endif
            m_LogFile << logMsg.str();

            return VSF_OK;
        }
        else
        {
            return VSFERR_INVALIDCALL;
        }
    }

    VoodooResult VOODOO_METHODTYPE VSLogger::Open(IFile * CONST pFile, CONST bool append)
    {
        if (!pFile) return VSFERR_INVALIDPARAMS;

        return this->Open(pFile->GetPath(), append);
    }

    bool VOODOO_METHODTYPE VSLogger::IsOpen() CONST 
    {
        return m_LogFile.is_open();
    }

    VoodooResult VOODOO_METHODTYPE VSLogger::Close()
    {
        if (this->IsOpen())
        {
            this->m_LogFile.close();
            return VSF_OK;
        }
        else
        {
            return VSFERR_INVALIDCALL;
        }
    }

    void VOODOO_METHODTYPE VSLogger::Flush()
    {
        if (this->IsOpen())
        {
            this->m_LogFile.flush();
        }
    }

    void VOODOO_METHODTYPE VSLogger::SetFilter(CONST uint32_t level)
    {
        m_Filter = (LogLevel)level;
    }

    LogLevel VOODOO_METHODTYPE VSLogger::GetFilter() CONST
    {
        return m_Filter;
    }

    void VOODOO_METHODTYPE VSLogger::SetFlags(CONST LogFlags flags)
    {
        m_Flags = flags;
    }

    LogFlags VOODOO_METHODTYPE VSLogger::GetFlags() CONST
    {
        return m_Flags;
    }

    VoodooResult VOODOO_METHODTYPE VSLogger::LogMessage
    (
        _In_ const uint32_t level,
        _In_ const String & source,
        _In_ const String & msg
    )
    {
        if (!this->IsOpen()) return false;

        uint32_t reqMask = VSLog_Critical | VSLog_Error;
        if ((level & (reqMask | m_Filter)) == 0) return false;

        try
        {
            // Format the message in memory to prevent partial messages from being dumped
            std::wstringstream logMsg;
            logMsg << std::hex << std::showbase << level << std::dec << std::noshowbase << VSTR(", ") << GetTickCount() << 
                VSTR(", ") << source << VSTR(", ") << msg << std::endl;

#ifdef _DEBUG
            if (level & (VSLog_ModWarning | VSLog_ModError))
            {
                OutputDebugString(logMsg.str().c_str());
#   ifdef VOODOO_DEBUG_CONSOLE
                cout << logMsg.str();
#   endif
                VOODOO_DEBUG_BREAK;
            }
#endif
            m_LogFile << logMsg.str();

#ifndef _DEBUG
            if (m_Flags & VSLogFlag_Flush)
#endif
            {
                m_LogFile << std::flush;
            }

            return true;
        }
        catch (const std::exception & exc)
        {
#ifdef _DEBUG
            OutputDebugStringA(exc.what());
#else
            UNREFERENCED_PARAMETER(exc);
#endif
            return false;
        }
    }
}
