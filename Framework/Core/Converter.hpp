/**
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

namespace VoodooShader
{
    /**
     * Conversion helper class for Voodoo and Cg types. This class provides methods to convert various enums to
     * other types or retrieve readable names (for logging).
     */
    class VOODOO_API Converter
    {
    public:
#ifndef VOODOO_NO_CG
        /**
         * Convert a Voodoo parameter type to the corresponding Cg runtime type. This function should be used to
         * convert between these two types, as it allows for future types to be added or updated as necessary
         * without changing any other code.
         *
         * @param type The internal parameter type.
         * @return The corresponding Cg type, if one is known.
         */
        static CGtype ToCGType(_In_ ParameterType type);

        /**
         * Convert a Cg runtime type to the corresponding Voodoo parameter type. This function should be used to
         * convert between these two types, as it allows for future types to be added or updated as necessary
         * without changing any other code.
         *
         * @param type The Cg type.
         * @return The corresponding parameter type, if one is known.
         */
        static ParameterType ToParameterType(_In_ CGtype type);
#endif

        static ParameterCategory ToParameterCategory(_In_ ParameterType type);

        static TextureFormat ToTextureFormat(_In_ const wchar_t * format);

        static int ToComponents(_In_ ParameterType Type);

        /**
         * Helper function to convert enum values into readable strings used in logging;
         * this overload handles texture formats.
         *
         * @param format The texture format to find the name of.
         * @return A string with the name (usually identical to the identifier).
         */
        static const wchar_t * ToString(_In_ TextureFormat format);

        /**
         * Helper function to convert enum values into readable strings used in logging;
         * this overload handles parameter types.
         *
         * @param type The parameter type to find the name of.
         * @return A string with the name (usually identical to the identifier).
         */
        static const wchar_t * ToString(_In_ ParameterType type);

        /**
         * Helper function to convert enum values into readable strings used in logging;
         * this overload handles parameter types.
         *
         * @param category The parameter type to find the name of.
         * @return A string with the name (usually identical to the identifier).
         */
        static const wchar_t * ToString(_In_ ParameterCategory category);
    };
}
