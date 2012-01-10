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
#include "VoodooInternal.hpp"

#include <d3dx9shader.h>

namespace VoodooShader
{
    class VSCompiler
    {
    public:
        static VoodooResult VOODOO_METHODTYPE CreateEffectFromFile(IFile * pFile, CompileFlags flags, VSShader ** ppShader);
        static VoodooResult VOODOO_METHODTYPE CompileProgramDX9(
            ICore * pCore, const std::string & source,
            const std::vector<std::pair<std::string, std::string>> & defines, 
            const std::string & function, const std::string & profile, CompileFlags flags,
            VSProgram ** ppProgram
        );
    };
}