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

    /**
     * Voodoo internal string class, providing constant and compiler-safe string passing between various modules. 
     */
    class VOODOO_API String
    {
        class StringImpl;

    public:
        String();
        String(const wchar_t ch);
        String(const wchar_t * str);
        String(const uint32_t size, const wchar_t ch);
        String(const uint32_t size, const wchar_t * str);
        String(const String & other);
#ifdef _VECTOR_
        String(const std::vector<wchar_t> & str)
        {
            this->WInit(&str[0]);
        }
#endif
#ifdef _STRING_
        String(const std::wstring & str)
        {
            this->WInit(str.c_str());
        };
#endif

        ~String();

        // Conversion functions
        static String CharToString(const char * str) { String vstr; vstr.CInit(str); };
#ifdef _STRING_
        inline static String StdToString(const std::string & str) { return String::CharToString(str.c_str()); };
#endif

        // Modify
        String & Append(const wchar_t ch);
        String & Append(const uint32_t size, const wchar_t ch);
        String & Append(const wchar_t * str);
        String & Append(const uint32_t size, const wchar_t * str);
        String & Append(const String & str);
        String & Assign(const wchar_t ch);
        String & Assign(const uint32_t size, const wchar_t ch);
        String & Assign(const wchar_t * str);
        String & Assign(const uint32_t size, const wchar_t * str);
        String & Assign(const String & str);
        String & Clear();
        String & Prepend(const wchar_t ch);
        String & Prepend(const uint32_t size, const wchar_t ch);
        String & Prepend(const wchar_t * str);
        String & Prepend(const uint32_t size, const wchar_t * str);
        String & Prepend(const String & str);
        String & Truncate(uint32_t size);

        // Copy
        String ToLower() const;
        String ToUpper() const;

        // Part
        String Left(uint32_t count) const;
        String Right(uint32_t count) const;
        String Substr(uint32_t start, uint32_t count = String::Npos) const;

        // Predicates
        bool Compare(const wchar_t ch, bool useCase = true) const;
        bool Compare(const wchar_t * str, bool useCase = true) const;
        bool Compare(const String & str, bool useCase = true) const;
        bool StartsWith(const wchar_t ch, bool useCase = true) const;
        bool StartsWith(const wchar_t * str, bool useCase = true) const;
        bool StartsWith(const String & str, bool useCase = true) const;
        bool EndsWith(const wchar_t ch, bool useCase = true) const;
        bool EndsWith(const wchar_t * str, bool useCase = true) const;
        bool EndsWith(const String & str, bool useCase = true) const;
        bool Contains(const wchar_t ch, bool useCase = true) const;
        bool Contains(const wchar_t * str, bool useCase = true) const;
        bool Contains(const String & str, bool useCase = true) const;

        // Find
        uint32_t Find(const wchar_t ch, bool useCase = true) const;
        uint32_t Find(const wchar_t * str, bool useCase = true) const;
        uint32_t Find(const String & str, bool useCase = true) const;
        uint32_t ReverseFind(const wchar_t ch, bool useCase = true) const;
        uint32_t ReverseFind(const wchar_t * str, bool useCase = true) const;
        uint32_t ReverseFind(const String & str, bool useCase = true) const;

        // Replace
        String & Replace(const wchar_t fch, const wchar_t rch, bool useCase = true);
        String & Replace(const wchar_t * fstr, const wchar_t * rstr, bool useCase = true);
        String & Replace(const String & fstr, const String & rstr, bool caseSensitive = true);
        String & Remove(const wchar_t fch, bool useCase = true);
        String & Remove(const wchar_t * fstr, bool useCase = true);
        String & Remove(const String & fstr, bool useCase = true);

        // Format
        static String FormatV(const wchar_t * fmt, va_list args);
        inline static String FormatV(const String & fmt, va_list args) 
        { 
            return String::FormatV(fmt.GetData(), args); 
        };
        inline static String Format(_In_ _Printf_format_string_ const wchar_t * fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            String str = String::FormatV(fmt, args);
            va_end(args);
            return str;
        };
        inline static String Format(_Printf_format_string_ const String & fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            String str = String::FormatV(fmt.GetData(), args);
            va_end(args);
            return str;
        };
        inline String & AppendFormat(_Printf_format_string_ const wchar_t * fmt, ...) 
        { 
            va_list args; 
            va_start(args, fmt); 
            this->Append(String::FormatV(fmt, args)); 
            va_end(args); 
            return (*this); 
        };
        inline String & AppendFormat(_Printf_format_string_ const String & fmt, ...)
        { 
            va_list args; 
            va_start(args, fmt); 
            this->Append(String::FormatV(fmt.GetData(), args)); 
            va_end(args);
            return (*this); 
        };

        // Buffer info/access
        uint32_t GetLength() const;
        bool IsEmpty() const;

        wchar_t GetAt(uint32_t pos) const;
        wchar_t & operator[](uint32_t pos);
        void SetAt(uint32_t pos, wchar_t data);

        const wchar_t * GetData() const;
#ifdef _STRING_
        inline std::wstring ToStdString() const { return std::wstring(this->GetData()); };
#endif

        inline String & operator=(const wchar_t ch) { this->Assign(ch); return (*this); };
        inline String & operator=(const wchar_t * str) { this->Assign(str); return (*this); };
        inline String & operator=(const String & str) { this->Assign(str); return (*this); };
        inline String & operator+=(const wchar_t ch) { this->Append(ch); return (*this); };
        inline String & operator+=(const wchar_t * str) { this->Append(str); return (*this); };
        inline String & operator+=(const String & str) { this->Append(str); return (*this); };
        inline const String operator+(const wchar_t ch) const { String c = (*this); c += ch; return c; };
        inline const String operator+(const wchar_t * str) const { String c = (*this); c += str; return c; };
        inline const String operator+(const String & str) const { String c = (*this); c += str; return c; };

        inline bool operator==(const wchar_t * str) const { return this->Compare(str); };
        inline bool operator==(const String & str) const { return this->Compare(str.GetData()); };
        inline bool operator!=(const wchar_t * str) const { return !(this->operator==(str)); };
        inline bool operator!=(const String & str) const { return !(this->operator==(str.GetData())); };

        bool operator<(const wchar_t * str) const;
        bool operator<(const String & str) const;
        bool operator>(const wchar_t * str) const;
        bool operator>(const String & str) const;
        inline bool operator<=(const wchar_t * str) const { return !(this->operator>(str)); };
        inline bool operator<=(const String & str) const { return !(this->operator>(str.GetData())); };
        inline bool operator>=(const wchar_t * str) const { return !(this->operator<(str)); };
        inline bool operator>=(const String & str) const { return !(this->operator<(str.GetData())); };

#ifdef _OSTREAM_
        friend inline std::wostream & operator<<(std::wostream & stream, String & str)
        {
            return (stream << str.GetData());
        }
#endif

        static const uint32_t Npos = (uint32_t)-1;

    private:
        void CInit(const char * str);
        void WInit(const wchar_t * str);

        StringImpl * m_Impl;
    };
    /**
     * @}
     */
}
