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

#include "Voodoo_D3D9.hpp"
#include "D3D9_Version.hpp"
#include "VSBindingDX9.hpp"

namespace VoodooShader
{
    namespace Voodoo_D3D9
    {
        static const Version moduleVersion = VOODOO_VERSION_STRUCT(D3D9);
        static const wchar_t * name_VSBindingDX9 = L"VSBindingDX9";
        static const Uuid clsid_VSBindingDX9 = CLSID_VSBindingDX9;

        const Version * VOODOO_CALLTYPE API_PluginInit(ICore * pCore)
        {
            UNREFERENCED_PARAMETER(pCore);

            return &moduleVersion;
        }

        const uint32_t VOODOO_CALLTYPE API_ClassCount()
        {
            return 1;
        }

        const wchar_t * VOODOO_CALLTYPE API_ClassInfo(const uint32_t index, Uuid * pUuid)
        {
            if (!pUuid)
            {
                return nullptr;
            }

            if (index == 0)
            {
                *pUuid = clsid_VSBindingDX9;
                return name_VSBindingDX9;
            }

            return nullptr;
        }

        IObject * VOODOO_CALLTYPE API_ClassCreate(const uint32_t number, ICore *core)
        {
            if (number == 0)
            {
                return new VSBindingDX9(core);
            }

            return nullptr;
        }
    }
}
