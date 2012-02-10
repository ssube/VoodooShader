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

#include "VoodooFramework.hpp"
// Boost
#pragma warning(push,3)
#include <boost/algorithm/string.hpp>
#pragma warning(disable: 4640)
#include <boost/uuid/string_generator.hpp>
#pragma warning(disable: 6246)
#include <boost/uuid/uuid_io.hpp>
// System
#include <string>
#include <sstream>
#include <iomanip>
#pragma warning(pop)

namespace VoodooShader
{
    class String::StringImpl
    {
    public:
        StringImpl() : m_Str() {};
        StringImpl(const uint32_t size, const wchar_t ch) : m_Str(size, ch) {};
        StringImpl(const wchar_t * str)
        {
            if (str) m_Str = (str);
        };
        StringImpl(const uint32_t size, const wchar_t * str)
        {
            if (str) m_Str = std::wstring(str, size);
        };
        StringImpl(const std::wstring & str) : m_Str(str) {};

    public:
        std::basic_string<wchar_t, std::char_traits<wchar_t>> m_Str;
    };

    String::String() :
        m_Impl(new StringImpl())
    {
    }

    String::String(const char ch) :
        m_Impl(nullptr)
    {
        wchar_t wch = 0;
        mbtowc(&wch, &ch, 1);
        m_Impl = new StringImpl(1, wch);
    }

    String::String(const char * str) :
        m_Impl(nullptr)
    {
        this->CInit(0, str);
    }

    String::String(const uint32_t size, const char ch) :
        m_Impl(nullptr)
    {
        wchar_t wch = 0;
        mbtowc(&wch, &ch, 1);
        m_Impl = new StringImpl(size, wch);
    }

    String::String(const uint32_t size, const char * str) :
        m_Impl(nullptr)
    {
        this->CInit(size, str);
    }

    String::String(const wchar_t ch) :
        m_Impl(new StringImpl(1, ch))
    {
    }

    String::String(const wchar_t * str) :
        m_Impl(new StringImpl(str))
    {
    }

    String::String(const uint32_t size, const wchar_t ch) :
        m_Impl(new StringImpl(size, ch))
    {
    }

    String::String(const uint32_t size, const wchar_t * str) :
        m_Impl(new StringImpl(size, str))
    {
    }

    String::String(const String & other) :
        m_Impl(nullptr)
    {
        if (other.m_Impl)
        {
            m_Impl = new StringImpl(other.m_Impl->m_Str);
        }
        else
        {
            m_Impl = new StringImpl();
        }
    }

    String::String(const Uuid & uuid) :
        m_Impl(new StringImpl(boost::uuids::to_wstring(uuid)))
    {
    }

    String::~String()
    {
        delete m_Impl;
        m_Impl = nullptr;
    }

    void String::CInit(const uint32_t size, const char * str)
    {
        if (m_Impl) delete m_Impl;

        if (!str)
        {
            m_Impl = new StringImpl();
        }
        else
        {
            int len = MultiByteToWideChar(CP_UTF8, NULL, str, -1, NULL, 0);
            if (len > 0)
            {
                std::vector<wchar_t> wstr((unsigned int)len);
                MultiByteToWideChar(CP_UTF8, NULL, str, -1, &wstr[0], len);

                if (size == 0)
                {
                    m_Impl = new StringImpl(&wstr[0]);
                }
                else
                {
                    m_Impl = new StringImpl(size, &wstr[0]);
                }
            }
            else
            {
                Throw(VOODOO_CORE_NAME, VSTR("Unable to convert character string."), nullptr);
            }
        }
    }

    void String::WInit(const uint32_t size, const wchar_t * str)
    {
        if (m_Impl) delete m_Impl;

        if (!str)
        {
            m_Impl = new StringImpl();
        }
        else if (size == 0)
        {
            m_Impl = new StringImpl(str);
        }
        else
        {
            m_Impl = new StringImpl(size, str);
        }
    }

    bool String::ToUuid(Uuid * pUuid) const
    {
        if (!pUuid)
        {
            return false;
        }

        try
        {
            boost::uuids::string_generator gen;
            *pUuid = gen(m_Impl->m_Str);
            return true;
        }
        catch (const boost::exception & exc)
        {
            UNREFERENCED_PARAMETER(exc);

            ZeroMemory(pUuid, sizeof(Uuid));
            return false;
        }
    }

