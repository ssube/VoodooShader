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

#ifndef VOODOO_PARAMETER_HPP
#define VOODOO_PARAMETER_HPP

#include "Meta.hpp"

namespace VoodooShader
{
    class VOODOO_API Parameter
    {
    public:
        /**
         * Virtual (global-level) parameter constructor. This will create a 
         * named parameter in the Cg runtime and register it in the given Core. 
         */
        Parameter
        (
            _In_ Core * core, 
            _In_ String name, 
            _In_ ParameterType type
        );

        Parameter
        (
            _In_ Shader * parent, 
            _In_ CGparameter param
        );

        String Name();

        CGparameter GetParameter();
        ParameterType GetType();

        /**
         * Binds two parameters, forcing their values to be identical. This 
         * should be used with care, as it has some requirements and 
         * restrictions. It is used to control some of the more common variables
         * found in shaders.
         *
         * @warning This <em>cannot</em> be used to bind one effect's parameter 
         *        to another. It can only be used to bind effect parameters to
         *        virtual or global parameters. If this is called on a non-virtual
         *        parameter, it will throw.
         */
        void Attach
        (
            _In_ ParameterRef param
        );

        void Set
        (
            _In_ TextureRef newTex
        );

        void Set
        (
            _In_ float newX
        );

        void Set
        (
            _In_ float newX, 
            _In_ float newY
        );

        void Set
        (
            _In_ float newX, 
            _In_ float newY, 
            _In_ float newZ
        );

        void Set
        (
            _In_ float newX, 
            _In_ float newY, 
            _In_ float newZ, 
            _In_ float newW
        );

        void Get
        (
            _Out_ TextureRef & param
        );

        void Get
        (
            _Out_ float & paramX
        );

        void Get
        (
            _Out_ float & paramX, 
            _Out_ float & paramY
        );

        void Get
        (
            _Out_ float & paramX, 
            _Out_ float & paramY, 
            _Out_ float & paramZ
        );

        void Get
        (
            _Out_ float & paramX, 
            _Out_ float & paramY, 
            _Out_ float & paramZ, 
            _Out_ float & paramW
        );

        inline TextureRef GetTexture()
        {
            return mValueTexture;
        };

        inline float * GetFloat()
        {
            return mValueFloat;
        };

        void ForceUpdate();

    private:
        Shader * mParent;
        Core * mCore;

        bool mVirtual;
        CGparameter mParam;
        ParameterType mType;

        // Value
        TextureRef mValueTexture;

        union
        {
            float mValueMatrix[4][4];
            float mValueFloat[16];
        };
    };
}

#endif /*VOODOO_PARAMETER_HPP*/
