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

#include <ostream>

namespace VoodooShader
{
    /** 
     * @addtogroup VoodooUtility 
     * @{ 
     */

    /**
     * Voodoo internal string class, providing constant and compiler-safe string passing between various modules. 
     */
    class VOODOO_API String
    {
        class StringImpl;

    public:
        String();
        String(const wchar_t ch);
        String(uint32_t size, const wchar_t ch);
        String(const wchar_t * str);
        String(uint32_t size, const wchar_t * str);
        String(const String & other);
#ifdef _STRING_
        String(const std::wstring & str);
#endif

        // Modify
        void Append(const wchar_t ch);
        void Append(const uint32_t size, const wchar_t ch);
        void Append(const wchar_t * str);
        void Append(const uint32_t size, const wchar_t * str);
        void Append(const String & str);
        void Assign(const wchar_t ch);
        void Assign(const uint32_t size, const wchar_t ch);
        void Assign(const wchar_t * str);
        void Assign(const uint32_t size, const wchar_t * str);
        void Assign(const String & str);
        void Clear();
        void Prepend(const wchar_t ch);
        void Prepend(const uint32_t size, const wchar_t ch);
        void Prepend(const wchar_t * str);
        void Prepend(const String & str);
        void Truncate(uint32_t size);

        // Copy
        String ToLower() const;
        String ToUpper() const;

        // Part
        String Left(uint32_t count) const;
        String Right(uint32_t count) const;
        String Substr(uint32_t start, uint32_t count = String::Npos) const;

        // Predicates
        bool StartsWith(const wchar_t ch, bool useCase = true) const;
        bool StartsWith(const wchar_t * str, bool useCase = true) const;
        bool StartsWith(const String & str, bool useCase = true) const;
        bool EndsWith(const wchar_t ch, bool useCase = true) const;
        bool EndsWith(const wchar_t * str, bool useCase = true) const;
        bool EndsWith(const String & str, bool useCase = true) const;
        bool Contains(const wchar_t ch, bool useCase = true) const;
        bool Contains(const wchar_t * str, bool useCase = true) const;
        bool Contains(const String & str, bool useCase = true) const;
        bool Compare(const wchar_t ch, bool useCase = true) const;
        bool Compare(const wchar_t * str, bool useCase = true) const;
        bool Compare(const String & str, bool useCase = true) const;

        // Find
        uint32_t Find(const wchar_t ch, bool useCase = true) const;
        uint32_t Find(const wchar_t * str, bool useCase = true) const;
        uint32_t Find(const String & str, bool useCase = true) const;
        uint32_t ReverseFind(const wchar_t ch, bool useCase = true) const;
        uint32_t ReverseFind(const wchar_t * str, bool useCase = true) const;
        uint32_t ReverseFind(const String & str, bool useCase = true) const;

        // Replace
        void Replace(const wchar_t cfind, const wchar_t creplace, bool useCase = true);
        void Replace(const wchar_t * find, const wchar_t * replace, bool useCase = true);
        void Replace(const String & sfind, const String & srep, bool caseSensitive = true);
        void Remove(const wchar_t fch, bool useCase = true);
        void Remove(const wchar_t * find, bool useCase = true);
        void Remove(const String & find, bool useCase = true);

        // Format
        static String Format(_Printf_format_string_ const wchar_t * fmt, ...);
        static String Format(_Printf_format_string_ const  String & fmt, ...);
        static String FormatV(const wchar_t * fmt, va_list args);
        static String FormatV(const  String & fmt, va_list args);
        void AppendFormat(_Printf_format_string_ const wchar_t * fmt, ...);
        void AppendFormat(_Printf_format_string_ const  String & fmt, ...);

        // Buffer info/access
        uint32_t GetLength() const;
        bool IsEmpty() const;

        wchar_t GetAt(uint32_t pos) const;
        wchar_t & operator[](uint32_t pos);
        void SetAt(uint32_t pos, wchar_t data);

        const wchar_t * GetData() const;
        //const char * Get
#ifdef _STRING_
        std::wstring ToStdString() const;
#endif

        String & operator=(const wchar_t ch) { this->Assign(ch); return (*this); };
        String & operator=(const wchar_t * str) { this->Assign(str); return (*this); };
        String & operator=(const String & str) { this->Assign(str); return (*this); };
        const String operator+(const wchar_t ch) const;
        const String operator+(const wchar_t * str) const;
        const String operator+(const String & str) const;
        String & operator+=(const wchar_t ch) { this->Append(ch); return (*this); };
        String & operator+=(const wchar_t * str) { this->Append(str); return (*this); };
        String & operator+=(const String & str) { this->Append(str); return (*this); };

        bool operator==(const wchar_t ch) const { return this->Compare(ch); };
        bool operator==(const wchar_t * str) const { return this->Compare(str); };
        bool operator==(const String & str) const { return this->Compare(str); };
        bool operator!=(const wchar_t ch) const { return !(this->operator==(ch)); };
        bool operator!=(const wchar_t * str) const { return !(this->operator==(str)); };
        bool operator!=(const String & str) const { return !(this->operator==(str)); };

        bool operator<(const wchar_t ch) const;
        bool operator<(const wchar_t * str) const;
        bool operator<(const String & str) const;
        bool operator<=(const wchar_t ch) const;
        bool operator<=(const wchar_t * str) const;
        bool operator<=(const String & str) const;
        bool operator>(const wchar_t ch) const;
        bool operator>(const wchar_t * str) const;
        bool operator>(const String & str) const;
        bool operator>=(const wchar_t ch) const;
        bool operator>=(const wchar_t * str) const;
        bool operator>=(const String & str) const;

#ifdef _OSTREAM_
        friend std::wostream & operator<<(std::wostream & stream, String & str)
        {
            return (stream << str.GetData());
        }
#endif

        static const uint32_t Npos = (uint32_t)-1;

    private:
        void Init(const wchar_t * str);

        StringImpl * m_Impl;
    };
    /**
     * @}
     */
}
