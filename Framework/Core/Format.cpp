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
#pragma once

#include "VoodooFramework.hpp"
// Boost
#include <boost/format.hpp>
#pragma warning(push)
#pragma warning(disable: 6246)
#include <boost/uuid/uuid_io.hpp>
#pragma warning(pop)

namespace VoodooShader
{
    class Format::FormatImpl
    {
    public:
        FormatImpl() : m_Format() { };
        FormatImpl(const wchar_t * fmt) : m_Format(fmt) { };

        boost::basic_format<wchar_t> m_Format;
    };

    Format::Format(_In_ const wchar_t * fmt) :
        m_Impl(new FormatImpl(fmt))
    {
        try
        {
            m_Impl = new FormatImpl(fmt);
        } catch (boost::exception & exc)
        {
            UNREFERENCED_PARAMETER(exc);
            Throw(VOODOO_CORE_NAME, VSTR("Format string error"), nullptr);
        }
    }

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
        VOODOO_CHECK_IMPL;

        std::basic_string<wchar_t, std::char_traits<wchar_t>> str(m_Impl->m_Format.str());
        return String(str);
    }

    Format & Format::operator<<(bool val)
    {
        VOODOO_CHECK_IMPL;

        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(char val)
    {
        VOODOO_CHECK_IMPL;

        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(unsigned char val)
    {
        VOODOO_CHECK_IMPL;

        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(short val)
    {
        VOODOO_CHECK_IMPL;

        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(unsigned short val)
    {
        VOODOO_CHECK_IMPL;

        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(int val)
    {
        VOODOO_CHECK_IMPL;

        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(unsigned int val)
    {
        VOODOO_CHECK_IMPL;

        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(long val)
    {
        VOODOO_CHECK_IMPL;

        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(unsigned long val)
    {
        VOODOO_CHECK_IMPL;

        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(float val)
    {
        VOODOO_CHECK_IMPL;

        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(double val)
    {
        VOODOO_CHECK_IMPL;

        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(wchar_t val)
    {
        VOODOO_CHECK_IMPL;

        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(const Exception & val)
    {
        VOODOO_CHECK_IMPL;

        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(const Regex & val)
    {
        VOODOO_CHECK_IMPL;

        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(const String & val)
    {
        VOODOO_CHECK_IMPL;

        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(const TextureDesc & val)
    {
        VOODOO_CHECK_IMPL;

        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(const TextureRegion & val)
    {
        VOODOO_CHECK_IMPL;

        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(const Uuid & val)
    {
        VOODOO_CHECK_IMPL;

        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(const Variant & val)
    {
        VOODOO_CHECK_IMPL;

        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(const Version & val)
    {
        VOODOO_CHECK_IMPL;

        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(const void * val)
    {
        VOODOO_CHECK_IMPL;

        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(const char * val)
    {
        VOODOO_CHECK_IMPL;

        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(const wchar_t * val)
    {
        VOODOO_CHECK_IMPL;

        m_Impl->m_Format % val;
        return (*this);
    }

    Format & Format::operator<<(const IObject * val)
    {
        VOODOO_CHECK_IMPL;

        if (val)
        {
            m_Impl->m_Format % val->ToString();
        }
        else
        {
            m_Impl->m_Format % VSTR("IObject(null)");
        }
        return (*this);
    }
}
