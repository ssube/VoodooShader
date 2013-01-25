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
#include "String.hpp"

namespace VoodooShader
{
    /**
     * @defgroup voodoo_stdstream Standard Stream Operators
     * @ingroup voodoo_utility
     * @todo Stream input operators for structs, vectors and some classes.
     * @{
     */
#if !defined(VOODOO_NO_STDLIB)
    class VOODOO_API StreamState
    {
    public:
        StreamState(std::ios_base & in) : s(in), f(in.flags()) {};
        StreamState(const StreamState & o) : s(o.s), f(o.f) {};
        ~StreamState() 
        { 
            try
            {
                s.flags(f);
            }
            catch (const std::exception & exc)
            {
                UNREFERENCED_PARAMETER(exc);
            }
        };
        StreamState & operator=(const StreamState & o)
        {
            if (this != &o)
            {
				//! @todo Fix this assignment: apparently VS11 made std::ios_base::operator= private
                //s = o.s;
                f = o.f;
            }
            return (*this);
        };

    private:
        std::ios_base & s;
        std::ios_base::fmtflags f;
    };

    template<typename Elem>
    std::basic_ostream<Elem> & operator<<(std::basic_ostream<Elem> & out, const Exception & val)
    {
        return out << VSTR("Exception(") << val.what() << VSTR(")");
    }

    template<typename Elem>
    std::basic_ostream<Elem> & operator<<(std::basic_ostream<Elem> & out, const StringFormat & val)
    {
        return out << val.ToString().GetData();
    }

    template<typename Elem>
    std::basic_ostream<Elem> & operator<<(std::basic_ostream<Elem> & out, const Regex & val)
    {
        return out << VSTR("Regex(") << val.GetExpr().GetData() << VSTR(")");
    }

    template<typename Elem>
    std::basic_ostream<Elem> & operator<<(std::basic_ostream<Elem> & out, const String & val)
    {
        return out << val.GetData();
    }

    template<typename Elem, typename VecElem>
    std::basic_ostream<Elem> & operator<<(std::basic_ostream<Elem> & out, const Vector1<VecElem> & val)
    {
        return out << "[" << val.X << "]";
    }

    template<typename Elem, typename VecElem>
    std::basic_ostream<Elem> & operator<<(std::basic_ostream<Elem> & out, const Vector2<VecElem> & val)
    {
        return out << "[" << val.X << ", " << val.Y << "]";
    }

    template<typename Elem, typename VecElem>
    std::basic_ostream<Elem> & operator<<(std::basic_ostream<Elem> & out, const Vector3<VecElem> & val)
    {
        return out << "[" << val.X << ", " << val.Y << ", " << val.Z << "]";
    }        

    template<typename Elem, typename VecElem>
    std::basic_ostream<Elem> & operator<<(std::basic_ostream<Elem> & out, const Vector4<VecElem> & val)
    {
        return out << "[" << val.X << ", " << val.Y << ", " << val.Z << ", " << val.W << "]";
    }

    template<typename Elem>
    std::basic_ostream<Elem> & operator<<(std::basic_ostream<Elem> & out, const ParameterDesc & v)
    {
        return out << VSTR("ParameterDesc(") << Converter::ToString(v.Type) << VSTR(", ") << v.Rows << 
            VSTR(", ") << v.Columns << VSTR(", ") << v.Elements << VSTR(")");
    }

    template<typename Elem>
    std::basic_ostream<Elem> & operator<<(std::basic_ostream<Elem> & out, const TextureDesc & v)
    {
        return out << VSTR("TextureDesc(") << v.Format << VSTR(", ") << v.Levels << VSTR(", ") <<
              v.Size << VSTR(", ") << v.Usage << VSTR(")");
    }

    template<typename Elem>
    std::basic_ostream<Elem> & operator<<(std::basic_ostream<Elem> & out, const TextureRegion & v)
    {
        return out << VSTR("TextureRegion(") << v.Format << VSTR(", ") << v.Levels << VSTR(", ") <<
              v.Size << VSTR(", ") << v.Usage << VSTR(", ") << v.Origin << VSTR(")");
    }

    template<typename Elem>
    std::basic_ostream<Elem> & operator<<(std::basic_ostream<Elem> & out, const Variant & v)
    {
        out << VSTR("Variant(") << v.Type << VSTR(", ") << v.Components ;

        switch (v.Type)
        {
        case VSUT_None:
            break;
        case VSUT_Bool:
            out << VSTR(", ") << v.VBool;
            break;
        case VSUT_Int8:
            out << VSTR(", ") << v.VInt8;
            break;
        case VSUT_UInt8:
            out << VSTR(", ") << v.VUInt8;
            break;
        case VSUT_Int16:
            out << VSTR(", ") << v.VInt16;
            break;
        case VSUT_UInt16:
            out << VSTR(", ") << v.VUInt16;
            break;
        case VSUT_Int32:
            out << VSTR(", ") << v.VInt32;
            break;
        case VSUT_UInt32:
            out << VSTR(", ") << v.VUInt32;
            break;
        case VSUT_Float:
            out << VSTR(", ") << v.VFloat;
            break;
        case VSUT_Double:
            out << VSTR(", ") << v.VDouble;
            break;
        case VSUT_Uuid:
            if (v.VPUuid)
            {
                out << VSTR(", ") << (*v.VPUuid);
            }
            break;
        case VSUT_String:
            if (v.VPString)
            {
                out << VSTR(", ") << (*v.VPString);
            }
            break;
        case VSUT_IObject:
            out << VSTR(", ") << v.VPIObject;
            break;
        case VSUT_PVoid:
            out << VSTR(", ") << v.VPVoid;
            break;
        case VSUT_Unknown:
        default:
            break;
        }

        return out << VSTR(")");
    }

    template<typename Elem>
    std::basic_ostream<Elem> & operator<<(std::basic_ostream<Elem> & out, const Version & val)
    {
        out << VSTR("Version(") << val.LibId << VSTR(", ") << val.Major << VSTR(", ") << val.Minor << 
              VSTR(", ") << val.Patch << VSTR(", ") << val.Build << VSTR(", ") << val.Debug;

        if (val.Name)  { out << VSTR(", ") << val.Name;  };
        if (val.RevId) { out << VSTR(", ") << val.RevId; };

        out << VSTR(")");
        return out;
    }

    template<typename Elem>
    std::basic_ostream<Elem> & operator<<(std::basic_ostream<Elem> & out, const IObject * val)
    {
        if (val)
        {
            return out << val->ToString();
        }
        else
        {
            return out << VSTR("IObject()");
        }
    }
#endif
    /**
     * @}
     */
}
