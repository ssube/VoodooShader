/**
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

#include "Includes.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup VoodooUtility
     * @{
     */

    /**
     * Voodoo internal string class, providing constant and compiler-safe string passing between various modules.
     *
     * @warning This @b must use the MSVC wchar_t type, or bad things will happen. <b>GCC does not properly handle
     *      Unicode strings and will not provide valid inputs for these functions.</b>
     */
    class VOODOO_API String
    {
        class StringImpl;

    public:
        /**
         * Creates an empty string.
         */
        String();

        /**
         * Creates a string from the given character (one character, converted).
         * @param ch Character to use.
         */
        String(_In_ const char ch);

        /**
         * Creates a string from a C-style string (converted).
         * @param str String to use.
         */
        String(_In_z_ const char * str);

        /**
         * Creates a string from the given character (converted and repeated).
         * @param size The number of characters.
         * @param ch The character to use.
         */
        String(_In_ const uint32_t size, _In_ const char ch);

        /**
         * Creates a string from a portion of the given string (taken from the beginning and converted).
         * @param size The number of characters to use.
         * @param str The string to use.
         */
        String(_In_ const uint32_t size, _In_z_count_(size) const char * str);

        /**
         * Creates a string from the given character (one character).
         * @param ch Character to use.
         */
        String(_In_ const wchar_t ch);

        /**
         * Creates a string from a C-style wide string.
         * @param str String to use.
         */
        String(_In_z_ const wchar_t * str);

        /**
         * Creates a string from the given character (repeated).
         * @param size The number of characters.
         * @param ch The character to use.
         */
        String(_In_ const uint32_t size, _In_ const wchar_t ch);

        /**
         * Create a string from a C-style wide string.
         * @param size The number of characters to use.
         * @param str The string to use.
         */
        String(_In_ const uint32_t size, _In_z_count_(size) const wchar_t * str);

        /**
         * Creates a string from another String (copy constructor).
         * @param str The string to copy.
         * @note This does not provide a partial copy constructor, use <code>String(String.Left())</code>.
         */
        String(_In_ const String & str);

        /**
         * Creates a string from a Uuid, converting to a string. This takes the unbraced 8/2/2/2/12 format, like so:
         * @li @code hhhhhhhh-hhhh-hhhh-hhhh-hhhhhhhhhhhh @endcode
         */
        String(_In_ const Uuid & uuid);

#ifdef _VECTOR_
        /**
         * Create a string from a vector, treating it as a character array (performs conversion).
         * @param vec The vector to convert and use.
         */
        String(_In_ const std::vector<char> & vec)
        {
            this->CInit(0, &vec[0]);
        }

        /**
         * Create a string from a vector, treating it as a wide character array.
         * @param vec The vector to use.
         */
        String(_In_ const std::vector<wchar_t> & vec)
        {
            this->WInit(0, &vec[0]);
        }
#endif

#ifdef _STRING_
        /**
         * Create a string from a std::string (performs conversion).
         * @param str The string to use.
         */
        String(_In_ const std::string & str)
        {
            this->CInit(0, str.c_str());
        }
        
        /**
         * Create a string from a std::wstring.
         * @param str The string to use.
         */
        String(_In_ const std::wstring & str)
        {
            this->WInit(0, str.c_str());
        };
#endif

        ~String();

        /**
         * Attempts to convert this String to a Uuid. The string must be of one of the following forms:
         * @li <code>{01234567-89ab-cdef-0123-456789abcdef}</code>
         * @li <code>01234567-89ab-cdef-0123-456789abcdef</code>
         * @li <code>0123456789abcdef0123456789abcdef</code>
         *
         * The first format is preferred, and matches the COM-standard 4/2/2/2/12 format, supported by most tools and
         * common interfaces. Wherever possible, UUIDs should be embedded in code using the appropriate Voodoo macros to
         * avoid string conversion costs.
         *
         * @param pUuid Pointer to the Uuid to be filled, if conversion succeeds. If conversion fails, this is not modified.
         * @return Success of conversion.
         */
        bool ToUuid(_Out_ Uuid * pUuid) const;

        /**
         * Attempts to convert this String to a character array. If size is 0 and pBuffer is null, this estimates the size
         * of the buffer required and returns that; otherwise it converts the string and returns the number of bytes written
         * to pBuffer.
         * 
         * @param size Size of buffer.
         * @param pBuffer Buffer to write converted string to.
         * @return Necessary buffer size or bytes converted.
         */
        int32_t ToCharStr(_In_ int32_t size, _Inout_opt_count_(size) char * const pBuffer) const;

#ifdef _STRING_
        /**
         * Creates a std::wstring from this string.
         * 
         * @return Copied string.
         */
        std::wstring ToString() const
        { 
            return std::wstring(this->GetData()); 
        };
#endif

        /* Modify functions */
        String & Append(_In_ const wchar_t ch);
        String & Append(_In_ const uint32_t size, _In_ const wchar_t ch);
        String & Append(_In_ const wchar_t * str);
        String & Append(_In_ const uint32_t size, _In_z_count_(size) const wchar_t * str);
        String & Append(_In_ const String & str);
        String & Assign(_In_ const wchar_t ch);
        String & Assign(_In_ const uint32_t size, _In_ const wchar_t ch);
        String & Assign(_In_ const wchar_t * str);
        String & Assign(_In_ const uint32_t size, _In_z_count_(size) const wchar_t * str);
        String & Assign(_In_ const String & str);
        String & Clear();
        String & Prepend(_In_ const wchar_t ch);
        String & Prepend(_In_ const uint32_t size, _In_ const wchar_t ch);
        String & Prepend(_In_z_ const wchar_t * str);
        String & Prepend(_In_ const uint32_t size, _In_z_count_(size) const wchar_t * str);
        String & Prepend(_In_ const String & str);
        String & Truncate(_In_ uint32_t size);

        // Copy
        String ToLower() const;
        String ToUpper() const;

        // Part
        String Left(_In_ uint32_t count) const;
        String Right(_In_ uint32_t count) const;
        String Substr(_In_ uint32_t start, _In_ uint32_t count = String::Npos) const;

        // Predicates
        bool Compare(_In_ const wchar_t ch, _In_ bool useCase = true) const;
        bool Compare(_In_z_ const wchar_t * str, _In_ bool useCase = true) const;
        bool Compare(_In_ const String & str, _In_ bool useCase = true) const;
        bool StartsWith(_In_ const wchar_t ch, _In_ bool useCase = true) const;
        bool StartsWith(_In_z_ const wchar_t * str, _In_ bool useCase = true) const;
        bool StartsWith(_In_ const String & str, _In_ bool useCase = true) const;
        bool EndsWith(_In_ const wchar_t ch, _In_ bool useCase = true) const;
        bool EndsWith(_In_z_ const wchar_t * str, _In_ bool useCase = true) const;
        bool EndsWith(_In_ const String & str, _In_ bool useCase = true) const;
        bool Contains(_In_ const wchar_t ch, _In_ bool useCase = true) const;
        bool Contains(_In_z_ const wchar_t * str, _In_ bool useCase = true) const;
        bool Contains(_In_ const String & str, _In_ bool useCase = true) const;

        // Find
        uint32_t Find(_In_ const wchar_t ch, _In_ bool useCase = true) const;
        uint32_t Find(_In_z_ const wchar_t * str, _In_ bool useCase = true) const;
        uint32_t Find(_In_ const String & str, _In_ bool useCase = true) const;
        uint32_t ReverseFind(_In_ const wchar_t ch, _In_ bool useCase = true) const;
        uint32_t ReverseFind(_In_z_ const wchar_t * str, _In_ bool useCase = true) const;
        uint32_t ReverseFind(_In_ const String & str, _In_ bool useCase = true) const;

        // Replace
        String & Replace(_In_ const wchar_t fch, _In_ const wchar_t rch, _In_ bool useCase = true);
        String & Replace(_In_z_ const wchar_t * fstr, _In_z_ const wchar_t * rstr, _In_ bool useCase = true);
        String & Replace(_In_ const String & fstr, _In_ const String & rstr, _In_ bool caseSensitive = true);
        String & Remove(_In_ const wchar_t fch, _In_ bool useCase = true);
        String & Remove(_In_z_ const wchar_t * fstr, _In_ bool useCase = true);
        String & Remove(_In_ const String & fstr, _In_ bool useCase = true);

        // Format
        static String FormatV(_In_z_ _Printf_format_string_ const wchar_t * fmt, _In_opt_ va_list args);
        inline static String FormatV(_In_ _Printf_format_string_ const String & fmt, va_list args)
        {
            return String::FormatV(fmt.GetData(), args);
        };

        inline static String Format(_In_z_ _Printf_format_string_ const wchar_t * fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            String str = String::FormatV(fmt, args);
            va_end(args);
            return str;
        };

        inline static String Format(_In_ _Printf_format_string_ const String & fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            String str = String::FormatV(fmt.GetData(), args);
            va_end(args);
            return str;
        };

        inline String & AppendFormat(_In_z_ _Printf_format_string_ const wchar_t * fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            this->Append(String::FormatV(fmt, args));
            va_end(args);
            return (*this);
        };

        inline String & AppendFormat(_In_ _Printf_format_string_ const String & fmt, ...)
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

        wchar_t GetAt(_In_ uint32_t pos) const;
        wchar_t & operator[](_In_ uint32_t pos);
        void SetAt(_In_ uint32_t pos, _In_ wchar_t data);

        const wchar_t * GetData() const;
#ifdef _STRING_
        inline std::wstring ToStdString() const { return std::wstring(this->GetData()); };
#endif

        inline String & operator=(_In_ const wchar_t ch) { this->Assign(ch); return (*this); };
        inline String & operator=(_In_z_ const wchar_t * str) { this->Assign(str); return (*this); };
        inline String & operator=(_In_ const String & str) { this->Assign(str); return (*this); };
        inline String & operator+=(_In_ const wchar_t ch) { this->Append(ch); return (*this); };
        inline String & operator+=(_In_z_ const wchar_t * str) { this->Append(str); return (*this); };
        inline String & operator+=(_In_ const String & str) { this->Append(str); return (*this); };
        inline const String operator+(_In_ const wchar_t ch) const { String c = (*this); c += ch; return c; };
        inline const String operator+(_In_z_ const wchar_t * str) const { String c = (*this); c += str; return c; };
        inline const String operator+(_In_ const String & str) const { String c = (*this); c += str; return c; };

        inline bool operator==(_In_z_ const wchar_t * str) const { return this->Compare(str); };
        inline bool operator==(_In_ const String & str) const { return this->Compare(str.GetData()); };
        inline bool operator!=(_In_z_ const wchar_t * str) const { return !(this->operator==(str)); };
        inline bool operator!=(_In_ const String & str) const { return !(this->operator==(str.GetData())); };

        bool operator<(_In_z_ const wchar_t * str) const;
        bool operator<(_In_ const String & str) const;
        bool operator>(_In_z_ const wchar_t * str) const;
        bool operator>(_In_ const String & str) const;
        inline bool operator<=(_In_z_ const wchar_t * str) const { return !(this->operator>(str)); };
        inline bool operator<=(_In_ const String & str) const { return !(this->operator>(str.GetData())); };
        inline bool operator>=(_In_z_ const wchar_t * str) const { return !(this->operator<(str)); };
        inline bool operator>=(_In_ const String & str) const { return !(this->operator<(str.GetData())); };

        static const uint32_t Npos = (uint32_t)-1;

    private:
        void CInit(_In_ const uint32_t size, _In_z_count_(size) const char * str);
        void WInit(_In_ const uint32_t size, _In_z_count_(size) const wchar_t * str);

        StringImpl * m_Impl;
    };
    /**
     * @}
     */
}
