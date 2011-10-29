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

#include <string>
#include <boost/algorithm/string.hpp>

#include "String.hpp"

namespace VoodooShader
{
    class String::StringImpl
    {
    public:
        StringImpl() : m_Str() {};
        StringImpl(const uint32_t size, const wchar_t ch) : m_Str(size, ch) {};
        StringImpl(const wchar_t * str) : m_Str(str) {};
        StringImpl(const uint32_t size, const wchar_t * str) : m_Str(str, size) {};

    public:
        std::wstring m_Str;
    };

    String::String()
    {
        m_Impl = new StringImpl();
    }

    String::String(const wchar_t ch)
    {
        m_Impl = new StringImpl(1, ch);
    }

    String::String(const wchar_t * str)
    {
        m_Impl = new StringImpl(str);
    }

    String::String(const uint32_t size, const wchar_t ch)
    {
        m_Impl = new StringImpl(size, ch);
    }

    String::String(const uint32_t size, const wchar_t * str)
    {
        m_Impl = new StringImpl(size, str);
    }

    String::~String()
    {
        delete m_Impl;
        m_Impl = nullptr;
    }

    int32_t String::ToCharStr(int32_t size, char * const pBuffer) const
    {
        return WideCharToMultiByte(CP_UTF8, NULL, m_Impl->m_Str.c_str(), -1, pBuffer, size, NULL, NULL);
    }

    void String::CInit(const char * str)
    {
        int len = MultiByteToWideChar(CP_UTF8, NULL, str, -1, NULL, 0);
        std::vector<wchar_t> wstr(len);
        MultiByteToWideChar(CP_UTF8, NULL, str, -1, &wstr[0], len);
        m_Impl = new StringImpl(&wstr[0]);
    }

    void String::WInit(const wchar_t * str)
    {
        if (m_Impl) delete m_Impl;
        m_Impl = new StringImpl(str);
    }

    String & String::Append(const wchar_t ch)
    {
        m_Impl->m_Str.append(1, ch);
        return (*this);
    }

    String & String::Append(const uint32_t size, const wchar_t ch)
    {
        m_Impl->m_Str.append(size, ch);
        return (*this);
    }

    String & String::Append(const wchar_t * str)
    {
        m_Impl->m_Str.append(str);
        return (*this);
    }

    String & String::Append(const uint32_t size, const wchar_t * str)
    {
        m_Impl->m_Str.append(str, size);
        return (*this);
    }

    String & String::Append(const String & str)
    {
        m_Impl->m_Str.append(str.m_Impl->m_Str);
        return (*this);
    }

    String & String::Assign(const wchar_t ch)
    {
        m_Impl->m_Str.assign(1, ch);
        return (*this);
    }

    String & String::Assign(const uint32_t size, const wchar_t ch)
    {
        m_Impl->m_Str.assign(size, ch);
        return (*this);
    }

    String & String::Assign(const wchar_t * str)
    {
        m_Impl->m_Str.assign(str);
        return (*this);
    }

    String & String::Assign(const uint32_t size, const wchar_t * str)
    {
        m_Impl->m_Str.assign(str, size);
        return (*this);
    }

    String & String::Assign(const String & str)
    {
        m_Impl->m_Str.assign(str.m_Impl->m_Str);
        return (*this);
    }

    String & String::Clear()
    {
        m_Impl->m_Str.clear();
        return (*this);
    }

    String & String::Prepend(const wchar_t ch)
    {
        m_Impl->m_Str = std::wstring(1, ch) + m_Impl->m_Str;
        return (*this);
    }

    String & String::Prepend(const uint32_t size, const wchar_t ch)
    {
        m_Impl->m_Str = std::wstring(size, ch) + m_Impl->m_Str;
        return (*this);
    }

    String & String::Prepend(const wchar_t * str)
    {
        m_Impl->m_Str = std::wstring(str) + m_Impl->m_Str;
        return (*this);
    }

    String & String::Prepend(const uint32_t size, const wchar_t * str)
    {
        m_Impl->m_Str = std::wstring(str, size) + m_Impl->m_Str;
        return (*this);
    }

    String & String::Prepend(const String & str)
    {
        m_Impl->m_Str = str.m_Impl->m_Str + m_Impl->m_Str;
        return (*this);
    }

    String & String::Truncate(uint32_t size)
    {
        m_Impl->m_Str = m_Impl->m_Str.substr(size);
        return (*this);
    }

    String String::ToLower() const
    {
        return String(boost::to_lower_copy(m_Impl->m_Str));
    }

    String String::ToUpper() const
    {
        return String(boost::to_upper_copy(m_Impl->m_Str));
    }

    String String::Left(uint32_t count) const
    {
        if (count == String::Npos)
        {
            return String(m_Impl->m_Str.substr(0, std::string::npos));
        } else {
            return String(m_Impl->m_Str.substr(0, count));
        }
    }