    int32_t String::ToChars(int32_t size, char * pBuffer) const
    {
        VOODOO_CHECK_IMPL;
        return WideCharToMultiByte(CP_UTF8, NULL, m_Impl->m_Str.c_str(), -1, pBuffer, size, NULL, NULL);
    }

    String & String::Append(const wchar_t ch)
    {
        VOODOO_CHECK_IMPL;
        m_Impl->m_Str.append(1, ch);
        return (*this);
    }

    String & String::Append(const uint32_t size, const wchar_t ch)
    {
        VOODOO_CHECK_IMPL;
        m_Impl->m_Str.append(size, ch);
        return (*this);
    }

    String & String::Append(const wchar_t * str)
    {
        VOODOO_CHECK_IMPL;
        m_Impl->m_Str.append(str);
        return (*this);
    }

    String & String::Append(const uint32_t size, const wchar_t * str)
    {
        VOODOO_CHECK_IMPL;
        m_Impl->m_Str.append(str, size);
        return (*this);
    }

    String & String::Append(const String & str)
    {
        VOODOO_CHECK_IMPL;
        m_Impl->m_Str.append(str.m_Impl->m_Str);
        return (*this);
    }

    String & String::Assign(const wchar_t ch)
    {
        VOODOO_CHECK_IMPL;
        m_Impl->m_Str.assign(1, ch);
        return (*this);
    }

    String & String::Assign(const uint32_t size, const wchar_t ch)
    {
        VOODOO_CHECK_IMPL;
        m_Impl->m_Str.assign(size, ch);
        return (*this);
    }

    String & String::Assign(const wchar_t * str)
    {
        VOODOO_CHECK_IMPL;
        m_Impl->m_Str.assign(str);
        return (*this);
    }

    String & String::Assign(const uint32_t size, const wchar_t * str)
    {
        VOODOO_CHECK_IMPL;
        m_Impl->m_Str.assign(str, size);
        return (*this);
    }

    String & String::Assign(const String & str)
    {
        VOODOO_CHECK_IMPL;
        m_Impl->m_Str.assign(str.m_Impl->m_Str);
        return (*this);
    }

    String & String::Clear()
    {
        VOODOO_CHECK_IMPL;
        m_Impl->m_Str.clear();
        return (*this);
    }

    String & String::Prepend(const wchar_t ch)
    {
        VOODOO_CHECK_IMPL;
        m_Impl->m_Str = std::wstring(1, ch) + m_Impl->m_Str;
        return (*this);
    }

    String & String::Prepend(const uint32_t size, const wchar_t ch)
    {
        VOODOO_CHECK_IMPL;
        m_Impl->m_Str = std::wstring(size, ch) + m_Impl->m_Str;
        return (*this);
    }

    String & String::Prepend(const wchar_t * str)
    {
        VOODOO_CHECK_IMPL;
        m_Impl->m_Str = std::wstring(str) + m_Impl->m_Str;
        return (*this);
    }

    String & String::Prepend(const uint32_t size, const wchar_t * str)
    {
        VOODOO_CHECK_IMPL;
        m_Impl->m_Str = std::wstring(str, size) + m_Impl->m_Str;
        return (*this);
    }

    String & String::Prepend(const String & str)
    {
        VOODOO_CHECK_IMPL;
        if (str.m_Impl)
        {
            m_Impl->m_Str = str.m_Impl->m_Str + m_Impl->m_Str;
        }
        return (*this);
    }

    String & String::Truncate(uint32_t size)
    {
        VOODOO_CHECK_IMPL;
        m_Impl->m_Str = m_Impl->m_Str.substr(size);
        return (*this);
    }

    void String::Reserve(uint32_t size)
    {
        VOODOO_CHECK_IMPL;
        m_Impl->m_Str.reserve(size);
    }

    uint32_t String::Split(const String & delims, const uint32_t count, String * pStrings, bool stripEmpty) const
    {
        VOODOO_CHECK_IMPL;

        std::vector<std::wstring> tokens;
        boost::split(tokens, m_Impl->m_Str, boost::is_any_of(delims.GetData()), stripEmpty ? boost::algorithm::token_compress_on : boost::algorithm::token_compress_off);

        if (pStrings)
        {
            uint32_t index = 0;
            uint32_t cap = min(count, tokens.size());
            while (index < cap)
            {
                pStrings[index] = tokens[index];
                ++index;
            }

            if (index == tokens.size())
            {
                return index;
            }
            else if (index == count)
            {
                cap = tokens.size();
                while (index < cap)
                {
                    pStrings[count - 1] += tokens[index];
                    ++index;
                }
            }
                
            return index;
        }
        else
        {
            return tokens.size();
        }
    }

