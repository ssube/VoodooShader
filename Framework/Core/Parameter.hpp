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
#include "IObject.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup VoodooCore
     * @{
     */
    /**
     *
     */
    class Parameter
                : public IObject
    {
    public:
        /**
         * Virtual parameter (context-level) constructor. This will create a named parameter in the
         * Cg runtime and register it in the given Core, but the parameter will not exist in any
         * effects. Virtual parameters are used to link many effect-level params to a single data
         * source.
         *
         * @sa See the @ref virtualparams "parameter documentation" for details.
         */
        Parameter
        (
            _In_ Core * core,
            _In_ String name,
            _In_ ParameterType type
        );

        /**
         * Creates a parameter from an actual parameter (shader-level). This parameter will update
         * values in the Shader, but is more limited in use.
         *
         * @sa See the @ref virtualparams "parameter documentation" for details.
         */
        Parameter
        (
            _In_ Shader * parent,
            _In_ CGparameter param
        );

        virtual ~Parameter();

        /**
         * Retrieves the fully-qualified parameter name. Virtual parameters will be of the form
         * <code>:param-name</code>, while effect parameters will use
         * <code>shader-name:param-name</code>. If the parameter is no longer valid, the value
         * "invalid-param" will be used instead (usually illegal as the '-' character cannot be
         * used in an identifier).
         *
         * @returns The parameter's name.
         */
        virtual String GetName();

        virtual Core * GetCore();

        /**
         * Retrieves the type of this parameter. This specifies what type and how many data
         * components are used (one texture or 1-16 floats).
         */
        virtual ParameterType GetType();

        virtual bool IsVirtual();

        /**
         * Attaches a second parameter to this one, forcing the other to update whenever this value
         * is changed.
         *
         * @warning This <em>cannot</em> be used to bind one effect's parameter to another. It can
         *    only be used to bind actual parameters to virtual parameters.
         *
         * @param param The parameter to bind to this one.
         * @throws Exception if called on an actual parameter.
         */
        virtual void AttachParameter
        (
            _In_ ParameterRef param
        );

        virtual int GetComponents();

        /**
         * Retrieves the texture source for this parameter.
         */
        virtual TextureRef GetTexture();
        virtual void SetTexture(TextureRef Texture);

        /**
         * Retrieves the float buffer for this parameter. This contains all 16 float components, for
         * all sizes (float1 to float4x4). Any component may be written to, but only the appropriate
         * number will be sent to the Cg parameter.
         */
        _Ret_count_c_(16)
        virtual float * GetScalar();

        virtual void SetScalar(int Count, _In_count_(Count) float * Values);

        virtual ShaderRef GetShader();

        /**
         * Retrieves the underlying Cg parameter object.
         *
         * @returns The Cg parameter this object is bound to.
         */
        virtual CGparameter GetCgParameter();

    private:
        String m_Name;
        Core * m_Core;

        ShaderRef m_Parent;

        bool m_Virtual;
        CGparameter m_Param;
        ParameterType m_Type;

        // Value
        TextureRef m_ValueTexture;
        float m_ValueFloat[16];
    };
    /**
     * @}
     */
}

#endif /*VOODOO_PARAMETER_HPP*/
