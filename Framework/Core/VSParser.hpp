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
#pragma once

#include "VoodooInternal.hpp"

namespace VoodooShader
{
    /**
     * @clsid e6f312a2-05af-11e1-9e05-005056c00008
     */
    VOODOO_CLASS(VSParser, IParser, ({0xA2, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08}))
    {
    public:
        VSParser();

        VOODOO_METHOD_(uint32_t, AddRef)() CONST;
        VOODOO_METHOD_(uint32_t, Release)() CONST;
        VOODOO_METHOD(QueryInterface)(_In_ CONST Uuid refid, _Outptr_result_maybenull_ IObject ** ppOut);
        VOODOO_METHOD_(String, ToString)() CONST;
        VOODOO_METHOD_(ICore *, GetCore)() CONST;

        VOODOO_METHOD(Add)(_In_ CONST String & name, _In_ CONST String & value, _In_ CONST VariableType type = VSVar_Normal);
        VOODOO_METHOD(Remove)(_In_ CONST String & name);
        VOODOO_METHOD_(String, Parse)(_In_ CONST String & input, _In_ CONST ParseFlags flags = VSParse_None) CONST;

        static const uint32_t VarMaxDepth    = 8;
        static const wchar_t  VarDelimPre    = VSTR('$');
        static const wchar_t  VarDelimStart  = VSTR('(');
        static const wchar_t  VarDelimEnd    = VSTR(')');
        static const wchar_t  VarMarkerState = VSTR(':');
        static const wchar_t  VarMarkerReq   = VSTR('!');
        static const wchar_t  VarMarkerDelay = VSTR('~');
        static const wchar_t  VarMarkerMode  = VSTR('@');
        static const wchar_t  VarMarkerModeD = VSTR(' ');
        static const wchar_t  VarMarkerModeM = VSTR('+');
        static const wchar_t  VarMarkerModeR = VSTR('-');

    private:
        // Private these to prevent copying internally (external libs never will).
        VSParser(CONST VSParser & other);
        VSParser & operator=(CONST VSParser & other);
        ~VSParser();

        String ParseStringRaw(_In_ String input, _In_ ParseFlags flags, _In_ uint32_t depth, _In_ StringMap & state) CONST;

        mutable uint32_t m_Refs;

        LoggerRef m_Logger;

        VariableMap m_Variables;
    };
}
