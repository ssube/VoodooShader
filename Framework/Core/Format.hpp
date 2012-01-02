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

#include "Includes.hpp"

#include "Exception.hpp"
#include "Regex.hpp"
#include "String.hpp"

#include "IObject.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup voodoo_utility Voodoo Utility
     * @{
     */

    /**
     * Printf-style formatting class with type safety and some extended features, including handling of standard Voodoo
     * types.
     */
    class VOODOO_API Format
    {
        class FormatImpl;

    public:
        Format(_In_ const wchar_t * fmt);
        Format(_In_ const String & fmt);
        ~Format();
        
        String ToString() const;
        operator String() const
        {
            return this->ToString();
        }

        Format & operator<<(bool val);
        Format & operator<<(char val);
        Format & operator<<(unsigned char val);
        Format & operator<<(short val);
        Format & operator<<(unsigned short val);
        Format & operator<<(int val);
        Format & operator<<(unsigned int val);
        Format & operator<<(long val);
        Format & operator<<(unsigned long val);
        Format & operator<<(float val);
        Format & operator<<(double val);

        Format & operator<<(wchar_t val);

        Format & operator<<(const Exception & val);
        Format & operator<<(const Format & val);
        Format & operator<<(const Regex & val);
        Format & operator<<(const String & val);

        Format & operator<<(const TextureDesc & val);
        Format & operator<<(const TextureRegion & val);
        Format & operator<<(const Uuid & val);
        Format & operator<<(const Variant & val);
        Format & operator<<(const Version & val);

        // Pointer overloads
        Format & operator<<(const void * val);
        Format & operator<<(const char * val);
        Format & operator<<(const wchar_t * val);
        Format & operator<<(const IObject * val);

        template<typename T>
        Format & operator<<(const Vector1<T> & val)
        {
            this->operator<<(VSTR("[")) << val.X << VSTR("]");
        }
        template<typename T>
        Format & operator<<(const Vector2<T> & val)
        {
            this->operator<<(VSTR("[")) << val.X << VSTR(", ") << val.Y << VSTR("]");
        }
        template<typename T>
        Format & operator<<(const Vector3<T> & val)
        {
            this->operator<<(VSTR("[")) << val.X << VSTR(", ") << val.Y << VSTR(", ") << val.Z << VSTR("]");
        }
        template<typename T>
        Format & operator<<(const Vector4<T> & val)
        {
            this->operator<<(VSTR("[")) << val.X << VSTR(", ") << val.Y << VSTR(", ") << val.Z << VSTR(", ") << val.W << VSTR("]");
        }

    private:
        FormatImpl * m_Impl;
    };
    /**
     * @}
     */
}
