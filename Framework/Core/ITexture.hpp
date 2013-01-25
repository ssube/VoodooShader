/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2013 by Sean Sube
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

#include "VoodooFramework.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup voodoo_interfaces
     * @{
     */
    /**
     * @class ITexture
     *
     * Contains a hardware texture, for use with Voodoo effects. These textures are usually bound to parameters and shaders 
     * can sample pixels from or draw to them to create the desired effects. Textures may also represent a depth texture,
     * with slightly different handling, or a null texture with no data storage. Textures may be created by calling
     * ICore::CreateTexture() or ICore::LoadTexture(), depending on the source. Textures are provided by the binding.
     *
     * @iid     e6f31296-05af-11e1-9e05-005056c00008
     * 
     * @note Textures have ITexture::StretchRect and ITexture::CopyVolume for transferring data. There is no specific method 
     *      for 1D textures, ITexture::StretchRect() should be used in this case, and the @a Y component set to 1 (other
     *      values will cause undefined behavior).
     */
    VOODOO_INTERFACE(ITexture, IResource, ({0x96, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08}))
    {
    public:
        /**
         * @name IObject Methods
         * @{
         */
        VOODOO_METHOD_(uint32_t, AddRef)() CONST PURE;
        VOODOO_METHOD_(uint32_t, Release)() CONST PURE;
        VOODOO_METHOD(QueryInterface)(_In_ CONST Uuid refid, _Outptr_result_maybenull_ IObject ** ppOut) PURE;
        VOODOO_METHOD_(String, ToString)() CONST PURE;
        VOODOO_METHOD_(ICore *, GetCore)() CONST PURE;
        /**
         * @}
         * @name IResource Methods
         * @{
         */
        VOODOO_METHOD_(String, GetName)() CONST;
        VOODOO_METHOD(GetProperty)(_In_ CONST Uuid propid, _Out_ Variant * pValue) CONST;
        VOODOO_METHOD(SetProperty)(_In_ CONST Uuid propid, _In_ Variant * pValue);
        /**
         * @}
         * @name ITexture Methods
         * @{
         */
        /**
         * Gets the texture description.
         */
        VOODOO_METHOD_(TextureDesc, GetDesc)() CONST PURE;
        /**
         * Binds this texture to a render-target on underlying API. This can only be done with textures that were created
         * with VSTexFlag_Target, other textures @a must fail regardless of API support.
         * 
         * @param   mode    The binding mode for this texture, whether it will be used as a source or target.
         * @param   index   The index to be bound to.
         * 
         * @note Every call to ITexture::Bind should have a corresponding call to ITexture::Reset.
         * 
         * @warning Binding a texture as both source and target is typically not possible <em>with the hardware API</em>. To
         *      allow this in most circumstances, Voodoo Shader may do some indirection within the binding. This is very
         *      likely to have a performance penalty, so it should be avoided.
         *      
         * @note If @a mode is VSTexMode_Target, @a index may be VOODOO_TEXTURE_DEPTH. This will bind the texture to the
         *      depth buffer. For this to work, the texture must be a depth format (VSFmt_D16, VSFmt_D24, VSFmt_D24S8,
         *      VSFmt_D32, or VSFmt_DMax). In some cases, these textures may also be bound as VSTexMode_Source.
         */
        VOODOO_METHOD(Bind)(_In_ CONST TextureMode mode, _In_ CONST uint32_t index);
        /**
         * Unbinds this texture from the render target, reseting the texture previously bound to that source or target.
         * 
         * @param   mode    The binding mode to reset. Since textures can be bound as source and target simultaneously, it
         *                  is possible to reset them separately.
         * @note This must only be called after ITexture::Bind, and will reset the value displaced by that call.
         */
        VOODOO_METHOD(Reset)(_In_ CONST TextureMode mode);
        /**
         * Copies a 1 or 2D region of texture data from another texture. 
         * 
         * @param   pSource The texture to copy from. This must be the same format with the same mipmap flags.
         * @param   source  The source rectangle.
         * @param   dest    The destination rectangle.
         * 
         * @note This method may do any stretching of the texture data. However, stretching may limit the possible allowed
         *      sources and destinations. 
         * 
         * @note If @a pSource has more dimensions than this texture, any more than this texture has must be disregarded.
         *      For example, if @a pSource is 3D and this is 2D, only the first "slice" will be copied. If @a pSource has
         *      fewer dimensions, the copy must fail. If any other behavior is needed, some form of render-to-texture should
         *      be used.
         * 
         * @warning Under some APIs, this call may require extra handling to copy from render target textures to others; 
         *      this may result in a notable performance hit. Because of this, avoid copying from render target textures.
         */
        VOODOO_METHOD(StretchRect)(_In_ ITexture * pSource, _In_ CONST Rect source, _In_ CONST Rect dest);
        /**
         * Copies a 3D region of texture data from another texture.
         * 
         * @param   pSource The texture to copy from. This must be the same format with the same mipmap flags.
         * @param   source  The source region.
         * @param   dest    The destination point.
         * 
         * @note This method does not do any stretching of the texture data.
         * 
         * @note @a pSource must have the same format and dimensions as this texture. If @a pSource has other dimensions, 
         *      this call must fail.
         */
        VOODOO_METHOD(CopyVolume)(_In_ ITexture * pSource, _In_ CONST Box source, _In_ CONST UInt3 dest);
        /**
         * @}
         */
    };
    /**
     * @}
     */
}
