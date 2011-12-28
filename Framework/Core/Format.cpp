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

#include "Format.hpp"
#include "Stream.hpp"

#include <boost/format.hpp>

namespace VoodooShader
{
    class Format::FormatImpl
    {
    public:
        FormatImpl() : m_Format() { };
        FormatImpl(const vchar_t * fmt) : m_Format(fmt) { };

        boost::basic_format<vchar_t> m_Format;
    };

    Format::Format(_In_ const vchar_t * fmt) :
        m_Impl(new FormatImpl(fmt))
    {}

    Format::Format(_In_ const String & fmt) :
        m_Impl(new FormatImpl(fmt.GetData()))
    {
    }

    Format::~Format()
    {
        delete m_Impl;
    }

    String Format::ToString() const
    {
        std::basic_string<vchar_t, std::char_traits<vchar_t>> str(m_Impl->m_Format.str());
        return String(str);
    }

    Format & Format::operator<<(bool val)
    {
        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(uint8_t val)
    {
        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(int8_t val)
    {
        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(uint16_t val)
    {
        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(int16_t val)
    {
        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(uint32_t val)
    {
        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(int32_t val)
    {
        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(float val)
    {
        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(double val)
    {
        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(const void *& val)
    {
        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(const char val)
    {
        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(const char * val)
    {
        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(const wchar_t val)
    {
        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(const wchar_t * val)
    {
        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(const Exception & val)
    {
        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(const Regex & val)
    {
        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(const String & val)
    {
        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(const TextureDesc & val)
    {
        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(const TextureRegion & val)
    {
        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(const Uuid & val)
    {
        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(const Variant & val)
    {
        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(const Version & val)
    {
        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(const IObject * val)
    {
        if (val)
        {
            m_Impl->m_Format % val->ToString();
        } else {
            m_Impl->m_Format % VSTR("IObject(null)");
        }
        return (*this);
    }
}
