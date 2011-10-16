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
#include "Utility/Reference.hpp"

#include "IObject.hpp"

namespace VoodooShader
{
    /**
    * @addtogroup VoodooCore @{
    */
    class IParameter :
        public IObject
    {
    public:
        /**
         * Virtual parameter (context-level) constructor. This will create a named parameter in the Cg runtime and register 
         * it in the given ICore, but the parameter will not exist in any effects. Virtual parameters are used to link many 
         * effect-level params to a single data source. 
         * 
         * @sa See the @ref virtualparams "parameter documentation" for details.
         */
        IParameter(_In_opt_ ICore * const pCore, _In_ String name, _In_ const ParameterType type);

        /**
        * Creates a parameter from an actual parameter (shader-level). This parameter will update values in the IShader, but 
        * is more limited in use. 
        * 
        * @sa See the @ref virtualparams "parameter documentation" for details
        */
        IParameter(_In_opt_ IShader * const pShader, _In_ const CGparameter pParam);

        virtual ~IParameter(void);

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
         * Retrieves the fully-qualified parameter name. Virtual parameters will be of the form <code>:param-name</code>, 
         * while effect parameters will use <code>shader-name:param-name</code>. If the parameter is no longer valid, the
         * value "invalid-param" will be used instead (usually illegal as the '-' character cannot be used in an identifier). 
         * 
         * @returns The parameter's name.
         */
        virtual String ToString(void) throw();

        /** 
         * Get the core this object was associated with. 
         * 
         * @return The core.
         */
        virtual ICore * GetCore(void) throw();

        /**
         * Retrieves the type of this parameter. This specifies what type and how many data components are used (one texture 
         * or 1-16 floats).
         */
        virtual ParameterType GetType(void);

        virtual Bool IsVirtual(void);

        /**
         * Attaches a second parameter to this one, forcing the other to update whenever this value is changed. 
         * 
         * @param pParam The parameter to bind to this one.
         * @return Success of the bind.
         * 
         * @warning This @e cannot be used to bind one effect's parameter to another. It can only be used to bind 
         *     actual parameters to virtual parameters. 
         *     
         */
        virtual Bool AttachParameter(_In_opt_ IParameter * const pParam);

        /**
         * Get the component count for this parameter.
         * 
         * @return The number of components.
         * 
         * @note This will have different meanings depending on parameter type:
         *     @li For a texture, this represents the number of dimensions (2 for texture2D).
         *     @li For a vector or matrix, this represents the raw number of components (2 for float2, 4 for float2x2, etc).
         *     @li For an array, this represents the number of elements in the array (2 for Light[2]).
         *     @li Structs and other types that do not also match one of the above will always return -1.
         */
        virtual Int32 GetComponents(void);

        /** 
         * Retrieves the texture source for this parameter. 
         * 
         * @return The texture source, if this parameter has a texture.
         */
        virtual ITexture * const GetTexture(void);

        virtual void SetTexture(_In_opt_ ITexture * const pTexture);

        /**
         * Retrieves the float buffer for this parameter. This contains all 16 float
         * components, for all sizes (float1 to float4x4). Any component may be written
         * to, but only the appropriate number will be sent to the Cg parameter.
         */
        _Ret_count_c_(16)
        virtual Float * const GetScalar(void);

        virtual void SetScalar(Int32 Count, _In_count_(Count) Float * const pValues);

        virtual IShader * const GetShader(void);

        /**
         * Retrieves the underlying Cg parameter object.
         * 
         * @returns The Cg parameter this object is bound to.
         */
        virtual CGparameter GetCgParameter(void);

    private:
        Int32 m_Refs;
        String m_Name;
        ICoreRef m_Core;

        IShaderRef m_Shader;

        bool m_Virtual;
        CGparameter m_Param;
        ParameterType m_Type;

        // Value
        ITextureRef m_ValueTexture;
        float m_ValueFloat[16];
    };
    /* @} */
}
