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

#include "Stream.hpp"

#include <string>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/uuid/string_generator.hpp>

#pragma warning(push)
#pragma warning(disable: 6246)
#include <boost/uuid/uuid_io.hpp>
#pragma warning(pop)

#include <iomanip>

namespace VoodooShader
{
    class Stream::StreamImpl
    {
    public:
        std::wstringstream m_Stream;
    };

    Stream & Stream::operator<<(uint8_t val)
    {
        m_Impl->m_Stream << val;
        return (*this);
    }

    Stream & Stream::operator<<(int8_t val)
    {
        m_Impl->m_Stream << val;
        return (*this);
    }

    Stream & Stream::operator<<(uint16_t val)
    {
        m_Impl->m_Stream << val;
        return (*this);
    }

    Stream & Stream::operator<<(int16_t val)
    {
        m_Impl->m_Stream << val;
        return (*this);
    }
    
    Stream & Stream::operator<<(uint32_t val)
    {
        m_Impl->m_Stream << val;
        return (*this);
    }

    Stream & Stream::operator<<(int32_t val)
    {
        m_Impl->m_Stream << val;
        return (*this);
    }

    Stream & Stream::operator<<(float val)
    {
        m_Impl->m_Stream << val;
        return (*this);
    }

    Stream & Stream::operator<<(double val)
    {
        m_Impl->m_Stream << val;
        return (*this);
    }

    Stream & Stream::operator<<(const void *& val)
    {
        m_Impl->m_Stream << val;
        return (*this);
    }

    Stream & Stream::operator<<(const Exception & val)
    {
        m_Impl->m_Stream << VSTR("Exception(") << val.what() << VSTR(")");
        return (*this);
    }

    Stream & Stream::operator<<(const Regex & val)
    {
        m_Impl->m_Stream << VSTR("Regex(") << val.GetExpr() << VSTR(")");
        return (*this);
    }

    Stream & Stream::operator<<(const String & val)
    {
        m_Impl->m_Stream << val.GetData();
        return (*this);
    }

    Stream & Stream::operator<<(const Uuid & val)
    {
        m_Impl->m_Stream << val;
        return (*this);
    }

    Stream & Stream::operator<<(const TextureDesc & val)
    {
        m_Impl->m_Stream << VSTR("TextureDesc(") << val.Format << VSTR(", ") << val.Mipmaps << 
            VSTR(", ") << val.RenderTarget << VSTR(", ") << val.Size << VSTR(")");
        return (*this);
    }

    Stream & Stream::operator<<(const TextureRegion & val)
    {
        m_Impl->m_Stream << VSTR("TextureRegion(") << val.Format << VSTR(", ") << val.Mipmaps << 
            VSTR(", ") << val.RenderTarget << VSTR(", ") << val.Size << VSTR(", ") << val.Region << VSTR(")");
        return (*this);
    }

    Stream & Stream::operator<<(const Variant & val)
    {
        m_Impl->m_Stream << VSTR("Variant(") << val.Type << VSTR(", ") << val.Components << VSTR(", ");
        switch (val.Type)
        {
        case UT_None:

        }
        return (*this);
    }

    Stream & Stream::operator<<(const Version & val)
    {
        m_Impl->m_Stream << VSTR("Version(uuid=") << val.LibId << 
            VSTR("; major=") << val.Major << VSTR("; minor=") << val.Minor <<
            VSTR("; patch=") << val.Patch << VSTR("; build=") << val.Build <<
            VSTR("; debug=") << val.Debug <<
            VSTR("; name=")  << val.Name << VSTR("; revid=") << val.RevId << VSTR(")");
        return (*this);
    }

    Stream & operator<<(Stream & out, const wchar_t val)
    {
        out.m_Impl->m_Stream << val;
        return out;
    }

    Stream & operator<<(Stream & out, const wchar_t * val)
    {
        if (val)
        {
            out.m_Impl->m_Stream << val;
        } else {
            out.m_Impl->m_Stream << VSTR("(null)");
        }
        return out;
    }