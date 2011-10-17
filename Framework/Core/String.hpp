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

namespace VoodooShader
{
    /** 
     * @addtogroup VoodooUtility 
     * @{ 
     */
    class StringImpl;

    /**
     * Voodoo internal string class, providing constant and compiler-safe string passing between various modules. Provides
     * methods similar to MFC's CString and Qt's QString, not so much like std::string. Can be easily converted to a
     * std::string (provides a cast if the proper headers are included).
     */
    class VOODOO_API String
    {
    public:
        String()
        {
            this->Init(nullptr);
        };

        String(const char * str)
        {
            this->Init(str);
        };

        String(const String & other)
        {
            this->Init(other.GetData());
        };

#ifdef _STRING_
        String(const std::string & str)
        {
            this->Init(str.c_str());
        };
#endif

        // Modify
        void ToLower();
        void ToUpper();
        void Clear();

        // Part
        String Left(UInt32 count) const;
        String Mid(UInt32 start, UInt32 count) const;
        String Right(UInt32 count) const;

        // Predicates
        Bool StartsWith(const char * str, Bool useCase = true) const;
        Bool EndsWith(const char * str, Bool useCase = true) const;
        Bool Contains(const char * str, Bool useCase = true) const;
        Bool Compare(const char * str, Bool useCase = true) const;

        // Find
        UInt32 Find(const char * str, Bool useCase = true) const;
        UInt32 ReverseFind(const char * str, Bool useCase = true) const;

        // Replace
        void Replace(const char * find, const char * replace, Bool useCase = true);
        void Erase(const char * find, Bool useCase = true);

        // Format
        static String Format(_Printf_format_string_ const char * fmt, ...);
        static String FormatV(const char * fmt, va_list args);
        void AppendFormat(_Printf_format_string_ const char * fmt, ...);

        // Buffer info/access
        UInt32 GetLength() const;
        Bool IsEmpty() const;
        const char * GetData() const;

        char GetAt(UInt32 pos) const;
        char & operator[](UInt32 pos);
        void SetAt(UInt32 pos, char data);

        operator const char *() const;
#ifdef _STRING_
        operator std::string() const
        {
            return std::string(this->GetData());
        };
#endif

        String & operator=(const char * other);
        String operator+(const char * other);
        String & operator+=(const char * other);

        Bool operator==(const char * other) const;
        Bool operator!=(const char * other) const;
        Bool operator<(const char * other) const;

    private:
        void Init(const char * str);

        StringImpl * m_Impl;
    };
    /**
     * @}
     */
}
