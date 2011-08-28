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

#pragma once

#include "Meta.hpp"
#include "Module.hpp"

namespace VoodooShader
{
    /**
     * Defines a simple interface all Voodoo objects from dynamic modules must inherit. This
     * interface handles destruction and identification of these objects.
     *
     * @warning All classes derived from IObject <b><em>must</em></b> have unique names.
     */
    class IObject
    {
    public:
        virtual String GetName() = 0;

        /**
         * Get the core this object was associated with (or nullptr if no core).
         */
        virtual Core * GetCore() = 0;
    };
}
