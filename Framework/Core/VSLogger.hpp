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
#pragma once

#include "ILogger.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup voodoo_core_null Null Implementations
     * @ingroup voodoo_core
     *
     * @{
     */

    /**
     * Voodoo Shader null logger implementation. Does not format or log messages or access any files.
     *
     * @clsid e6f3129e-05af-11e1-9e05-005056c00008
     */
    VOODOO_CLASS(VSLogger, ILogger, {0x9E, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08})
    {
    public:
        VSLogger(ICore * pCore);
        ~VSLogger();

        VOODOO_METHOD_(uint32_t, AddRef)() CONST;
        VOODOO_METHOD_(uint32_t, Release)() CONST;
        VOODOO_METHOD(QueryInterface)(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST;
        VOODOO_METHOD_(String, ToString)() CONST;
        VOODOO_METHOD_(ICore *, GetCore)() CONST;

        VOODOO_METHOD(Open)(_In_ const String & filename, _In_ const bool append);
        VOODOO_METHOD(Open)(_In_ IFile * const pFile, _In_ const bool append);
        VOODOO_METHOD(IsOpen)() CONST;
        VOODOO_METHOD(Close)();
        VOODOO_METHOD(Flush)();
        VOODOO_METHOD_(void, SetFilter)(_In_ const uint32_t level);
        VOODOO_METHOD_(LogLevel, GetFilter)() const;
        VOODOO_METHOD_(void, SetFlags)(_In_ const LogFlags flush);
        VOODOO_METHOD_(LogFlags, GetFlags)() const;
        VOODOO_METHOD(LogMessage)(_In_ const uint32_t level, _In_ const String & source, _In_ const String & msg);

    private:
        mutable uint32_t m_Refs;
        ICore * m_Core;
    };
}
