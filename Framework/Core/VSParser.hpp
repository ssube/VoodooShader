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

#include "IParser.hpp"

namespace VoodooShader
{
    class VSParser : 
        public IParser
    {
    public:
        VSParser(_In_ ICore * const pCore) throw();

        virtual ~VSParser(void) throw();
        
        virtual uint32_t AddRef(void) const throw();
        virtual uint32_t Release(void) const throw();
        virtual String ToString(void) const throw();
        virtual ICore * GetCore(void) const throw();

        virtual void Add(_In_ const String & name, _In_ const String & value, _In_ const VariableType type = VT_Normal) throw();
        virtual void Remove(_In_ const String & name) throw();
        virtual String Parse(_In_ const String & input, _In_ const ParseFlags flags = PF_None) const throw();

        static const uint32_t VarMaxDepth    = 8;
        static const wchar_t VarDelimPre    = L'$';
        static const wchar_t VarDelimStart  = L'(';
        static const wchar_t VarDelimEnd    = L')';
        static const wchar_t VarMarkerState = L':';
        static const wchar_t VarMarkerRaw   = L'!';
        static const wchar_t VarMarkerSupp  = L'?';

    private:
        String ParseStringRaw(_In_ String input, _In_ ParseFlags flags, _In_ uint32_t depth, _In_ Dictionary & state) const;

        mutable uint32_t m_Refs;
        ICore * m_Core;

        Dictionary m_Variables;
        Dictionary m_SysVariables;
    };
}
