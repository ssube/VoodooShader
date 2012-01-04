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
#include "String.hpp"

namespace VoodooShader
{
    /**
     * @ingroup voodoo_utility
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
     * @defgroup voodoo_stdstream Standard Stream Operators
     * @ingroup voodoo_utility
     * @{
     */
#if !defined(VOODOO_NO_STDLIB)
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
            }
            else
            {
                os << VSTR("(null)");
            }
            break;
        case UT_String:
            if (v.VString)
            {
                os << v.VString->GetData();
            }
            else
            {
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
        os << VSTR("Version(LibId: ") << v.LibId << VSTR("; Major: ") << v.Major << VSTR("; Minor: ") << v.Minor << 
              VSTR("; Patch: ") << v.Patch << VSTR("; Build: ") << v.Build << VSTR("; Debug: ") << v.Debug;

        if (v.Name)  { os << VSTR("; Name: ")  << v.Name;  };
        if (v.RevId) { os << VSTR("; RevId: ") << v.RevId; };

        os << VSTR(")");
        return os;
    }

    template<typename Elem>
    std::basic_ostream<Elem> & operator<<(std::basic_ostream<Elem> & os, const IObject * v)
    {
        if (v)
        {
            os << v->ToString();
        }
        else
        {
            os << VSTR("IObject(null)");
        }
        return os;
    }
#endif
    /**
     * @}
     */
}
