/**************************************************************************************************\
 * This file is part of the Voodoo Shader Framework, a comprehensive shader support library.
 * Copyright (c) 2010-2011 by Sean Sube
 *
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program;
 * if  not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301 US
 *
 * Support and more information may be found at http://www.voodooshader.com, or by contacting the
 * developer at peachykeen@voodooshader.com
\**************************************************************************************************/

#ifndef VOODOO_CONVERTER_HPP
#define VOODOO_CONVERTER_HPP

#include "Meta.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup VoodooCore
     * @{
     */

    /**
     * Conversion-helper class for Voodoo core types. This class provides
     * methods to convert various enums to other types or retrieve readable
     * names.
     */
    class VOODOO_API Converter
    {
    public:
#ifndef VOODOO_NO_CG
        /**
         * Convert a Voodoo parameter type to the corresponding Cg runtime type.
         * This function should be used to convert between these two types, as
         * it allows for future types to be added or updated as necessary
         * without changing any other code.
         *
         * @param pt The internal parameter type.
         * @return The Cg runtime's corresponding type, if one is known.
         */
        static CGtype ToCGType
        (
            _In_ ParameterType pt
        );

        /**
        * Convert a Cg runtime type to the corresponding Voodoo parameter type.
        * This function should be used to convert between these two types, as it
        * allows for future types to be added or updated as necessary without
        * changing any other code.
        *
        * @param pt The Cg runtime's type.
        * @return The internal parameter corresponding type, if one is known.
        */
        static ParameterType ToParameterType
        (
            _In_ CGtype pt
        );
#endif

        static ParameterCategory ToParameterCategory
        (
            _In_ ParameterType type
        );

        static TextureFormat ToTextureFormat
        (
            _In_ const char * format
        );

        static int ToComponents
        (
            _In_ ParameterType type
        );

        /**
         * Helper function to convert enum values into readable strings used in
         * logging; this overload handles texture formats.
         *
         * @param tf The texture format to find the name of.
         * @return A C-string with the name (usually identical to the
         *        identifier).
         */
        static const char * ToString
        (
            _In_ TextureFormat tf
        );

        /**
        * Helper function to convert enum values into readable strings used in
        * logging; this overload handles parameter types.
        *
        * @param pt The parameter type to find the name of.
        * @return A C-string with the name (usually identical to the
        *        identifier).
        */
        static const char * ToString
        (
            _In_ ParameterType pt
        );

        static const char * ToString
        (
            _In_ ParameterCategory cat
        );
    };
    /**
     * @}
     */
}

#endif /*VOODOO_CONVERTER_HPP*/
