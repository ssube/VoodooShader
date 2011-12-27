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
        String ToString();

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
        Stream & operator<<(const Uuid & val);

        Stream & operator<<(const TextureDesc & val);
        Stream & operator<<(const TextureRegion & val);
        Stream & operator<<(const Variant & val);
        Stream & operator<<(const Version & val);

        Stream & operator<<(Stream & (*pf)(Stream &))
        {
            if (pf)
            {
                return pf(*this);
            } else {
                return (*this);
            }
        }

        friend Stream & operator<<(Stream & out, const wchar_t val);
        friend Stream & operator<<(Stream & out, const wchar_t * val);
        friend Stream & operator<<(Stream & out, const IObject * val);

        template<typename T>
        friend Stream & operator<<(Stream & out, const Vector1<T> & val);
        template<typename T>
        friend Stream & operator<<(Stream & out, const Vector2<T> & val);
        template<typename T>
        friend Stream & operator<<(Stream & out, const Vector3<T> & val);
        template<typename T>
        friend Stream & operator<<(Stream & out, const Vector4<T> & val);
        /**
         * @}
         * @name Finalization Operators
         * These operators finalize the stream, returning it in string form with an optional trailing newline.
         * @{
         */
        String operator!()
        {
            this->operator<<(VSTR("\n"));
            return this->operator~();
        }
        String operator~()
        {
            return this->ToString();
        }
        /**
         * @}
         */
    private:
        std::unique_ptr<StreamImpl> m_Impl;
    };

    Stream & operator<<(Stream & out, const wchar_t val);
    Stream & operator<<(Stream & out, const wchar_t * val);

    Stream & operator<<(Stream & out, const IObject * val)
    {
        if (val)
        {
            out << val->ToString();
        } else {
            out << VSTR("IObject(null)");
        }
    }

    template<typename T>
    Stream & operator<<(Stream & out, const Vector1<T> & val)
    {
        out << "[" << val.X << "]";
    }

    template<typename T>
    Stream & operator<<(Stream & out, const Vector2<T> & val)
    {
        out << "[" << val.X << ", " << val.Y << "]";
    }

    template<typename T>
    Stream & operator<<(Stream & out, const Vector3<T> & val)
    {
        out << "[" << val.X << ", " << val.Y << ", " << val.Z << "]";
    }        

    template<typename T>
    Stream & operator<<(Stream & out, const Vector4<T> & val)
    {
        out << "[" << val.X << ", " << val.Y << ", " << val.Z << ", " << val.W << "]";
    }

    /**
     * @addtogroup voodoo_stream_manipulators Stream Manipulators
     * @{
     */
    Stream & Hex(Stream & val);
    String Print(Stream & val);
    /**
     * @}
     */
}
