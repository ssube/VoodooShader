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

#include "Voodoo_OGL.hpp"
#include "OGL_Version.hpp"
#include "VSBindingOGL.hpp"

namespace VoodooShader
{
    namespace Voodoo_OGL
    {
        static const Version moduleVersion = VOODOO_VERSION_STRUCT(OGL);
        static const wchar_t * name_VSBindingOGL = L"VSBindingOGL";
        static const Uuid clsid_VSBindingOGL = CLSID_VSBindingOGL;

        const Version * VOODOO_CALLTYPE API_PluginInit(_In_ ICore * pCore)
        {
            UNREFERENCED_PARAMETER(pCore);

            return &moduleVersion;
        }

        void VOODOO_CALLTYPE API_PluginReset(_In_ ICore * pCore)
        {
            UNREFERENCED_PARAMETER(pCore);
        }

        const uint32_t VOODOO_CALLTYPE API_ClassCount()
        {
            return 1;
        }

        const wchar_t * VOODOO_CALLTYPE API_ClassInfo(_In_ const uint32_t index, _Out_ Uuid * pUuid)
        {
            if (!pUuid)
            {
                return nullptr;
            }

            if (index == 0)
            {
                *pUuid = clsid_VSBindingOGL;
                return name_VSBindingOGL;
            }

			*pUuid = NilUuid;
            return nullptr;
        }

        IObject * VOODOO_CALLTYPE API_ClassCreate(_In_ const uint32_t number, _In_ ICore * pCore)
        {
			if (!pCore)
			{
				return nullptr;
			}

            if (number == 0)
            {
                return new VSBindingOGL(pCore);
            }

            return nullptr;
        }
    }
}
