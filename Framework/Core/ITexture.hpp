/**
 * This file is part of the Voodoo IShader Framework, a comprehensive shader support library. 
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
 */
#pragma once

#include "Includes.hpp"

#include "IObject.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup VoodooCore @{
     */

    /**
     * Contains a hardware texture, for use with Voodoo shaders and effects. These textures may usually be bound to
     * parameters and shaders can sample pixels within them to create the on-screen images. Textures may be used to
     * represent various other surfaces, including render targets or even the backbuffer.
     */
    class ITexture :
        public IObject
    {
    public:
        ITexture(_In_ String name, _In_opt_ void * texture = nullptr);

        virtual ~ITexture(void);

        /**
         * Add a reference to this object.
         * 
         * @return The new reference count.
         */
        virtual Int32 AddRef(void) throw();

        /**
         * Release a reference from this object.
         * 
         * @return The new reference count.
         */
        virtual Int32 Release(void) throw();

        /** 
         * Gets the name.
         * 
         * @return The name. 
         */
        virtual String ToString(void);

        virtual ICore * GetCore(void);

        template<typename T>
        _Check_return_ 
        inline T * GetData(void) { return reinterpret_cast<T *>(GetData()); };

        /** 
         * Gets the data. 
         * 
         * @return nullptr if it fails, else the data. 
         */
        _Check_return_ virtual void * GetData(void);

        /**
         * Gets the texture description. 
         * 
         * @return The description.
         */ 
        virtual TextureDesc GetDesc(void);

    private:
        Int32 m_Refs;
        String m_Name;
        ICoreRef m_Core;
        void * m_Data;
        TextureDesc m_Desc;
    };
    /* @} */
}
