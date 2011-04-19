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

#include "Texture.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup VoodooCore
     * @{
     */
    /**
     * 
     */
    [
        coclass,
        progid("VoodooCore.Scalar.1"), vi_progid("VoodooCore.Scalar"), default(IVoodooScalar),
        uuid("F530DD21-9A1A-4F25-89B9-46E976ECCD64")
    ]
    class Scalar
        : public IVoodooScalar
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
        Scalar
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
        Scalar
        (
            _In_ Shader * parent, 
            _In_ CGparameter param
        );

        ~Scalar();

        // IUnknown
        STDMETHOD(QueryInterface)(REFIID riid, void ** ppvObj) PURE;
        STDMETHOD_(ULONG,AddRef)(void) PURE;
        STDMETHOD_(ULONG,Release)(void) PURE;

        // IVoodooObject
        /**
         * Retrieves the fully-qualified parameter name. Virtual parameters will be of the form 
         * <code>:param-name</code>, while effect parameters will use 
         * <code>shader-name:param-name</code>. If the parameter is no longer valid, the value 
         * "invalid-param" will be used instead (usually illegal as the '-' character cannot be 
         * used in an identifier). 
         * 
         * @returns The parameter's name.
         */
        STDMETHOD(GetName)(LPBSTR pName) PURE;

        STDMETHOD(GetCore)(IVoodooCore ** ppCore) PURE;

        // IVoodooParameter
        [propget, id(6)] STDMETHOD_(UINT, IsVirtual)() PURE;

        STDMETHOD(GetShader)(IVoodooShader ** ppShader) PURE;
        /**
         * Retrieves the type of this parameter. This specifies what type and how many data 
         * components are used (one texture or 1-16 floats).
         */
        STDMETHOD(GetParameterType)(ParameterType * pType) PURE;
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
        STDMETHOD(AttachParameter)(IVoodooScalar * pParameter) PURE;
        /**
         * Retrieves the underlying Cg parameter object.
         * 
         * @returns The Cg parameter this object is bound to.
         */
        STDMETHOD(GetCgParameter)(void ** ppCgParameter) PURE;

        STDMETHOD(Value)(SAFEARRAY * pData) PURE;
        STDMETHOD(Value)(SAFEARRAY ** ppData) PURE;

        STDMETHOD_(UINT, Components)() PURE;

    private:
        IVoodooShader * m_Shader;
        IVoodooCore * m_Core;
        CComBSTR m_Name;

        BOOL m_Virtual;

        CGparameter m_Parameter;
        ParameterType m_Type;
        CComSafeArray m_Value;
    };
    /**
     * @}
     */
}

#endif /*VOODOO_PARAMETER_HPP*/