    String String::ToLower() const
    {
        VOODOO_CHECK_IMPL;
        return String(boost::to_lower_copy(m_Impl->m_Str));
    }

    String String::ToUpper() const
    {
        VOODOO_CHECK_IMPL;
        return String(boost::to_upper_copy(m_Impl->m_Str));
    }

    String String::Left(uint32_t count) const
    {
        VOODOO_CHECK_IMPL;
        if (count == String::Npos)
        {
            return String(m_Impl->m_Str.substr(0, std::string::npos));
        }
        else
        {
            return String(m_Impl->m_Str.substr(0, count));
        }
    }

    String String::Right(uint32_t count) const
    {
        VOODOO_CHECK_IMPL;
        return String(m_Impl->m_Str.substr(max(this->GetLength() - count, 0), count));
    }

    String String::Substr(uint32_t start, uint32_t count) const
    {
        VOODOO_CHECK_IMPL;
        if (count == String::Npos)
        {
            return String(m_Impl->m_Str.substr(start, std::string::npos));
        }
        else
        {
            return String(m_Impl->m_Str.substr(start, count));
        }
    }

    bool String::Compare(const wchar_t ch, bool useCase) const
    {
        VOODOO_CHECK_IMPL;
        if (useCase)
        {
            return boost::equals(m_Impl->m_Str, std::wstring(1, ch));
        }
        else
        {
            return boost::iequals(m_Impl->m_Str, std::wstring(1, ch));
        }
    }

    bool String::Compare(const wchar_t * str, bool useCase) const
    {
        VOODOO_CHECK_IMPL;
        if (useCase)
        {
            return boost::equals(m_Impl->m_Str, str);
        }
        else
        {
            return boost::iequals(m_Impl->m_Str, str);
        }
    }

    bool String::Compare(const String & str, bool useCase) const
    {
        VOODOO_CHECK_IMPL;
        if (useCase)
        {
            return boost::equals(m_Impl->m_Str, str.m_Impl->m_Str);
        }
        else
        {
            return boost::iequals(m_Impl->m_Str, str.m_Impl->m_Str);
        }
    }

    bool String::StartsWith(const wchar_t ch, bool useCase) const
    {
        VOODOO_CHECK_IMPL;
        if (useCase)
        {
            return boost::starts_with(m_Impl->m_Str, std::wstring(1, ch));
        }
        else
        {
            return boost::istarts_with(m_Impl->m_Str, std::wstring(1, ch));
        }
    }

    bool String::StartsWith(const wchar_t * str, bool useCase) const
    {
        VOODOO_CHECK_IMPL;
        if (useCase)
        {
            return boost::starts_with(m_Impl->m_Str, str);
        }
        else
        {
            return boost::istarts_with(m_Impl->m_Str, str);
        }
    }

    bool String::StartsWith(const String & str, bool useCase) const
    {
        VOODOO_CHECK_IMPL;
        if (useCase)
        {
            return boost::starts_with(m_Impl->m_Str, str.m_Impl->m_Str);
        }
        else
        {
            return boost::istarts_with(m_Impl->m_Str, str.m_Impl->m_Str);
        }
    }

    bool String::EndsWith(const wchar_t ch, bool useCase) const
    {
        VOODOO_CHECK_IMPL;
        if (useCase)
        {
            return boost::ends_with(m_Impl->m_Str, std::wstring(1, ch));
        }
        else
        {
            return boost::iends_with(m_Impl->m_Str, std::wstring(1, ch));
        }
    }

    bool String::EndsWith(const wchar_t * str, bool useCase) const
    {
        VOODOO_CHECK_IMPL;
        if (useCase)
        {
            return boost::ends_with(m_Impl->m_Str, str);
        }
        else
        {
            return boost::iends_with(m_Impl->m_Str, str);
        }
    }

    bool String::EndsWith(const String & str, bool useCase) const
    {
        VOODOO_CHECK_IMPL;
        if (useCase)
        {
            return boost::ends_with(m_Impl->m_Str, str.m_Impl->m_Str);
        }
        else
        {
            return boost::iends_with(m_Impl->m_Str, str.m_Impl->m_Str);
        }
    }

