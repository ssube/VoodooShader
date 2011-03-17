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
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; 
 * if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301 US
 *
 * Support and more information may be found at http://www.voodooshader.com, or by contacting the
 * developer at peachykeen@voodooshader.com
\**************************************************************************************************/
  
/**
 *
 * @page Shaders
 * <p>
 *    Voodoo shaders are split into a number of levels with a tree structure.
 * </p>
 * <p>
 *    Each shader may have one or more techniques. A technique describes a full
 *    rendering process, and may have one or more passes. Each pass describes a
 *    particular way to render the given geometry, and may have multiple programs.
 *    Each program controls a particular part of the render process. In addition,
 *    shaders, techniques and passes contain some metadata used by the system to
 *    control how and where they are rendered.
 * </p>
 *
 * @section programstages Program Stages
 * <p>
 *    Each program is designed to control a very specific stage of the render
 *    pipeline.
 * </p>
 * <p>
 *    The two most common stages, vertex and fragment, work with existing data and
 *    transform it. Both have been supported since DirectX 8 in some form, and
 *    most systems today support both. Given Voodoo's focus, it is safe to assume
 *    both stages are supported (the exact level of support may vary, however).
 * </p>
 * <p>
 *  The vertex stage handles each vertex, transforming them into view space and 
 *    sometimes calculating lighting or surface normals. 
 * </p>
 * <p>
 *    The fragment stage handles each pixel (or fragment) as it is rendered. The
 *    fragment program handles lighting in per-pixel lighting solutions, as well
 *    applying and blending texture stages, blending vertex colors, and any other
 *    operations that may vary for each pixel rendered.
 * </p>
 * <p> 
 *    The geometry stage has the opportunity to create vertexes if necessary and 
 *    can build geometry dynamically. This is often used with a form of render-to-
 *    vertex buffer, to generate and store the geometry for later use. Geometry
 *    stages will often depend heavily on parameters passed into the effect. 
 *    Geometry stages are only supported on DirectX 10 and higher (roughly OpenGL
 *    2.0).
 * </p>
 * @todo Verify what OGL version supports geometry shaders
 * <p>
 *    Most recently, two tessellation control stages have been added. While these
 *    are supported in DirectX 11 and higher, and so are not wide spread, they
 *    provide detailed control over geometry and can manipulate or create it
 *    at runtime. They have some similarities to the geometry stage, but provide
 *    more control over the hardware tessellation process.
 * <p>
 * @note Voodoo does not yet support these shader stages, mostly because I
 *        don't have access to DX11 hardware. Geometry stages are supported, and
 *        should work, but aren't terribly useful (older games can't use them).
 *        The vertex and pixel stages are the most used, by far.
 *
 * @section shadersystem Shader System
 * <p>
 *    In addition to the standard Cg compiler, Voodoo adds a few additional
 *    features to the shader system.
 * </p>
 * @subsection shaderparameters Shader Parameters
 * <p>
 *    Voodoo provides a system to link effect-level parameters to application or
 *    system-level variables. This provides an easy way to provide data to shaders
 *    and is capable of linking many parameters from various effects to the same
 *    global parameter.
 * </p>
 * <p>
 *    Effect parameters may only be linked to global parameters. Global parameters
 *    are created by calling Core::CreateParameter(), then any number of other
 *    parameters may be linked by calling Parameter::Attach() on the global
 *    parameter and providing the effect parameter, like so:<br />
 *    <code>
 *    ParameterRef global = Core->CreateParameter("time", PT_Float);<br />
 *    ParameterRef effect = Shader->GetParameter("time");<br />
 *    global->Attach(effect);<br />
 *    </code>
 *    Whenever the global parameter is set to a new value, all attached parameters
 *    will also be updated.
 * </p>
 * <p>
 *    This mechanism is useful for things like system time, common matrices,
 *    random values or other data that changes each frame but not within the
 *    frame.
 * </p>
 * @subsection shadertargets Shader Targets
 * <p>
 *    Voodoo also provides a system to specify a target for each technique or
 *    pass. This provides an easy way to setup render to texture effects.
 * </p>
 * <p>
 *    Each shader technique and pass may have a target annotation, which gives
 *    the name of a texture. The results of the technique or pass, usually sent
 *    to the <code>lastshader</code> or <code>lastpass</code> texture, will
 *    instead go to the specified target. Targets may be specified like so:<br />
 *    <code>
 *    technique dx9_high<br />
 *        < string target=":buffer_group1"; ><br />
 *    {<br />
 *        pass diffuse<br />
 *            < string target=":buffer_diffuse"; ><br />
 *        {<br />
 *            ...<br />
 *        }<br />
 *    }<br />
 *    </code>
 *    This allows pass and technique output to be stored and later used, by other
 *    passes from the same effect or even by other effects.
 * </p>
 * <p>
 *    If no target is specified in the technique or pass annotations, the shader
 *    linker uses the texture currently bound to the @ref VoodooShader::Core 
 *    "Core's" special textures. These may be set using 
 *    VoodooShader::Core::SetTexture(TextureType, TextureRef). Relinking a shader 
 *    may change these targets.
 * </p>
 *
 * @page Textures
 * <p>
 *    The shaders used in Voodoo are capable of accessing a number of texture
 *    types, and Voodoo will be able to load a number of texture formats. This
 *    provides a very powerful system and allows you to use a good variety of
 *    textures.
 * </p>
 * 
 * @section texturetypes Texture Types
 * <p>
 *    Three basic texture types are supported in Voodoo:
 *    <ol>
 *     <li>1-dimensional textures (a simple row of pixels, 1 by x)</li>
 *     <li>2-dimensional textures (the most common scene, a rectangle of pixels,
 *        x by y)</li>
 *     <li>3-dimensional textures (effectively a number of 2D textures stacked in
 *        slices, x by y by z)</li>
 *    </ol>
 * </p>
 * <p>
 *    A different sampler type is provided for each texture type (
 *    <code>sampler1D</code>, <code>sampler2D</code>, and <code>sampler3D</code>),
 *    each optimized to sample pixels from a particular texture type.
 * </p>
 * <p>
 *    Samplers require texture coordinates when used, and each type of texture
 *    requires a different number of coordinates (one for each dimension).
 * </p>
 *
 * @section textureformats Texture Formats
 * <p>
 *    Voodoo supports a decent sampling of internal texture formats. These formats
 *    specify how much data a texture holds and how it is stored. There are some
 *    limitations and some potentially useful formats are not yet supported, due
 *    to the differences between DirectX and OpenGL, but many common formats are
 *    available.
 * </p>
 * <p>
 *    All the listed formats are usable as a 2D texture, and with the exception
 *    of the depth formats, should be usable as 1D or 3D textures as well. For 
 *    most of these formats, DirectX and OpenGL equivalents are indicated below, 
 *    along with a notation indicating if the texture may be used as a 
 *    render target or copied to and from.
 * </p>
 *
 * <table>
 *    <tr><th>Voodoo Format</th>        <th>DirectX Format</th>        
 *        <th>OpenGL Format</th>        <th>RT/Copy</th>
 *        <th>Bits</th>                 <th>Channels</th></tr>
 *    <tr><td>TF_Unknown</td>           <td>D3DFMT_UNKNOWN</td>        
 *        <td>(unknown)</td>            <td>No</td></tr>
 *    <tr><td>TF_RGB5</td>              <td>D3DFMT_X1R5G5B5</td>    
 *        <td>GL_RGB5</td>              <td>Yes</td>
 *        <td>15/16</td>                <td>RGB</td></tr>
 *    <tr><td>TF_RGB5A1</td>            <td>D3DFMT_A1R5G5B5</td>    
 *        <td>GL_RGB5_A1</td>           <td>Yes</td>
 *        <td>16</td>                   <td>RGBA</td></tr>
 *    <tr><td>TF_RGB8</td>              <td>D3DFMT_X8R8G8B8</td>    
 *        <td>GL_RGB8</td>              <td>Yes</td>
 *        <td>24</td>                   <td>RGB</td></tr>
 *    <tr><td>TF_RGBA8</td>             <td>D3DFMT_A8R8G8B8</td>    
 *        <td>GL_RGBA8</td>             <td>Yes</td>
 *        <td>32</td>                   <td>RGBA</td></tr>
 *    <tr><td>TF_RGB10A2</td>           <td>D3DFMT_A2R10G10B10</td>    
 *        <td>GL_RGB10_A2</td>          <td>Yes</td>
 *        <td>32</td>                   <td>RGBA</td></tr>
 *    <tr><td>TF_RGBA16F<sup>1</sup></td><td>D3DFMT_A16B16G16R16F</td>
 *        <td>GL_RGBA16F</td>           <td>N/Y</td>
 *        <td>64</td>                   <td>RGBA</td></tr>
 *    <tr><td>TF_RGBA32F<sup>1</sup></td><td>D3DFMT_A32B32G32R32F</td>
 *        <td>GL_RGBA32F</td>           <td>N/Y</td>
 *        <td>128</td>                  <td>RGBA</td></tr>
 *    <tr><td>TF_D16F</td>              <td>D3DFMT_D16F</td>    
 *        <td>GL_DEPTH_COMPONENT16</td> <td>N/Y-ish<sup>2</sup></td>
 *        <td>16</td>                   <td>R<sup>3</sup></td></tr>
 *    <tr><td>TF_D32F</td>              <td>D3DFMT_D32F</td>
 *        <td>GL_DEPTH_COMPONENT32</td> <td>N/Y-ish<sup>2</sup></td>
 *        <td>32</td>                   <td>R<sup>3</sup></td></tr>
 * </table>
 *  <em><sup>1</sup>:</em>
 *     Floating-point texture formats (RGBA16F and RGBA32F) may not be supported 
 *     on all hardware.
 *     Any features requiring these formats should provide an alternate render 
 *     path for old hardware (quality may suffer).
 * <br />
 * <em><sup>2</sup>:</em>
 *     Depth textures suffer additional restrictions when used with the DirectX 9 
 *     API.
 *     See the @ref depthbuffers "depth buffers" section for more information.
 * <br />
 * <em><sup>3</sup>:</em>
 *     Depth textures do not map to components in all APIs. The most likely
 *     mapping is given, but access is not guaranteed.
 * <br />
 *
 * @section depthbuffers Depth Buffers
 * <p>
 *    For an adapter to be considered Voodoo-compatible, it must supply realtime
 *    depth data to a shader-readable texture. How this is performed is left to
 *    the discretion of the adapter, considering how obnoxious it can be under
 *    DirectX 9. To work around the DX9 issues, most Voodoo adapters will use a
 *    two-pass or two-target rendering method with an adapter-specific shader.
 *    The two-pass method takes more time but is compatible on all systems, the
 *    two-target method requires rendering the geometry once, but may not always
 *    be supported (DX9 has some issues with multitarget rendering and multisampling).
 * </p>
 * <p>
 *    When possible, it is recommended that the depth information be stored in a
 *    32-bit floating point component. Depth data must be accessible to shaders
 *    from the R channel of the depth texture. At this time, the GBA components of
 *  the texture are considered undefined, but reserved for future use. Depending
 *  on implementation, they may contain depth data, uninitialized memory, or
 *  other data. They should not be used.
 * </p>
 * <p>
 *    As DirectX 9 does not usually allow D32F textures to be read, this may 
 *    require rendering to a second target and using a RGBA32F texture. At this
 *  point, the GBA components are not used, but future versions of Voodoo may
 *  pack additional data into them.
 * </p>
 *
 */