    String String::Right(uint32_t count) const
    {
        return String(m_Impl->m_Str.substr(max(this->GetLength() - count, 0), count));
    }

    String String::Substr(uint32_t start, uint32_t count) const
    {
        if (count == String::Npos)
        {
            return String(m_Impl->m_Str.substr(start, std::string::npos));
        } else {
            return String(m_Impl->m_Str.substr(start, count));
        }
    }

    bool String::Compare(const wchar_t ch, bool useCase) const
    {
        if (useCase)
        {
            return boost::equals(m_Impl->m_Str, std::wstring(1, ch));
        } else {
            return boost::iequals(m_Impl->m_Str, std::wstring(1, ch));
        }
    }

    bool String::Compare(const wchar_t * str, bool useCase) const
    {
        if (useCase)
        {
            return boost::equals(m_Impl->m_Str, str);
        } else {
            return boost::iequals(m_Impl->m_Str, str);
        }
    }

    bool String::Compare(const String & str, bool useCase) const
    {
        if (useCase)
        {
            return boost::equals(m_Impl->m_Str, str.m_Impl->m_Str);
        } else {
            return boost::iequals(m_Impl->m_Str, str.m_Impl->m_Str);
        }
    }

    bool String::StartsWith(const wchar_t ch, bool useCase) const
    {
        if (useCase)
        {
            return boost::starts_with(m_Impl->m_Str, std::wstring(1, ch));
        } else {
            return boost::istarts_with(m_Impl->m_Str, std::wstring(1, ch));
        }
    }

    bool String::StartsWith(const wchar_t * str, bool useCase) const
    {
        if (useCase)
        {
            return boost::starts_with(m_Impl->m_Str, str);
        } else {
            return boost::istarts_with(m_Impl->m_Str, str);
        }
    }

    bool String::StartsWith(const String & str, bool useCase) const
    {
        if (useCase)
        {
            return boost::starts_with(m_Impl->m_Str, str.m_Impl->m_Str);
        } else {
            return boost::istarts_with(m_Impl->m_Str, str.m_Impl->m_Str);
        }
    }

    bool String::EndsWith(const wchar_t ch, bool useCase) const
    {
        if (useCase)
        {
            return boost::ends_with(m_Impl->m_Str, std::wstring(1, ch));
        } else {
            return boost::iends_with(m_Impl->m_Str, std::wstring(1, ch));
        }
    }

    bool String::EndsWith(const wchar_t * str, bool useCase) const
    {
        if (useCase)
        {
            return boost::ends_with(m_Impl->m_Str, str);
        } else {
            return boost::iends_with(m_Impl->m_Str, str);
        }
    }

    bool String::EndsWith(const String & str, bool useCase) const
    {
        if (useCase)
        {
            return boost::ends_with(m_Impl->m_Str, str.m_Impl->m_Str);
        } else {
            return boost::iends_with(m_Impl->m_Str, str.m_Impl->m_Str);
        }
    }

    bool String::Contains(const wchar_t ch, bool useCase) const
    {
        if (useCase)
        {
            return boost::contains(m_Impl->m_Str, std::wstring(1, ch));
        } else {
            return boost::icontains(m_Impl->m_Str, std::wstring(1, ch));
        }
    }

    bool String::Contains(const wchar_t * str, bool useCase) const
    {
        if (useCase)
        {
            return boost::contains(m_Impl->m_Str, str);
        } else {
            return boost::icontains(m_Impl->m_Str, str);
        }
    }

    bool String::Contains(const String & str, bool useCase) const
    {
        if (useCase)
        {
            return boost::contains(m_Impl->m_Str, str.m_Impl->m_Str);
        } else {
            return boost::icontains(m_Impl->m_Str, str.m_Impl->m_Str);
        }
    }

    uint32_t String::Find(const wchar_t ch, bool useCase) const
    {
        if (useCase)
        {
            return m_Impl->m_Str.find(ch);
        } else {
            std::wstring lstr = boost::to_lower_copy(m_Impl->m_Str);
            wchar_t lch = towlower(ch);
            return lstr.find(lch);
        }
    }

    uint32_t String::Find(const wchar_t * str, bool useCase) const
    {
        if (useCase)
        {
            return m_Impl->m_Str.find(str);
        } else {
            std::wstring lstr = boost::to_lower_copy(m_Impl->m_Str);
            std::wstring lfstr(str); 
            boost::to_lower(lfstr);
            return lstr.find(lfstr);
        }
    }

    uint32_t String::Find(const String & str, bool useCase) const
    {
        if (useCase)
        {
            return m_Impl->m_Str.find(str.m_Impl->m_Str);
        } else {
            std::wstring lstr = boost::to_lower_copy(m_Impl->m_Str);
            std::wstring lfstr = boost::to_lower_copy(str.m_Impl->m_Str);
            return lstr.find(lfstr);
        }
    }

