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
#pragma once

#include "IObject.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup voodoo_framework_interfaces
     * @{
     */
    
    /**
     * @addtogroup voodoo_uuids 
     */
    DEFINE_IID(ITexture) = {0x96, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08};

    /**
     * Contains a hardware texture, for use with Voodoo shaders and effects. These textures may usually be bound to
     * parameters and shaders can sample pixels within them to create the on-screen images. Textures may be used to
     * represent various other surfaces, including render targets or even the backbuffer.
     *      
     * @par IID
     *      e6f31296-05af-11e1-9e05-005056c00008
     */
    class ITexture :
        public IObject
    {
    public:
        virtual ~ITexture(void) {};

        virtual uint32_t AddRef(void) const throw() = 0;
        virtual uint32_t Release(void) const throw() = 0;
        virtual bool QueryInterface(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const throw() = 0;
        virtual String ToString(void) const throw() = 0;
        virtual ICore * GetCore(void) const throw() = 0;

        virtual String GetName() const throw() = 0;

        /**
         * Gets the data.
         *
         * @return nullptr if it fails, else the data.
         */
        virtual void * GetData(void) const throw() = 0;

        /**
         * Set the texture data.
         *
         * @param pData The new data value.
         * @return The previous data value.
         */
        virtual void * SetData(_In_ void * pData) throw() = 0;

        /**
         * Gets the texture description.
         *
         * @return The description.
         */
        virtual const TextureDesc * GetDesc(void) const throw() = 0;
    };
    /**
     * @}
     */
}