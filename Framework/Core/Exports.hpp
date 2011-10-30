/**
 * This file is part of the Voodoo Shader Framework, a comprehensive shader support library. 
 * 
 * Copyright (c) 2010-2011 by Sean Sube 
 * 
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation; either version 2 of the License, or (at your 
 * option) any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License along with this program; if not, write to 
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 US 
 * 
 * Support and more information may be found at 
 *   http://www.voodooshader.com
 * or by contacting the lead developer at 
 *   peachykeen@voodooshader.com
 **/
#pragma once

#include "Includes.hpp"

#include "String.hpp"

namespace VoodooShader
{
    /**
     * Creates a new core. This function is exported and meant for use by the loader.
     * 
     * @param pInitParams Setup parameters for this core. 
     * @return A new ICore object.
     * @throws std::exception in case of errors, if CatchErrors is false.
     */
    _Check_return_ ICore * VOODOO_CALL CreateCore(_In_ const InitParams * const pInitParams, _In_ bool catchErrors = true);

    /* Plugin exports. */
    const Version * VOODOO_CALL API_ModuleVersion(void);
    const uint32_t   VOODOO_CALL API_ClassCount(void);
    const char *    VOODOO_CALL API_ClassInfo(_In_ const uint32_t Index);
    IObject *       VOODOO_CALL API_ClassCreate(_In_ const uint32_t Index, _In_ ICore * pCore);
}
