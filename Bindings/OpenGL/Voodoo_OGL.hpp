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

#include "mojoshader.h"
#include "GL/GL.h"

namespace VoodooShader
{
    namespace Voodoo_OGL
    {
        class VSBindingOGL;
        class VSEffectOGL;
        class VSParameterOGL;
        class VSPassOGL;
        class VSTechniqueOGL;
        class VSTextureOGL;

		typedef boost::intrusive_ptr<VSBindingOGL> VSBindingOGLRef;
		typedef boost::intrusive_ptr<VSEffectOGL> VSEffectOGLRef;
		typedef boost::intrusive_ptr<VSParameterOGL> VSParameterOGLRef;
		typedef boost::intrusive_ptr<VSPassOGL> VSPassOGLRef;
		typedef boost::intrusive_ptr<VSTechniqueOGL> VSTechniqueOGLRef;
		typedef boost::intrusive_ptr<VSTextureOGL> VSTextureOGLRef;

        const Version * VOODOO_CALLTYPE API_PluginInit(_In_ ICore * pCore);
        const uint32_t  VOODOO_CALLTYPE API_ClassCount();
        const wchar_t * VOODOO_CALLTYPE API_ClassInfo(_In_ const uint32_t index, _Out_ Uuid * pUuid);
        IObject *       VOODOO_CALLTYPE API_ClassCreate(_In_ const uint32_t index, _In_ ICore * pCore);
    }
}
