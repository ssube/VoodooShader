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
    class StringImpl
    {
    public:
        StringImpl()
            : m_Str()
        {};

        StringImpl(const char * str)
            : m_Str(str)
        {};

    public:
        std::string m_Str;
    };

    void String::Init(const char * str)
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

    String String::Left(UInt32 count)
    {
        return String(m_Impl->m_Str.substr(0, count));
    }

    String String::Mid(UInt32 start, UInt32 count)
    {
        return String(m_Impl->m_Str.substr(start, count));
    }

    String String::Right(UInt32 count)
    {
        return String(m_Impl->m_Str.substr(max(this->GetLength() - count, 0), count));
    }

    Bool String::StartsWith(const char * str, Bool useCase)
    {
        if (useCase)
        {
            return boost::starts_with(m_Impl->m_Str, str);
        } else {
            return boost::istarts_with(m_Impl->m_Str, str);
        }
    }

    Bool String::EndsWith(const char * str, Bool useCase)
    {
        if (useCase)
        {
            return boost::ends_with(m_Impl->m_Str, str);
        } else {
            return boost::iends_with(m_Impl->m_Str, str);
        }
    }

    Bool String::Contains(const char * str, Bool useCase)
    {
        if (useCase)
        {
            return boost::contains(m_Impl->m_Str, str);
        } else {
            return boost::icontains(m_Impl->m_Str, str);
        }
    }

    Bool String::Compare(const char * str, Bool useCase)
    {
        if (useCase)
        {
            return boost::equals(m_Impl->m_Str, str);
        } else {
            return boost::iequals(m_Impl->m_Str, str);
        }
    }

    UInt32 String::Find(const char * str, Bool useCase)
    {
        if (useCase)
        {
            return m_Impl->m_Str.find(str);
        } else {
            return boost::to_lower_copy(m_Impl->m_Str).find(boost::to_lower_copy(str));
        }
    }

    UInt32 String::ReverseFind(const char * str, Bool useCase)
    {
        if (useCase)
        {
            return m_Impl->m_Str.rfind(str);
        } else {
            return boost::to_lower_copy(m_Impl->m_Str).rfind(boost::to_lower_copy(str));
        }
    }

    void String::Replace(const char * find, const char * replace, Bool useCase)
    {
        if (useCase)
        {
            return boost::replace_all(m_Impl->m_Str, find, replace);
        } else {
            return boost::ireplace_all(m_Impl->m_Str, find, replace);
        }
    }

    void String::Erase(const char * find, Bool useCase)
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

    UInt32 String::GetLength()
    {
        return m_Impl->m_Str.size();
    }

    Bool String::IsEmpty()
    {
        return (this->GetLength() == 0);
    }

    const char * String::GetData()
    {
        return m_Impl->m_Str.c_str();
    }

    char String::GetAt(UInt32 pos)
    {
        //! @todo Error checking on pos.
        return m_Impl->m_Str.at(pos);
    }

    char & String::operator[](UInt32 pos)
    {
        //! @todo Error checking on pos.
        return m_Impl->m_Str.at(pos);
    }

    void String::SetAt(UInt32 pos, char data)
    {
        //! @todo Error checking on pos.
        m_Impl->m_Str[pos] = data;
    }

    String::operator const char *()
    {
        return m_Impl->m_Str.c_str();
    }

    String & String::operator=(const char * other)
    {
        m_Impl->m_Str = other;
        return (*this);
    }

    String String::operator+(const char * other)
    {
        String str(this->GetData());
        str += other;
        return str;
    }

    String & String::operator+=(const char * other)
    {
        m_Impl->m_Str += other;
        return (*this);
    }

    Bool String::operator==(const char * other)
    {
        return (m_Impl->m_Str == other);
    }

    Bool String::operator!=(const char * other)
    {
        return !(*this == other);
    }

    Bool String::operator<(const char * other)
    {
        return (m_Impl->m_Str < other);
    }
}
