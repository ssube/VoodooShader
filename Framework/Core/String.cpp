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
        StringImpl() : 
          m_Str()
        {};

        StringImpl(const wchar_t * str) : 
          m_Str(str)
        {};

    public:
        std::wstring m_Str;
    };

    void String::Init(const wchar_t * str)
    {
        if (str)
        {
            m_Impl = new StringImpl(str);
        } else {
            m_Impl = new StringImpl();
        }
    }

    void String::ToLower()
    {
        return boost::to_lower(m_Impl->m_Str);
    }

    void String::ToUpper()
    {
        return boost::to_upper(m_Impl->m_Str);
    }

    void String::Clear()
    {
        return m_Impl->m_Str.clear();
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

    String String::Substr(uint32_t start, uint32_t count) const
    {
        if (count == String::Npos)
        {
            return String(m_Impl->m_Str.substr(start, std::string::npos));
        } else {
            return String(m_Impl->m_Str.substr(start, count));
        }
    }

    String String::Right(uint32_t count) const
    {
        return String(m_Impl->m_Str.substr(max(this->GetLength() - count, 0), count));
    }

    bool String::StartsWith(const char ch, bool useCase) const
    {
        if (useCase)
        {
            return boost::starts_with(m_Impl->m_Str, ch);
        } else {
            return boost::istarts_with(m_Impl->m_Str, ch);
        }
    }

    bool String::StartsWith(const char * str, bool useCase) const
    {
        if (useCase)
        {
            return boost::starts_with(m_Impl->m_Str, str);
        } else {
            return boost::istarts_with(m_Impl->m_Str, str);
        }
    }

    bool String::EndsWith(const char ch, bool useCase) const
    {
        if (useCase)
        {
            return boost::ends_with(m_Impl->m_Str, ch);
        } else {
            return boost::iends_with(m_Impl->m_Str, ch);
        }
    }

    bool String::EndsWith(const char * str, bool useCase) const
    {
        if (useCase)
        {
            return boost::ends_with(m_Impl->m_Str, str);
        } else {
            return boost::iends_with(m_Impl->m_Str, str);
        }
    }

    bool String::Contains(const char ch, bool useCase) const
    {
        if (useCase)
        {
            return boost::contains(m_Impl->m_Str, ch);
        } else {
            return boost::icontains(m_Impl->m_Str, ch);
        }
    }

    bool String::Contains(const char * str, bool useCase) const
    {
        if (useCase)
        {
            return boost::contains(m_Impl->m_Str, str);
        } else {
            return boost::icontains(m_Impl->m_Str, str);
        }
    }

    bool String::Compare(const char ch, bool useCase) const
    {
        if (useCase)
        {
            return boost::equals(m_Impl->m_Str, ch);
        } else {
            return boost::iequals(m_Impl->m_Str, ch);
        }
    }

    bool String::Compare(const char * str, bool useCase) const
    {
        if (useCase)
        {
            return boost::equals(m_Impl->m_Str, str);
        } else {
            return boost::iequals(m_Impl->m_Str, str);
        }
    }

    uint32_t String::Find(const char ch, bool useCase) const
    {
        if (useCase)
        {
            return m_Impl->m_Str.find(ch);
        } else {
            return boost::to_lower_copy(m_Impl->m_Str).find(boost::to_lower(ch));
        }
    }

    uint32_t String::Find(const char * str, bool useCase) const
    {
        if (useCase)
        {
            return m_Impl->m_Str.find(str);
        } else {
            return boost::to_lower_copy(m_Impl->m_Str).find(boost::to_lower_copy(str));
        }
    }

    uint32_t String::ReverseFind(const char ch, bool useCase) const
    {
        if (useCase)
        {
            return m_Impl->m_Str.rfind(ch);
        } else {
            return boost::to_lower_copy(m_Impl->m_Str).rfind(boost::to_lower_copy(ch));
        }
    }

    uint32_t String::ReverseFind(const char * str, bool useCase) const
    {
        if (useCase)
        {
            return m_Impl->m_Str.rfind(str);
        } else {
            return boost::to_lower_copy(m_Impl->m_Str).rfind(boost::to_lower_copy(str));
        }
    }

    void String::Replace(const char cfind, const char creplace, bool useCase)
    {
        if (useCase)
        {
            return boost::replace_all(m_Impl->m_Str, cfind, creplace);
        } else {
            return boost::ireplace_all(m_Impl->m_Str, cfind, creplace);
        }
    }

    void String::Replace(const char cfind, const char * replace, bool useCase)
    {
        if (useCase)
        {
            return boost::replace_all(m_Impl->m_Str, cfind, replace);
        } else {
            return boost::ireplace_all(m_Impl->m_Str, cfind, replace);
        }
    }

    void String::Replace(const char * find, const char creplace, bool useCase)
    {
        if (useCase)
        {
            return boost::replace_all(m_Impl->m_Str, find, creplace);
        } else {
            return boost::ireplace_all(m_Impl->m_Str, find, creplace);
        }
    }

    void String::Replace(const char * find, const char * replace, bool useCase)
    {
        if (useCase)
        {
            return boost::replace_all(m_Impl->m_Str, find, replace);
        } else {
            return boost::ireplace_all(m_Impl->m_Str, find, replace);
        }
    }

    void String::Erase(const char cfind, bool useCase)
    {
        if (useCase)
        {
            return boost::erase_all(m_Impl->m_Str, cfind);
        } else {
            return boost::ierase_all(m_Impl->m_Str, cfind);
        }
    }

    void String::Erase(const char * find, bool useCase)
    {
        if (useCase)
        {
            return boost::erase_all(m_Impl->m_Str, find);
        } else {
            return boost::ierase_all(m_Impl->m_Str, find);
        }
    }

    String String::Format(const char * fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        String str = String::FormatV(fmt, args);
        va_end(args);

        return str;
    }

    String String::FormatV(const char * fmt, va_list args)
    {
        char buffer[4096];
        vsnprintf_s(buffer, 4095, fmt, args);
        buffer[4095] = 0;
        return String(buffer);
    }

    void String::AppendFormat(_Printf_format_string_ const char * fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        (*this) += String::FormatV(fmt, args);
        va_end(args);        
    }

    uint32_t String::GetLength() const
    {
        return m_Impl->m_Str.size();
    }

    bool String::IsEmpty() const
    {
        return (this->GetLength() == 0);
    }

    const char * String::GetData() const
    {
        return m_Impl->m_Str.c_str();
    }

    char String::GetAt(uint32_t pos) const
    {
        //! @todo Error checking on pos.
        return m_Impl->m_Str.at(pos);
    }

    char & String::operator[](uint32_t pos)
    {
        //! @todo Error checking on pos.
        return m_Impl->m_Str.at(pos);
    }

    void String::SetAt(uint32_t pos, char data)
    {
        //! @todo Error checking on pos.
        m_Impl->m_Str[pos] = data;
    }

    String::operator const char *() const
    {
        return m_Impl->m_Str.c_str();
    }

    String & String::operator=(const char cother)
    {
        m_Impl->m_Str = cother;
        return (*this);
    }

    String & String::operator=(const char * other)
    {
        m_Impl->m_Str = other;
        return (*this);
    }

    const String String::operator+(const char cother) const
    {
        String str(this->GetData());
        str += cother;
        return str;
    }

    const String String::operator+(const char * other) const
    {
        String str(this->GetData());
        str += other;
        return str;
    }

    String & String::operator+=(const char cother)
    {
        m_Impl->m_Str += cother;
        return (*this);
    }

    String & String::operator+=(const char * other)
    {
        m_Impl->m_Str += other;
        return (*this);
    }

    bool String::operator==(const char * other) const
    {
        return (m_Impl->m_Str == other);
    }

    bool String::operator!=(const char * other) const
    {
        return !(*this == other);
    }

    bool String::operator<(const char * other) const
    {
        return (m_Impl->m_Str < other);
    }
}
