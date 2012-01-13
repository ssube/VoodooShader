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

namespace VoodooShader
{
    /**
     * @ingroup voodoo_utility
     * Conversion helper class for Voodoo. This class provides methods to convert various enums to other types or retrieve 
     * readable names (for logging).
     */
    class VOODOO_API Converter
    {
    public:
        static TextureFormat ToTextureFormat(_In_ const wchar_t * format);

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
        static const wchar_t * ToString(ShaderStage stage);
    };
}
