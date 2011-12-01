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

#include "Frost_Module.hpp"
#include "Frost_Adapter.hpp"

VoodooShader::ICore * gpVoodooCore = nullptr;
VoodooShader::IShader * gpTestShader;

GLint gDepthTexture = 0;
GLint gThisFrame = 0;
GLint gLastFrame = 0;
GLint gLastShader = 0;
GLint gLastPass = 0;
GLsizei gViewWidth = 0;
GLsizei gViewHeight = 0;

namespace VoodooShader
{
    namespace VoodooFrost
    {
        const Version * API_ModuleVersion(void)
        {
            static const Version moduleVersion = VOODOO_META_VERSION_STRUCT(FROST);

            return &moduleVersion;
        }

        uint32_t API_ClassCount(void)
        {
            return 1;
        }

        const wchar_t * API_ClassInfo(_In_ uint32_t number, Uuid & clsid)
        {
            if (number == 0)
            {
                clsid = CLSID_FrostAdapter;
                return L"Frost_Adapter";
            }
            else
            {
                return nullptr;
            }
        }

        IObject * API_ClassCreate(_In_ uint32_t index, _In_ ICore * pCore)
        {
            if (index == 0)
            {
                return new FrostAdapter(pCore);
            }
            else
            {
                return nullptr;
            }
        }
    }
}