    bool String::Contains(const wchar_t ch, bool useCase) const
    {
        VOODOO_CHECK_IMPL;
        if (useCase)
        {
            return boost::contains(m_Impl->m_Str, std::wstring(1, ch));
        }
        else
        {
            return boost::icontains(m_Impl->m_Str, std::wstring(1, ch));
        }
    }

    bool String::Contains(const wchar_t * str, bool useCase) const
    {
        VOODOO_CHECK_IMPL;
        if (useCase)
        {
            return boost::contains(m_Impl->m_Str, str);
        }
        else
        {
            return boost::icontains(m_Impl->m_Str, str);
        }
    }

    bool String::Contains(const String & str, bool useCase) const
    {
        VOODOO_CHECK_IMPL;
        if (useCase)
        {
            return boost::contains(m_Impl->m_Str, str.m_Impl->m_Str);
        }
        else
        {
            return boost::icontains(m_Impl->m_Str, str.m_Impl->m_Str);
        }
    }

    uint32_t String::Find(const wchar_t ch, bool useCase) const
    {
        VOODOO_CHECK_IMPL;
        if (useCase)
        {
            return m_Impl->m_Str.find(ch);
        }
        else
        {
            std::wstring lstr = boost::to_lower_copy(m_Impl->m_Str);
            wchar_t lch = towlower(ch);
            return lstr.find(lch);
        }
    }

    uint32_t String::Find(const wchar_t * str, bool useCase) const
    {
        VOODOO_CHECK_IMPL;
        if (useCase)
        {
            return m_Impl->m_Str.find(str);
        }
        else
        {
            std::wstring lstr = boost::to_lower_copy(m_Impl->m_Str);
            std::wstring lfstr(str);
            boost::to_lower(lfstr);
            return lstr.find(lfstr);
        }
    }

    uint32_t String::Find(const String & str, bool useCase) const
    {
        VOODOO_CHECK_IMPL;
        if (useCase)
        {
            return m_Impl->m_Str.find(str.m_Impl->m_Str);
        }
        else
        {
            std::wstring lstr = boost::to_lower_copy(m_Impl->m_Str);
            std::wstring lfstr = boost::to_lower_copy(str.m_Impl->m_Str);
            return lstr.find(lfstr);
        }
    }

    uint32_t String::ReverseFind(const wchar_t ch, bool useCase) const
    {
        VOODOO_CHECK_IMPL;
        if (useCase)
        {
            return m_Impl->m_Str.find(ch);
        }
        else
        {
            std::wstring lstr = boost::to_lower_copy(m_Impl->m_Str);
            wchar_t lch = towlower(ch);
            return lstr.rfind(lch);
        }
    }

    uint32_t String::ReverseFind(const wchar_t * str, bool useCase) const
    {
        VOODOO_CHECK_IMPL;
        if (useCase)
        {
            return m_Impl->m_Str.find(str);
        }
        else
        {
            std::wstring lstr = boost::to_lower_copy(m_Impl->m_Str);
            std::wstring lfstr(str);
            boost::to_lower(lfstr);
            return lstr.rfind(lfstr);
        }
    }

    uint32_t String::ReverseFind(const String & str, bool useCase) const
    {
        VOODOO_CHECK_IMPL;
        if (useCase)
        {
            return m_Impl->m_Str.find(str.m_Impl->m_Str);
        }
        else
        {
            std::wstring lstr = boost::to_lower_copy(m_Impl->m_Str);
            std::wstring lfstr = boost::to_lower_copy(str.m_Impl->m_Str);
            return lstr.rfind(lfstr);
        }
    }

    String & String::Replace(const wchar_t fch, const wchar_t rch, bool useCase)
    {
        VOODOO_CHECK_IMPL;
        if (useCase)
        {
            boost::replace_all(m_Impl->m_Str, std::wstring(1, fch), std::wstring(1, rch));
        }
        else
        {
            boost::ireplace_all(m_Impl->m_Str, std::wstring(1, fch), std::wstring(1, rch));
        }
        return (*this);
    }

    String & String::Replace(const wchar_t * fstr, const wchar_t * rstr, bool useCase)
    {
        VOODOO_CHECK_IMPL;
        if (useCase)
        {
            boost::replace_all(m_Impl->m_Str, fstr, rstr);
        }
        else
        {
            boost::ireplace_all(m_Impl->m_Str, fstr, rstr);
        }
        return (*this);
    }

