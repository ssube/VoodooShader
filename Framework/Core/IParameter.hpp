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
        virtual ~IParameter(void) throw() {};

        /**
         * Add a reference to this object.
         * 
         * @return The new reference count.
         */
        virtual uint32_t AddRef(void) const throw() = 0;

        /**
         * Release a reference from this object.
         * 
         * @return The new reference count.
         */
        virtual uint32_t Release(void) const throw() = 0;

        /**
         * Retrieves the fully-qualified parameter name. Virtual parameters will be of the form <code>:param-name</code>, 
         * while effect parameters will use <code>shader-name:param-name</code>. If the parameter is no longer valid, the
         * value "invalid-param" will be used instead (usually illegal as the '-' character cannot be used in an identifier). 
         * 
         * @returns The parameter's name.
         */
        virtual String ToString(void) const throw() = 0;

        /** 
         * Get the core this object was associated with. 
         * 
         * @return The core.
         */
        virtual ICore * GetCore(void) const throw() = 0;

        virtual String GetName() const throw() = 0;

        /**
         * Retrieves the type of this parameter. This specifies what type and how many data components are used (one texture 
         * or 1-16 floats).
         */
        virtual ParameterType GetType(void) const throw() = 0;

        /** 
         * Checks if the parameter is virtual (belongs to the core only) or physical (from a shader).
         *
         * @return Virtual status.
         */
        virtual bool IsVirtual(void) const throw() = 0;

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
        virtual bool AttachParameter(_In_opt_ IParameter * const pParam) throw() = 0;

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
        virtual const uint32_t GetComponents(void) const throw() = 0;

        /** 
         * Retrieves the texture source for this parameter. 
         * 
         * @return The texture source, if this parameter has a texture.
         */
        virtual ITexture * GetTexture(void) const throw() = 0;

        virtual void SetTexture(_In_opt_ ITexture * const pTexture) throw() = 0;

        /**
         * Retrieves the float buffer for this parameter. This contains all 16 float
         * components, for all sizes (float1 to float4x4). Any component may be written
         * to, but only the appropriate number will be sent to the Cg parameter.
         */
        _Ret_count_c_(16) virtual float * const GetScalar(void) throw() = 0;

        virtual void SetScalar(_In_ const uint32_t count, _In_count_(Count) float * const pValues) throw() = 0;

        virtual IShader * const GetShader(void) const throw() = 0;

        /**
         * Retrieves the underlying Cg parameter object.
         * 
         * @returns The Cg parameter this object is bound to.
         */
        virtual CGparameter GetCgParameter(void) const throw() = 0;
    };
    /* @} */
}
