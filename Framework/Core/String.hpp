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

#include "Includes.hpp"

#ifndef VOODOO_NO_STDSTRING
#include <string>
#endif

#ifndef VOODOO_NO_COLLECTIONS
#include <vector>
#endif

namespace VoodooShader
{
    /**
     * @addtogroup voodoo_utility Voodoo Utility
     * @{
     */

    /**
     * @addtogroup voodoo_utility_string Voodoo Strings
     * @ingroup voodoo_utility
     *
     * Voodoo internal string class, providing constant and compiler-safe string passing between various modules.
     *
     * @warning If built with Unicode, wchar_t must be a wide character meeting the size and behavior of Visual Studio's
     *      wchar_t. Otherwise, it must be a character meeting the size and behavior of the standard 8-bit ASCII char. The
     *      wchar_t type is used to allow future porting to proper C++11 UTF character types.
     */
    class VOODOO_API String
    {
        class StringImpl;

    public:
        /**
         * @name String Constructors
         * @{
         */

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
         * Creates a string from a Uuid, converting to a string with the unbraced 4/2/2/2/6 format, like so:
         *
         * @code 01234567-89ab-cdef-0123-456789abcdef @endcode
         *
         * This is considered the standard form for Voodoo Uuids, and is used in the registry and most other locations.
         */
        String(_In_ const Uuid & uuid);

#ifdef _VECTOR_
        /**
         * Create a string from a vector, treating it as a character array (performs conversion).
         * @param vec The vector to convert and use.
         */
        String(_In_ const std::vector<char> & vec) :
            m_Impl(nullptr)
        {
            this->CInit(0, &vec[0]);
        }

        /**
         * Create a string from a vector, treating it as a wide character array.
         * @param vec The vector to use.
         */
        String(_In_ const std::vector<wchar_t> & vec) :
            m_Impl(nullptr)
        {
            this->WInit(0, &vec[0]);
        }
#endif

#ifdef _STRING_
        /**
         * Create a string from a std::string (performs conversion).
         * @param str The string to use.
         */
        String(_In_ const std::string & str) :
            m_Impl(nullptr)
        {
            this->CInit(0, str.c_str());
        }

        /**
         * Create a string from a std::wstring.
         * @param str The string to use.
         */
        String(_In_ const std::wstring & str) :
            m_Impl(nullptr)
        {
            this->WInit(0, str.c_str());
        };
#endif
        /**
         * @}
         * @name Static Creation Methods
         * @{
         */
            
        /**
         * Formats a timestamp as <code>Time(HHMMSS)</code>. Leading zeros are guaranteed to be present.
         *
         * @note If the system time cannot be retrieved, an equal-length error stamp of <code>Time(000000)</code> will be
         *      returned.
         */
        static String Time(time_t * pTime = nullptr);
        /**
         * Formats a date as <code>Date(YYYYMMDD)</code>. Leading zeros are guaranteed to be present.
         *
         * @note If the system time cannot be retrieved, an equal-length error stamp of <code>Date(00000000)</code> will be
         *      returned.
         */
        static String Date(time_t * pTime = nullptr);
        /**
         * Formats the system's current tick count as <code>Ticks(n)</code>, with a varying length. This records ms since 
         * system start.
         */
        static String Ticks();
        /**
         * @}
         */

        ~String();

        /**
         * @}
         * @name String Modification
         * @{
         */
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
        /**
         * @}
         * @name 
         * @{
         */
        String ToLower() const;
        String ToUpper() const;
        /**
         * Splits the string into tokens based on the delimeters given. The remainder of the string,
         * after the maximum number of tokens is reached, will be appended to the end of the final
         * token. The delimeter characters will not be included in the split strings.
         *
         * @param delims The list of delimeter characters.
         * @param count The maximum number of tokens. Must be 0 if pStrings is null.
         * @param pStrings The destination array of strings to be filled with split tokens. If null, the number of tokens
         *      that would be split is counted and returned without any strings being copied.
         * @param stripEmpty If set, empty (0-length) tokens will be discarded.
         * @return The final number of tokens.
         */
        uint32_t Split
        (
            _In_ const String & delims, 
            _In_ const uint32_t count, 
            _In_count_(count) String * pStrings, 
            _In_ bool stripEmpty = false
        ) const;
        /**
         * Attempts to convert this String to a Uuid. The string must be of one of the following forms:
         * @li <code>{01234567-89ab-cdef-0123-456789abcdef}</code>
         * @li <code>01234567-89ab-cdef-0123-456789abcdef</code>
         * @li <code>0123456789abcdef0123456789abcdef</code>
         *
         * The second format is preferred, and matches the COM-standard 4/2/2/2/12 format, supported by most tools and
         * common interfaces. Wherever possible, Uuids should be embedded in code using the appropriate Voodoo macros to
         * avoid string conversion costs.
         *
         * @param pUuid Pointer to the Uuid to be filled, if conversion succeeds. If conversion fails, this is set to the
         *      null Uuid.
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
        /**
         * @}
         * @name String Partial Copy
         * @{
         */
        String Left(_In_ uint32_t count) const;
        String Right(_In_ uint32_t count) const;
        String Substr(_In_ uint32_t start, _In_ uint32_t count = String::Npos) const;
        /**
         * @}
         * @name String Predicates
         * @{
         */
        bool Compare(_In_ const wchar_t ch, _In_ bool useCase = true) const;
        bool Compare(_In_z_ const wchar_t * str, _In_ bool useCase = true) const;
        bool Compare(_In_ const String & str, _In_ bool useCase = true) const;
        bool Contains(_In_ const wchar_t ch, _In_ bool useCase = true) const;
        bool Contains(_In_z_ const wchar_t * str, _In_ bool useCase = true) const;
        bool Contains(_In_ const String & str, _In_ bool useCase = true) const;
        bool StartsWith(_In_ const wchar_t ch, _In_ bool useCase = true) const;
        bool StartsWith(_In_z_ const wchar_t * str, _In_ bool useCase = true) const;
        bool StartsWith(_In_ const String & str, _In_ bool useCase = true) const;
        bool EndsWith(_In_ const wchar_t ch, _In_ bool useCase = true) const;
        bool EndsWith(_In_z_ const wchar_t * str, _In_ bool useCase = true) const;
        bool EndsWith(_In_ const String & str, _In_ bool useCase = true) const;
        /**
         * @}
         * @name String Find and Replace
         * @{
         */
        uint32_t Find(_In_ const wchar_t ch, _In_ bool useCase = true) const;
        uint32_t Find(_In_z_ const wchar_t * str, _In_ bool useCase = true) const;
        uint32_t Find(_In_ const String & str, _In_ bool useCase = true) const;
        uint32_t ReverseFind(_In_ const wchar_t ch, _In_ bool useCase = true) const;
        uint32_t ReverseFind(_In_z_ const wchar_t * str, _In_ bool useCase = true) const;
        uint32_t ReverseFind(_In_ const String & str, _In_ bool useCase = true) const;

