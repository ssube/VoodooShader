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

#include "Includes.hpp"
#include "Exception.hpp"
#include "String.hpp"
#include "Regex.hpp"
#include "IObject.hpp"

#ifndef VOODOO_NO_STREAM
#include <ios>
#endif

namespace VoodooShader
{
    /**
     * @class Stream
     *
     * Provides formatted string creation.
     */
    class VOODOO_API Stream
    {
        class StreamImpl;

    public:
        Stream();
        Stream(_In_ const String & str);
        ~Stream();
                
        void Clear();

        String ToString() const;
        operator String() const
        {
            return this->ToString();
        }

        /**
         * @name Insertion Operators
         * @{
         */
        Stream & operator<<(bool val);
        Stream & operator<<(uint8_t val);
        Stream & operator<<(int8_t val);
        Stream & operator<<(uint16_t val);
        Stream & operator<<(int16_t val);
        Stream & operator<<(uint32_t val);
        Stream & operator<<(int32_t val);
        Stream & operator<<(float val);
        Stream & operator<<(double val);
        Stream & operator<<(const void *& pVal);

        Stream & operator<<(const Exception & val);
        Stream & operator<<(const Regex & val);
        Stream & operator<<(const Stream & val);
        Stream & operator<<(const String & val);

        Stream & operator<<(const TextureDesc & val);
        Stream & operator<<(const TextureRegion & val);
        Stream & operator<<(const Uuid & val);
        Stream & operator<<(const Variant & val);
        Stream & operator<<(const Version & val);
        
        Stream & operator<<(const char val);
        Stream & operator<<(const char * val);
        Stream & operator<<(const wchar_t val);
        Stream & operator<<(const wchar_t * val);
        Stream & operator<<(const IObject * val);

        Stream & operator<<(Stream & (*pf)(Stream &))
        {
            if (pf)
            {
                return pf(*this);
            } else {
                return (*this);
            }
        }

        String operator<<(String (*pf)(Stream &))
        {
            if (pf)
            {
                return pf(*this);
            } else {
                return this->ToString();
            }
        }

        template<typename T>
        Stream & operator<<(const Vector1<T> & val)
        {
            (*this) << VSTR("{") << val.X << VSTR("}");
            return (*this);
        };

        template<typename T>
        Stream & operator<<(const Vector2<T> & val)
        {
            (*this) << VSTR("{") << val.X << VSTR("; ") << val.Y << VSTR("}");
            return (*this);
        };

        template<typename T>
        Stream & operator<<(const Vector3<T> & val)
        {
            (*this) << VSTR("{") << val.X << VSTR("; ") << val.Y << VSTR("; ") << val.Z << VSTR("}");
            return (*this);
        };

        template<typename T>
        Stream & operator<<(const Vector4<T> & val)
        {
            (*this) << VSTR("{") << val.X << VSTR("; ") << val.Y << VSTR("; ") << val.Z << VSTR("; ") << val.W << VSTR("}");
            return (*this);
        };
        /**
         * @}
         */
    private:
        StreamImpl * m_Impl;
    };

    /**
     * @addtogroup voodoo_stream_manipulators Stream Manipulators
     * @{
     */
    Stream & Hex(Stream & val);
    String Print(Stream & val);
    /**
     * @}
     */

#ifndef VOODOO_NO_STREAM
    template<typename Elem>
    std::basic_ostream<Elem> & operator<<(std::basic_ostream<Elem> & out, const Exception & val)
    {
        out << VSTR("Exception(") << val.what() << VSTR(")");
        return out;
    }

    template<typename Elem>
    std::basic_ostream<Elem> & operator<<(std::basic_ostream<Elem> & out, const Format & val)
    {
        out << val.ToString().GetData();
        return out;
    }

    template<typename Elem>
    std::basic_ostream<Elem> & operator<<(std::basic_ostream<Elem> & out, const Regex & val)
    {
        out << VSTR("Regex(") << val.GetExpr().GetData() << VSTR(")");
        return out;
    }

    template<typename Elem>
    std::basic_ostream<Elem> & operator<<(std::basic_ostream<Elem> & out, const String & val)
    {
        out << val.GetData();
        return out;
    }

    template<typename Elem, typename VecElem>
    std::basic_ostream<Elem> & operator<<(std::basic_ostream<Elem> & out, const Vector1<VecElem> & val)
    {
        out << "[" << val.X << "]";
        return out;
    }

