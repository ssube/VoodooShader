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

#define VOODOO_IMPORT
#define VOODOO_NO_PUGIXML
#include "VoodooFramework.hpp"

#pragma warning(push,3)
#   ifdef _DEBUG
#       define D3D_DEBUG_INFO
#   endif
#   include <d3d9.h>
#   include <d3dx9effect.h>
#pragma warning(pop)

namespace VoodooShader
{
    namespace Voodoo_D3D9
    {
        class VSBindingDX9;
        class VSEffectDX9;
        class VSParameterDX9;
        class VSPassDX9;
        class VSTechniqueDX9;
        class VSTextureDX9;

		typedef boost::intrusive_ptr<VSBindingDX9> VSBindingDX9Ref;
		typedef boost::intrusive_ptr<VSEffectDX9> VSEffectDX9Ref;
		typedef boost::intrusive_ptr<VSParameterDX9> VSParameterDX9Ref;
		typedef boost::intrusive_ptr<VSPassDX9> VSPassDX9Ref;
		typedef boost::intrusive_ptr<VSTechniqueDX9> VSTechniqueDX9Ref;
		typedef boost::intrusive_ptr<VSTextureDX9> VSTextureDX9Ref;

        const Version * VOODOO_CALLTYPE API_PluginInit(_In_ ICore * pCore);
        const uint32_t  VOODOO_CALLTYPE API_ClassCount();
        const wchar_t * VOODOO_CALLTYPE API_ClassInfo(_In_ const uint32_t index, _Out_ Uuid * pUuid);
        IObject *       VOODOO_CALLTYPE API_ClassCreate(_In_ const uint32_t index, _In_ ICore * pCore);
    }
}
