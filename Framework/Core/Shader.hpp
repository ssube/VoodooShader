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

#include "Common.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup VoodooCore
     * @{
     */

    /**
     * Complete shader class, managing techniques, passes and metadata. Shaders
     * contain significant linking and processing data, as well as the Voodoo-specific
     * linking stage. Shader file loading and compilation are handled internally,
     * the shader can be constructed from a file location or existing Cg effect
     * object.
     * 
     * @note    Voodoo @ref Shader Shaders correspond to DirectX effects or a group of OpenGL 
     *    programs.
     * @note    A shader may contain multiple @ref Technique techniques. See the
     *    Technique class for more information. Shader techniques must be
     *    separately validated, only techniques valid on the current hardware
     *    will be usable in the shader.
     * @warning For any technique in a shader to be valid, the entire shader must
     *    compile without errors. This <em>does not</em> mean that all techniques
     *    will be valid.
     */
    [
        coclass, noncreatable,
        vi_progid("VoodooCore.Shader"), progid("VoodooCore.Shader.1"), default(IVoodooShader),
        uuid("FC7FA3C9-DCB3-47FC-9645-80F4A96D2E51")
    ]
    class Shader
        : public IVoodooShader
    {
    public:
        Shader();

        /**
         * Destroys the shader, unlinking and cleaning up the effect and all techniques.
         */
        ~Shader();
        
        /**
         * Create a shader from a file, passing options to the compiler.
         * 
         * @param parent The Core this Shader is created under.
         * @param filename The file to use for source.
         * @param args Optionals arguments to be passed to the compiler (last item in the array must be null).
         */
        static Shader * Create(IVoodooCore * pCore, IVoodooFile * pFile);

        // IVoodooObject
        /**
         * Return a simple name (usually filename of source file) for this shader.
         */
        STDMETHOD(GetName)(LPBSTR pName);
        STDMETHOD(GetCore)(IVoodooCore ** ppCore);

        // IVoodooShader
        STDMETHOD_(UINT, get_TechniqueCount)();
        /**
         * Retrieve a technique from the shader by name. Most cases should use
         * the default technique, but some specific applications may want a
         * particular technique. 
         *
         * @param name The technique name to find.
         * @return A shared reference to the technique if it is found, an empty
         *        shared reference otherwise.
         */
        STDMETHOD(GetTechnique)(UINT Number, IVoodooTechnique ** ppTechnique);
        /**
         * Retrieves the default technique from this shader. All drawing should
         * be done with the default technique: it is guaranteed to be valid for
         * the current shader profiles and API. The default technique is the
         * first technique to validate.
         *
         * @return A reference to the default technique.
         * 
         * @note To influence the priority of techniques, simply place the most
         *        specific or least compatible first (often high-quality ones) and
         *        count down with the most compatible and simplest technique last.
         *        The first valid technique found becomes the default.
         */
        STDMETHOD(get_DefaultTechnique)(IVoodooTechnique ** ppTechnique);
        /**
         * Set a technique from this shader to be used as the default technique.
         * Some adapter functions simply retrieve the default technique to draw
         * with. 
         * 
         * @param name The name of a technique within this shader to use.
         *
         * @note This takes a name and not a TechniqueRef to prevent cross-
         *        linking shaders (which would be a Bad Thing, at best).
         * 
         * @throws Exception if the name is not a valid technique.
         */
        STDMETHOD(put_DefaultTechnique)(IVoodooTechnique * pTechnique);
        /**
         * Retrieve the number of effect-level parameters in this shader. These
         * hold a single value for all passes.
         * 
         * @return The parameter count.
         */
        STDMETHOD_(UINT, get_ParameterCount)();
        /**
         * Retrieve a specific parameter from the shader. These may be modified
         * at runtime and will automatically update Cg and the GPU with their
         * value (in most cases).
         *
         * @param index The index of the parameter to retrieve.
         * @return A shared reference to the parameter, or to NULL if it is not
         *        found (the index is invalid).
         */
        STDMETHOD(GetParameter)(UINT Number, IVoodooParameter ** ppParameter);
        /**
         * Retrieve the underlying Cg technique.
         * 
         * @return A pointer to the Cg technique.
         */
        STDMETHOD(GetCgShader)(void ** ppCgShader);

    private:
        STDMETHOD(Link)();

        /**
         * Link a particular effect-level parameter against various core
         * elements (exact behavior depends on param type).
         *
         * @param param The parameter to link.
         */
        STDMETHOD(LinkParameter)(IVoodooParameter * pParameter);

        /**
         * Links a particular effect-level sampler against a core texture.
         * This generally should be called by Shader::LinkParameter().
         * 
         * @param param The sampler to link.
         */
        STDMETHOD(LinkSampler)(IVoodooParameter * pSampler);

        /**
         * Find texture information from a parameter and create a texture
         * based on that data. Calls Shader::LinkSampler() after texture
         * creation if appropriate.
         * 
         * @param param The parameter to use.
         */
        STDMETHOD(CreateParameterTexture)(IVoodooParameter * pParameter);

        STDMETHOD(SetupTechniques)();

        UINT m_Refrs;
        IVoodooCore * m_Core;
        CComBSTR m_Name;
        CGeffect m_Shader;

        IVoodooTechnique * m_DefaultTechnique;
        CArray<CComPtr<IVoodooTechnique>> m_Techniques;
        CArray<CComPtr<IVoodooParameter>> m_Parameters;
    };
    /**
     * @}
     */
}
