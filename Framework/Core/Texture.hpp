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
#include "IObject.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup VoodooCore
     * @{
     */

    /**
     * Contains a hardware texture, for use with Voodoo shaders and effects.
     * These textures may usually be bound to parameters and shaders can sample pixels
     * within them to create the on-screen images. Textures may be used to represent
     * various other surfaces, including render targets or even the backbuffer.
     */
    class VOODOO_API Texture
        : public IObject
    {
    public:
        Texture
        (
            _In_ String name, 
            _In_opt_ void * texture = nullptr
        );

        virtual ~Texture();

        virtual String GetName();

        virtual Core * GetCore();

        template<typename T>
        _Check_return_
        inline T * GetData()
        {
            return reinterpret_cast<T*>(GetData());
        };

        _Check_return_
        virtual void * GetData();

        virtual TextureDesc GetDesc();

    private:
        String m_Name;
        Core * m_Core;
        void * m_Data;
        TextureDesc m_Desc;
    };
    /**
     * @}
     */
}
