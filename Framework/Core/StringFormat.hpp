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
        StringFormat(_In_ const wchar_t * fmt);
        StringFormat(_In_ const String & fmt);
        ~StringFormat();
        
        String ToString() const;
        operator String() const
        {
            return this->ToString();
        }

        StringFormat & operator<<(bool val);
        StringFormat & operator<<(char val);
        StringFormat & operator<<(unsigned char val);
        StringFormat & operator<<(short val);
        StringFormat & operator<<(unsigned short val);
        StringFormat & operator<<(int val);
        StringFormat & operator<<(unsigned int val);
        StringFormat & operator<<(long val);
        StringFormat & operator<<(unsigned long val);
		StringFormat & operator<<(long long val);
		StringFormat & operator<<(unsigned long long val);
        StringFormat & operator<<(float val);
        StringFormat & operator<<(double val);

        StringFormat & operator<<(wchar_t val);

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

        StringFormat & operator<<(const Exception & val);
        StringFormat & operator<<(const StringFormat & val);
        StringFormat & operator<<(const Regex & val);
        StringFormat & operator<<(const String & val);

        StringFormat & operator<<(const ParameterDesc & val);
        StringFormat & operator<<(const TextureDesc & val);
        StringFormat & operator<<(const TextureRegion & val);
        StringFormat & operator<<(const Uuid & val);
        StringFormat & operator<<(const Variant & val);
        StringFormat & operator<<(const Version & val);

        // Pointer overloads
        StringFormat & operator<<(const void * val);
        StringFormat & operator<<(const char * val);
        StringFormat & operator<<(const wchar_t * val);
        StringFormat & operator<<(const IObject * val);

        template<typename T>
        StringFormat & operator<<(const Vector1<T> & val)
        {
            this->operator<<(VSTR("[")) << val.X << VSTR("]");
        }
        template<typename T>
        StringFormat & operator<<(const Vector2<T> & val)
        {
            this->operator<<(VSTR("[")) << val.X << VSTR(", ") << val.Y << VSTR("]");
        }
        template<typename T>
        StringFormat & operator<<(const Vector3<T> & val)
        {
            this->operator<<(VSTR("[")) << val.X << VSTR(", ") << val.Y << VSTR(", ") << val.Z << VSTR("]");
        }
        template<typename T>
        StringFormat & operator<<(const Vector4<T> & val)
        {
            this->operator<<(VSTR("[")) << val.X << VSTR(", ") << val.Y << VSTR(", ") << val.Z << VSTR(", ") << val.W << VSTR("]");
        }

    private:
        FormatImpl * m_Impl;
    };
}