    template<typename Elem, typename VecElem>
    std::basic_ostream<Elem> & operator<<(std::basic_ostream<Elem> & out, const Vector2<VecElem> & val)
    {
        out << "[" << val.X << ", " << val.Y << "]";
        return out;
    }

    template<typename Elem, typename VecElem>
    std::basic_ostream<Elem> & operator<<(std::basic_ostream<Elem> & out, const Vector3<VecElem> & val)
    {
        out << "[" << val.X << ", " << val.Y << ", " << val.Z << "]";
        return out;
    }        

    template<typename Elem, typename VecElem>
    std::basic_ostream<Elem> & operator<<(std::basic_ostream<Elem> & out, const Vector4<VecElem> & val)
    {
        out << "[" << val.X << ", " << val.Y << ", " << val.Z << ", " << val.W << "]";
        return out;
    }

    template<typename Elem>
    std::basic_ostream<Elem> & operator<<(std::basic_ostream<Elem> & os, const TextureDesc & v)
    {
        os << VSTR("TextureDesc(Format: ") << v.Format << VSTR("; Mipmaps: ") << v.Mipmaps << VSTR("; RenderTarget: ") <<
              v.RenderTarget << VSTR("; Size: ") << v.Size << VSTR(")");
        return os;
    }

    template<typename Elem>
    std::basic_ostream<Elem> & operator<<(std::basic_ostream<Elem> & os, const TextureRegion & v)
    {
        os << VSTR("TextureRegion(Format: ") << v.Format << VSTR("; Mipmaps: ") << v.Mipmaps << VSTR("; RenderTarget: ") <<
              v.RenderTarget << VSTR("; Size: ") << v.Size << VSTR("; Origin: ") << v.Origin << VSTR(")");
        return os;
    }

    template<typename Elem>
    std::basic_ostream<Elem> & operator<<(std::basic_ostream<Elem> & os, const Uuid & v)
    {
        std::ios_base::fmtflags flags = os.flags(std::ios_base::hex);
        for (int i = 0; i < 16; ++i)
        {
            os << v.data[i];
            if (i == 3 || i == 5 || i == 7 || i == 9) os << VSTR("-");
        }
        os.flags(flags);

        return os;
    }

    template<typename Elem>
    std::basic_ostream<Elem> & operator<<(std::basic_ostream<Elem> & os, const Variant & v)
    {
        os << VSTR("Variant(") << v.Type << VSTR("; ") << v.Components;

        if (v.Type == UT_None)
        {
            return os << VSTR(")");
        }

        switch (v.Type)
        {
        case UT_None:
            break;
        case UT_Bool:
            os << v.VBool;
            break;
        case UT_Int8:
            os << v.VInt8;
            break;
        case UT_UInt8:
            os << v.VUInt8;
            break;
        case UT_Int16:
            os << v.VInt16;
            break;
        case UT_UInt16:
            os << v.VUInt16;
            break;
        case UT_Int32:
            os << v.VInt32;
            break;
        case UT_UInt32:
            os << v.VUInt32;
            break;
        case UT_Float:
            os << v.VFloat;
            break;
        case UT_Double:
            os << v.VDouble;
            break;
        case UT_Uuid:
            if (v.VUuid)
            {
                Uuid t = (*v.VUuid);
                os << t;
            } else {
                os << VSTR("(null)");
            }
            break;
        case UT_String:
            if (v.VString)
            {
                os << v.VString->GetData();
            } else {
                os << VSTR("(null)");
            }
            break;
        case UT_IObject:
            os << VSTR("; ") << v.VIObject;
            break;
        case UT_PVoid:
            os << VSTR("; ") << v.VPVoid;
            break;
        case UT_Unknown:
        default:
            os << VSTR("; (unknown)");
            break;
        }

        return os << VSTR(")");
    }

    template<typename Elem>
    std::basic_ostream<Elem> & operator<<(std::basic_ostream<Elem> & os, const Version & v)
    {
        os << VSTR("Version(LibId: ") << v.LibId << VSTR("; Major: ") << v.Major << VSTR("; Minor: ") <<
              v.Minor << VSTR("; Patch: ") << v.Patch << VSTR("; Build: ") << v.Build << VSTR("; Debug: ") <<
              v.Debug << VSTR("; Name: ") << v.Name << VSTR("; RevId: ") << v.RevId << VSTR(")");
        return os;
    }
#endif
}