        String & Replace(_In_ const wchar_t fch, _In_ const wchar_t rch, _In_ bool useCase = true);
        String & Replace(_In_z_ const wchar_t * fstr, _In_z_ const wchar_t * rstr, _In_ bool useCase = true);
        String & Replace(_In_ const String & fstr, _In_ const String & rstr, _In_ bool caseSensitive = true);
        String & Remove(_In_ const wchar_t fch, _In_ bool useCase = true);
        String & Remove(_In_z_ const wchar_t * fstr, _In_ bool useCase = true);
        String & Remove(_In_ const String & fstr, _In_ bool useCase = true);
        /**
         * @}
         * @name Data Methods
         * Underlying data access methods.
         * @{
         */
        /**
         * Get the current length of the string, in characters.
         */
        uint32_t GetLength() const;
        /**
         * Get whether the string's length is 0 characters.
         */
        bool IsEmpty() const;
        /**
         * Reserve the given number of characters, minimizing string reallocations.
         *
         * @param size Characters to reserve.
         */
        void Reserve(uint32_t size);
        /**
         * Get a single character from the string.
         * @param pos Position to retrieve.
         */
        wchar_t GetAt(_In_ uint32_t pos) const;
        /**
         * Sets a single character in the string.
         * @param pos Position to assign.
         * @param data Character to assign.
         */
        void SetAt(_In_ uint32_t pos, _In_ wchar_t data);
        /**
         * Get a reference to a character in the string, allowing it to be changed.
         * @param pos Position to retrieve.
         */
        wchar_t & operator[](_In_ uint32_t pos);
        /**
         * Get a pointer to the data of the string, often for use as a LPWSTR.
         *
         * @warning The C-string returned may be read-only, attempting to write is undefined.
         */
        const wchar_t * GetData() const;
        /**
         * @}
         * @name Assignment and Concatenation Operators
         * @{
         */
        inline String & operator=(_In_ const wchar_t ch)
        {
            this->Assign(ch);
            return (*this);
        };

        inline String & operator=(_In_z_ const wchar_t * str)
        {
            this->Assign(str);
            return (*this);
        };

        inline String & operator=(_In_ const String & str)
        {
            this->Assign(str);
            return (*this);
        };

        inline String & operator+=(_In_ const wchar_t ch)
        {
            this->Append(ch);
            return (*this);
        };

        inline String & operator+=(_In_z_ const wchar_t * str)
        {
            this->Append(str);
            return (*this);
        };

        inline String & operator+=(_In_ const String & str)
        {
            this->Append(str);
            return (*this);
        };

        inline const String operator+(_In_ const wchar_t ch) const
        {
            String c = (*this);
            c += ch;
            return c;
        };

        inline const String operator+(_In_z_ const wchar_t * str) const
        {
            String c = (*this);
            c += str;
            return c;
        };

        inline const String operator+(_In_ const String & str) const
        {
            String c = (*this);
            c += str;
            return c;
        };
        /**
         * @}
         * @name Comparison Operators
         * @{
         */
        inline bool operator==(_In_z_ const wchar_t * str) const
        {
            return this->Compare(str);
        };

        inline bool operator==(_In_ const String & str) const
        {
            return this->Compare(str);
        };

        inline bool operator!=(_In_z_ const wchar_t * str) const
        {
            return !(this->operator==(str));
        };

        inline bool operator!=(_In_ const String & str) const
        {
            return !(this->operator==(str));
        };

        bool operator<(_In_z_ const wchar_t * str) const;
        bool operator<(_In_ const String & str) const;
        bool operator>(_In_z_ const wchar_t * str) const;
        bool operator>(_In_ const String & str) const;

        inline bool operator<=(_In_z_ const wchar_t * str) const
        {
            return !(this->operator>(str));
        };

        inline bool operator<=(_In_ const String & str) const
        {
            return !(this->operator>(str));
        };

        inline bool operator>=(_In_z_ const wchar_t * str) const
        {
            return !(this->operator<(str));
        };

        inline bool operator>=(_In_ const String & str) const
        {
            return !(this->operator<(str));
        };
        /**
         * @}
         * @
         */

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