    String & String::Replace(const String & fstr, const String & rstr, bool useCase)
    {
        VOODOO_CHECK_IMPL;
        if (useCase)
        {
            boost::replace_all(m_Impl->m_Str, fstr.m_Impl->m_Str, rstr.m_Impl->m_Str);
        }
        else
        {
            boost::ireplace_all(m_Impl->m_Str, fstr.m_Impl->m_Str, rstr.m_Impl->m_Str);
        }
        return (*this);
    }

    String & String::Remove(const wchar_t fch, bool useCase)
    {
        VOODOO_CHECK_IMPL;
        if (useCase)
        {
            boost::erase_all(m_Impl->m_Str, std::wstring(1, fch));
        }
        else
        {
            boost::ierase_all(m_Impl->m_Str, std::wstring(1, fch));
        }
        return (*this);
    }

    String & String::Remove(const wchar_t * fstr, bool useCase)
    {
        VOODOO_CHECK_IMPL;
        if (useCase)
        {
            boost::erase_all(m_Impl->m_Str, fstr);
        }
        else
        {
            boost::ierase_all(m_Impl->m_Str, fstr);
        }
        return (*this);
    }

    String & String::Remove(const String & fstr, bool useCase)
    {
        VOODOO_CHECK_IMPL;
        if (useCase)
        {
            boost::erase_all(m_Impl->m_Str, fstr.m_Impl->m_Str);
        }
        else
        {
            boost::ierase_all(m_Impl->m_Str, fstr.m_Impl->m_Str);
        }
        return (*this);
    }

    uint32_t String::GetLength() const
    {
        VOODOO_CHECK_IMPL;
        return m_Impl->m_Str.size();
    }

    bool String::IsEmpty() const
    {
        VOODOO_CHECK_IMPL;
        return (this->GetLength() == 0);
    }

    wchar_t String::GetAt(uint32_t pos) const
    {
        VOODOO_CHECK_IMPL;
        //! @todo Error checking on pos.
        return m_Impl->m_Str.at(pos);
    }

    wchar_t & String::operator[](uint32_t pos)
    {
        VOODOO_CHECK_IMPL;
        //! @todo Error checking on pos.
        return m_Impl->m_Str[pos];
    }

    void String::SetAt(uint32_t pos, wchar_t data)
    {
        VOODOO_CHECK_IMPL;
        //! @todo Error checking on pos.
        m_Impl->m_Str[pos] = data;
    }

    const wchar_t * String::GetData() const
    {
        VOODOO_CHECK_IMPL;
        return m_Impl->m_Str.c_str();
    }

    bool String::operator<(const wchar_t * str) const
    {
        VOODOO_CHECK_IMPL;
        return (m_Impl->m_Str < str);
    }

    bool String::operator<(const String & str) const
    {
        VOODOO_CHECK_IMPL;
        return (m_Impl->m_Str < str.m_Impl->m_Str);
    }

    bool String::operator>(const wchar_t * str) const
    {
        VOODOO_CHECK_IMPL;
        return (m_Impl->m_Str > str);
    }

    bool String::operator>(const String & str) const
    {
        VOODOO_CHECK_IMPL;
        return (m_Impl->m_Str > str.m_Impl->m_Str);
    }

    String String::Time(time_t * pTime)
    {
        tm localTime;
        if (pTime)
        {
            if (localtime_s(&localTime, pTime) != 0)
            {
                return String(VSTR("Unknown Time"));
            }
        }
        else
        {
            time_t now = time(nullptr);
            if (localtime_s(&localTime, &now) != 0)
            {
                return String(VSTR("Unknown Time"));
            }
        }

        std::basic_stringstream<wchar_t, std::char_traits<wchar_t>> stamp;
        stamp << std::put_time(&localTime, VSTR("%H%M%S"));
        return stamp.str();
    }

    String String::Date(time_t * pTime)
    {
        using namespace std;

        tm localTime;
        if (pTime)
        {
            if (localtime_s(&localTime, pTime) != 0)
            {
                return String(VSTR("Unknown Date"));
            }
        }
        else
        {
            time_t now = time(nullptr);
            if (localtime_s(&localTime, &now) != 0)
            {
                return String(VSTR("Unknown Date"));
            }
        }

        std::basic_stringstream<wchar_t, std::char_traits<wchar_t>> stamp;
        stamp << std::put_time(&localTime, VSTR("%Y%m%d"));
        return stamp.str();
    }

    String String::Ticks()
    {
        StringFormat fmt(VSTR("%d"));
        fmt << (uint32_t)GetTickCount();

        return fmt.ToString();
    }
}