    uint32_t String::ReverseFind(const wchar_t ch, bool useCase) const
    {
        if (useCase)
        {
            return m_Impl->m_Str.find(ch);
        } else {
            std::wstring lstr = boost::to_lower_copy(m_Impl->m_Str);
            wchar_t lch = towlower(ch);
            return lstr.rfind(lch);
        }
    }

    uint32_t String::ReverseFind(const wchar_t * str, bool useCase) const
    {
        if (useCase)
        {
            return m_Impl->m_Str.find(str);
        } else {
            std::wstring lstr = boost::to_lower_copy(m_Impl->m_Str);
            std::wstring lfstr(str); 
            boost::to_lower(lfstr);
            return lstr.rfind(lfstr);
        }
    }

    uint32_t String::ReverseFind(const String & str, bool useCase) const
    {
        if (useCase)
        {
            return m_Impl->m_Str.find(str.m_Impl->m_Str);
        } else {
            std::wstring lstr = boost::to_lower_copy(m_Impl->m_Str);
            std::wstring lfstr = boost::to_lower_copy(str.m_Impl->m_Str);
            return lstr.rfind(lfstr);
        }
    }

    String & String::Replace(const wchar_t fch, const wchar_t rch, bool useCase)
    {
        if (useCase)
        {
            boost::replace_all(m_Impl->m_Str, std::wstring(1, fch), std::wstring(1, rch));
        } else {
            boost::ireplace_all(m_Impl->m_Str, std::wstring(1, fch), std::wstring(1, rch));
        }
        return (*this);
    }

    String & String::Replace(const wchar_t * fstr, const wchar_t * rstr, bool useCase)
    {
        if (useCase)
        {
            boost::replace_all(m_Impl->m_Str, fstr, rstr);
        } else {
            boost::ireplace_all(m_Impl->m_Str, fstr, rstr);
        }
        return (*this);
    }

    String & String::Replace(const String & fstr, const String & rstr, bool useCase)
    {
        if (useCase)
        {
            boost::replace_all(m_Impl->m_Str, fstr.m_Impl->m_Str, rstr.m_Impl->m_Str);
        } else {
            boost::ireplace_all(m_Impl->m_Str, fstr.m_Impl->m_Str, rstr.m_Impl->m_Str);
        }
        return (*this);
    }

    String & String::Remove(const wchar_t fch, bool useCase)
    {
        if (useCase)
        {
            boost::erase_all(m_Impl->m_Str, std::wstring(1, fch));
        } else {
            boost::ierase_all(m_Impl->m_Str, std::wstring(1, fch));
        }
        return (*this);
    }

    String & String::Remove(const wchar_t * fstr, bool useCase)
    {
        if (useCase)
        {
            boost::erase_all(m_Impl->m_Str, fstr);
        } else {
            boost::ierase_all(m_Impl->m_Str, fstr);
        }
        return (*this);
    }

    String & String::Remove(const String & fstr, bool useCase)
    {
        if (useCase)
        {
            boost::erase_all(m_Impl->m_Str, fstr.m_Impl->m_Str);
        } else {
            boost::ierase_all(m_Impl->m_Str, fstr.m_Impl->m_Str);
        }
        return (*this);
    }

    String String::FormatV(const wchar_t * fmt, va_list args)
    {
        int bufsize = _vscwprintf(fmt, args) + 1;
        std::vector<wchar_t> buffer(bufsize);

        int len = _vsnwprintf_s(&buffer[0], bufsize, bufsize-1, fmt, args);

        if (len == -1)
        {
            return String();
        } else {
            return String(buffer);
        }
    }

    uint32_t String::GetLength() const
    {
        return m_Impl->m_Str.size();
    }

    bool String::IsEmpty() const
    {
        return (this->GetLength() == 0);
    }

    wchar_t String::GetAt(uint32_t pos) const
    {
        //! @todo Error checking on pos.
        return m_Impl->m_Str.at(pos);
    }

    wchar_t & String::operator[](uint32_t pos)
    {
        //! @todo Error checking on pos.
        return m_Impl->m_Str.at(pos);
    }

    void String::SetAt(uint32_t pos, wchar_t data)
    {
        //! @todo Error checking on pos.
        m_Impl->m_Str[pos] = data;
    }

    const wchar_t * String::GetData() const
    {
        return m_Impl->m_Str.c_str();
    }

    bool String::operator<(const wchar_t * str) const
    {
        return (m_Impl->m_Str < str);
    }

    bool String::operator<(const String & str) const
    {
        return (m_Impl->m_Str < str.m_Impl->m_Str);
    }

    bool String::operator>(const wchar_t * str) const
    {
        return (m_Impl->m_Str > str);
    }

    bool String::operator>(const String & str) const
    {
        return (m_Impl->m_Str > str.m_Impl->m_Str);
    }
}
