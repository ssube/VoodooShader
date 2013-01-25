/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2013 by Sean Sube
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
#include "Stream.hpp"
#include "String.hpp"

namespace VoodooShader
{
    /**
     * @ingroup voodoo_utility
     * Printf-style formatting class with type safety and some extended features, including handling of standard Voodoo
     * types.
     */
    class VOODOO_API StringFormat
    {
        class FormatImpl;

    public:
        StringFormat(_In_z_ CONST wchar_t * fmt);
        StringFormat(_In_ CONST String & fmt);
        ~StringFormat();
        
        String ToString() CONST;
        operator String() CONST
        {
            return this->ToString();
        }

        StringFormat & operator<<(CONST bool val);
        StringFormat & operator<<(CONST char val);
        StringFormat & operator<<(CONST unsigned char val);
        StringFormat & operator<<(CONST short val);
        StringFormat & operator<<(CONST unsigned short val);
        StringFormat & operator<<(CONST int val);
        StringFormat & operator<<(CONST unsigned int val);
        StringFormat & operator<<(CONST long val);
        StringFormat & operator<<(CONST unsigned long val);
		StringFormat & operator<<(CONST long long val);
		StringFormat & operator<<(CONST unsigned long long val);
        StringFormat & operator<<(CONST float val);
        StringFormat & operator<<(CONST double val);

        StringFormat & operator<<(CONST wchar_t val);

#if defined(_STRING)
        StringFormat & operator<<(std::string val)
        {
            return this->operator<<(val.c_str());
        }
        StringFormat & operator<<(std::wstring val)
        {
            return this->operator<<(val.c_str());
        }
#endif

        StringFormat & operator<<(CONST Exception & val);
        StringFormat & operator<<(CONST StringFormat & val);
        StringFormat & operator<<(CONST Regex & val);
        StringFormat & operator<<(CONST String & val);

        StringFormat & operator<<(CONST ParameterDesc & val);
        StringFormat & operator<<(CONST TextureDesc & val);
        StringFormat & operator<<(CONST TextureRegion & val);
        StringFormat & operator<<(CONST Uuid & val);
        StringFormat & operator<<(CONST Variant & val);
        StringFormat & operator<<(CONST Version & val);

        // Pointer overloads
        StringFormat & operator<<(CONST void * val);
        StringFormat & operator<<(CONST char * val);
        StringFormat & operator<<(CONST wchar_t * val);
        StringFormat & operator<<(CONST IObject * val);

        template<typename T>
        StringFormat & operator<<(CONST Vector1<T> & val)
        {
            this->operator<<(VSTR("[")) << val.X << VSTR("]");
        }
        template<typename T>
        StringFormat & operator<<(CONST Vector2<T> & val)
        {
            this->operator<<(VSTR("[")) << val.X << VSTR(", ") << val.Y << VSTR("]");
        }
        template<typename T>
        StringFormat & operator<<(CONST Vector3<T> & val)
        {
            this->operator<<(VSTR("[")) << val.X << VSTR(", ") << val.Y << VSTR(", ") << val.Z << VSTR("]");
        }
        template<typename T>
        StringFormat & operator<<(CONST Vector4<T> & val)
        {
            this->operator<<(VSTR("[")) << val.X << VSTR(", ") << val.Y << VSTR(", ") << val.Z << VSTR(", ") << val.W << VSTR("]");
        }

    private:
        FormatImpl * m_Impl;
    };
}
