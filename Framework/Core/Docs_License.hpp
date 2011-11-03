/**
 * This file is part of the Voodoo Shader Framework. 
 * 
 * Copyright (c) 2010-2011 by Sean Sube 
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

namespace VoodooShader
{
    /**
     * @mainpage Voodoo Shader Framework 
     * The Voodoo Shader Framework is a set of support libraries designed to provide extensive shader integration into 
     * graphics applications, both before and after development.
     *
     * Voodoo uses a system of core modules and adapters to provide the widest compatibility and best performance possible
     * across a wide range of applications and APIs. Voodoo supports both OpenGL and DirectX, using nVidia's Cg language. 
     * In addition, Voodoo provides a second processing step to provide shaders with easy access to other resources, 
     * especially textures.
     *
     * The Voodoo core and supported shaders remain constant between target applications and adapters, providing a unified
     * set of features and single target for shaders.
     *
     * @section voodoo_main_shaders Shader Support
     * 
     * Voodoo uses nVidia's Cg Toolkit to provide a common, simple shader language and cross-API compiler for widely 
     * supported shaders. Voodoo shaders run under both OpenGL and DirectX and can scale over a great variety of hardware,
     * from OpenGL 1.1 to 4.0 and DirectX 8 to 11. Applications using any of the supported APIs are supported within Voodoo, 
     * with a near-identical feature set provided across the board. 
     *
     * In addition to fully supporting the Cg language, Voodoo implements a second, internal linking stage to offer more 
     * powerful features such as deferred rendering and material shaders, with or without original application support.
     * Voodoo uses features such as semantics and annotations to provide these features without adding complexity to the 
     * language or breaking compatibility with other shader tools. Voodoo provides a set of compiler options to allow 
     * shaders to conditionally compile various levels of support. Voodoo fully supports techniques and passes within a 
     * shader, as well as various program stages.
     *
     * Access to hardware textures and render information is provided through application-specific adapters, allowing Voodoo
     * to be tied tightly into a huge number of new and existing programs. 
     *
     * @section voodoo_main_supportlibs Support Libraries 
     * 
     * Voodoo provides a number of support libraries, many used within Voodoo. These include the Voodoo dynamic loader 
     * (capable to dynamically loading the Voodoo core and an adapter into a running process), a simple file system manager 
     * to streamline resource loading, EasyHook-based function intercept library, comprehensive Unicode string handling, and
     * other useful code. These libraries are available for use by code within and outside of the Voodoo project and
     * simplify a number of common or complex tasks. 
     *
     * @section voodoo_main_quality Testing and Code Quality 
     *
     * The Voodoo framework covers an immense scope and tackles an intriguing and often complex task. To maximize stability, 
     * speed and code quality, all code within the Voodoo Shader Framework must meet @ref voodoo_quality "a set of
     * quality-control standards" and be fully tested before release. Tests are carried out within specialized test suits
     * and target applications. 
     *
     * @section voodoo_main_logs Logs and Debugging 
     *
     * In addition to code testing, Voodoo provides logging libraries to record all actions in detail. The verbosity of the 
     * log can be customized, with the highest levels recording API calls and their parameters. Errors and warnings
     * originating within Voodoo code are always logged, and safeguards are put in place to make sure log data is written to
     * disk, even in the event of a fatal error or loss of power. 
     *
     * Debug builds of Voodoo are available, as is source code, to aid debugging. Debug builds are built with full symbols
     * and additional error checking. 
     * 
     * Voodoo is also fully compatible with your favorite graphics debugger, including PIX for Windows (DirectX) and 
     * GLintercept (OpenGL). This also extends to screen capture programs and system tools, and code debuggers. In some 
     * cases, Voodoo's logging may be able to augment the information from these tools.
     *
     * On the Github project, a tracker for bugs and features is available and open to all users to submit bugs and logs.
     *
     * @section voodoo_main_docs Documentation
     *
     * The entire Voodoo project is heavily documented using doxygen notation. These docs are compiled and uploaded along 
     * with each code push and release. Everything from the variable syntax to texture load formats to the adapter interface
     * are documented in depth, cross-referenced and any potential issues are noted. The documentation is interwoven in the 
     * source, so it is kept up-to-date and extensive and new code is documented as it's written. 
     * 
     * @page voodoo_license License 
     *
     * @section voodoo_license_summary Summary 
     * 
     * The Voodoo Shader Framework is licensed under the terms of the @ref voodoo_license_terms_lgpl3. This is in an effort
     * to make it useful to as many people as possible. Some additional modules may use different license terms, the terms 
     * below apply only to the core library. Please contact the relevant developer(s) with any questions you may have. 
     * 
     * While being licensed under the LGPL v3, it should be made clear that linking against the various Voodoo interface
     * headers and utility classes do not make an application a derived work (a clarification of section 3 of the LPGL).
     * This means that applications not licensed under the (L)GPL, including commercial ones, may implement these interfaces
     * without being restricted under this license. This is intended primarily to allow commercial applications to use
     * Voodoo; while it technically permits adapters and modules be sold, I would ask this be discouraged and as much of
     * Voodoo be kept free as possible.
     * 
     * @subsection voodoo_license_summary_eula Application EULAs 
     *
     * The Voodoo Shader Framework is not a derivative work of any other work, nor is it bound under the terms of an other
     * application's EULA or other license restrictions. The framework is licensed solely under the terms of the GNU Lesser
     * General Public License, version 3, and any claims otherwise are false and unlawful. 
     * 
     * @note Portions of the Voodoo Shader Framework may be designed to operate aware of other applications. These
     *      applications may be licensed under different terms than the framework. Claims have been made that Voodoo is then
     *      bound by the terms of these target applications, is owned by the author of these applications, or has no license 
     *      terms. These are not true and do not effect the terms given in the license document at all; Voodoo is released
     *      under the LGPL and unless written permission is gained, these terms are legally binding and must be followed.
     *
     * @subsection voodoo_license_summary_contact Contact 
     * 
     * Questions about the Voodoo framework and its license terms should be directed to Sean Sube, at the email address
     * <a href="mailto:peachykeen@voodooshader.com">peachykeen@voodooshader.com</a>. Questions about other modules and their 
     * terms should be directed to the appropriate developers. Please check any documentation included in the modules for 
     * contact information.
     *
     * @subsection voodoo_license_summary_trademark Trademarks and other property 
     * 
     * Any trademarks or company names used in this documentation or the Voodoo framework are the property of their 
     * respective owner(s). This includes nVidia, Cg, Windows and any other such content. 
     *
     * @section voodoo_license_pugixml pugixml License 
     * 
     * The Voodoo Shader Framework uses the <a href="http://pugixml.org/">pugixml library</a> to handle XML 
     * parsing and data retrieval. This library is used under the terms of the MIT license, detailed at 
     * @ref voodoo_license_pugixml_terms. 
     * 
     * Thanks are given to Arseny Kapoulkine and all others involved for this excellent library, it has made config files a 
     * @em lot easier to work with and the API is very nice.

     * @section voodoo_license_easyhook EasyHook License 
     * 
     * The Voodoo Shader Framework uses the <a href="http://easyhook.codeplex.com/">EasyHook library</a> to handle function
     * hooking and intercept mechanisms. These provide a fundamental part of how Voodoo works, and the project would be much
     * more difficult without this library. 
     * 
     * EasyHook is used under the terms of the GNU Lesser GPL, version 3. This is the same license as the Voodoo framework,
     * so the terms are included once, later in this document. Please see @ref voodoo_license_terms_lgpl3 for details.
     *
     * @section voodoo_license_boost Boost Software License
     * 
     * The Voodoo Shader Framework uses portions of the <a href="http://www.boost.org">Boost C++ Libraries</a> in a number
     * of areas. Libraries used include Lexical Cast, Smart Pointers, String Algorithms, and Uuid. Thanks to the authors
     * of these libraries, and the Boost project as a whole. These libraries are used under the terms of the 
     * @ref voodoo_license_terms_boost.
     * 
     * @section voodoo_license_glew GLEW License
     * 
     * The Voodoo Shader Framework uses The OpenGL Extension Wrangler Library in some components. This library is used under
     * the terms included in @ref voodoo_license_terms_glew.
     * 
     * @section voodoo_license_cg Cg Framework License 
     *
     * The Voodoo Shader Framework uses nVidia's Cg Toolkit for shader compiling and handling. This library is used under 
     * the terms given in the @ref voodoo_license_terms_cg. 
     * 
     * Voodoo would not really be possible without Cg, so many thanks are given to the Cg team for this library. 
     *
     * @section voodoo_license_pugixml_terms Pugixml License
     *
     * Copyright (c) 2006-2010 Arseny Kapoulkine 
     *
     * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
     * documentation files (the "Software"), to deal in the Software without restriction, including without limitation the 
     * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to 
     * permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright 
     * notice and this permission notice shall be included in all copies or substantial portions of the Software. 
     *
     * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
     * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
     * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
     * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
     * 
     * @section voodoo_license_terms_boost Boost Software License
     * 
     * Boost Software License - Version 1.0 - August 17th, 2003
     * 
     * Permission is hereby granted, free of charge, to any person or organization
     * obtaining a copy of the software and accompanying documentation covered by
     * this license (the "Software") to use, reproduce, display, distribute,
     * execute, and transmit the Software, and to prepare derivative works of the
     * Software, and to permit third-parties to whom the Software is furnished to
     * do so, all subject to the following:
     * 
     * The copyright notices in the Software and this entire statement, including
     * the above license grant, this restriction and the following disclaimer,
     * must be included in all copies of the Software, in whole or in part, and
     * all derivative works of the Software, unless such copies or derivative
     * works are solely in the form of machine-executable object code generated by
     * a source language processor.
     * 
     * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
     * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
     * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
     * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
     * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
     * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
     * DEALINGS IN THE SOFTWARE.
     * 
     * @section voodoo_license_terms_glew Glew License Terms
     * 
     * The OpenGL Extension Wrangler Library
     * Copyright (C) 2002-2007, Milan Ikits &lt;milan ikits[]ieee org&gt;
     * Copyright (C) 2002-2007, Marcelo E. Magallon &lt;mmagallo[]debian org&gt;
     * Copyright (C) 2002, Lev Povalahev
     * All rights reserved.
     * 
     * Redistribution and use in source and binary forms, with or without 
     * modification, are permitted provided that the following conditions are met:
     * 
     * * Redistributions of source code must retain the above copyright notice, 
     * this list of conditions and the following disclaimer.
     * * Redistributions in binary form must reproduce the above copyright notice, 
     * this list of conditions and the following disclaimer in the documentation 
     * and/or other materials provided with the distribution.
     * * The name of the author may be used to endorse or promote products 
     * derived from this software without specific prior written permission.
     * 
     * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
     * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
     * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
     * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
     * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
     * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
     * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
     * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
     * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
     * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
     * THE POSSIBILITY OF SUCH DAMAGE.
     * 
     * Mesa 3-D graphics library
     * Version:  7.0
     * 
     * Copyright (C) 1999-2007  Brian Paul   All Rights Reserved.
     * 
     * Permission is hereby granted, free of charge, to any person obtaining a
     * copy of this software and associated documentation files (the "Software"),
     * to deal in the Software without restriction, including without limitation
     * the rights to use, copy, modify, merge, publish, distribute, sublicense,
     * and/or sell copies of the Software, and to permit persons to whom the
     * Software is furnished to do so, subject to the following conditions:
     * 
     * The above copyright notice and this permission notice shall be included
     * in all copies or substantial portions of the Software.
     * 
     * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
     * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
     * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
     * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
     * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
     * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
     * 
     * 
     * Copyright (c) 2007 The Khronos Group Inc.
     * 
     * Permission is hereby granted, free of charge, to any person obtaining a
     * copy of this software and/or associated documentation files (the
     * "Materials"), to deal in the Materials without restriction, including
     * without limitation the rights to use, copy, modify, merge, publish,
     * distribute, sublicense, and/or sell copies of the Materials, and to
     * permit persons to whom the Materials are furnished to do so, subject to
     * the following conditions:
     * 
     * The above copyright notice and this permission notice shall be included
     * in all copies or substantial portions of the Materials.
     * 
     * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
     * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
     * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
     * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
     * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
     * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
     * MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
     *
     * @section voodoo_license_terms_cg License For Customer Use of NVIDIA Software
     *
     * License For Customer Use of NVIDIA Software
     * 
     * IMPORTANT NOTICE -- READ CAREFULLY: This License For Customer Use of
     * NVIDIA Software ("LICENSE") is the agreement which governs use of the
     * software of NVIDIA Corporation and its subsidiaries ("NVIDIA") downloadable
     * herefrom, including computer software and associated printed materials
     * ("SOFTWARE"). By downloading, installing, copying, or otherwise using the
     * SOFTWARE, you agree to be bound by the terms of this LICENSE.  If you do
     * not agree to the terms of this LICENSE, do not download the SOFTWARE.
     * 
     * RECITALS
     * 
     * Use of NVIDIA's products requires three elements: the SOFTWARE, the hardware
     * on a graphics controller board, and a personal computer. The SOFTWARE is
     * protected by copyright laws and international copyright treaties, as well as
     * other intellectual property laws and treaties.  The SOFTWARE is not sold, and
     * instead is only licensed for use, strictly in accordance with this document.
     * The hardware is protected by various patents, and is sold, but this agreement
     * does not cover that sale, since it may not necessarily be sold as a package
     * with the SOFTWARE. This agreement sets forth the terms and conditions of the
     * SOFTWARE LICENSE only.
     * 
     * 1.  DEFINITIONS
     * 
     * 1.1  Customer.  Customer means the entity or individual that downloads the
     *      SOFTWARE.
     * 
     * 2.  GRANT OF LICENSE
     * 
     * 2.1  Rights and Limitations of Grant.  NVIDIA hereby grants Customer the
     *      following non-exclusive, worldwide, royalty-free, non-transferable right
     *      to use the SOFTWARE, with the following limitations:
     * 
     * 2.1.1  Rights.   Customer may use, reproduce, distribute, publicly display
     *        and publicly perform the SOFTWARE.
     * 
     * 2.1.2  Limitations.
     * 
     * No Reverse Engineering.  Customer may not reverse engineer, decompile, or
     * disassemble the SOFTWARE, nor attempt in any other manner to obtain the
     * source code.
     * 
     * No Modification.  The SOFTWARE may be redistributed providing that distributed
     * Cg compiler and runtime binaries are unmodified, except for decompression and
     * compression.
     * 
     * No Rental.  Customer may not rent or lease the SOFTWARE to someone else.
     * 
     * No Support.  NVIDIA will not be responsible for providing maintenance and
     * support to Customer or any other end users for the Software distributed by
     * Customer or others.
     * 
     * 3.  TERMINATION
     * 
     * This LICENSE will automatically terminate if Customer fails to comply with
     * any of the terms and conditions hereof.  In such event, Customer must destroy
     * all copies of the SOFTWARE and all of its component parts.
     * 
     * 4.  COPYRIGHT
     * 
     * All title and copyrights in and to the SOFTWARE (including but not limited to
     * all images, photographs, animations, video, audio, music, text, and other
     * information incorporated into the SOFTWARE), the accompanying printed
     * materials, and any copies of the SOFTWARE, are owned by NVIDIA, or its
     * suppliers.  The SOFTWARE is protected by copyright laws and international
     * treaty provisions.  Accordingly, Customer is required to treat the SOFTWARE
     * like any other copyrighted material. 
     * 
     * Customer  agrees that the Software is proprietary information of NVIDIA and
     * that NVIDIA owns all right, title and interest therein. There are no implied
     * licenses under this License, and any rights not expressly granted are reserved
     * by NVIDIA. The Software is not sold, and instead is only licensed for use,
     * strictly in accordance with this License. All copies of the Software shall
     * contain NVIDIA's proprietary rights notices as provided therein. Customer
     * shall not remove or modify any such proprietary rights notices of NVIDIA. This
     * License will automatically terminate if Customer fails to comply with any of
     * the terms and conditions hereof. In such event, Customer must cease
     * reproducing, distributing, or otherwise using the Software and destroy all
     * copies thereof.
     * 
     * 5.  APPLICABLE LAW
     * 
     * This agreement shall be deemed to have been made in, and shall be construed
     * pursuant to, the laws of the State of Delaware.  Any suit or controversy
     * arising hereunder shall be brought in the federal or state courts located in
     * Santa Clara County, California, and each party submits to the venue and
     * jurisdiction thereof.
     * 
     * 6.  DISCLAIMER OF WARRANTIES AND LIMITATION ON LIABILITY
     * 
     * 6.1  No Warranties.  TO THE MAXIMUM EXTENT PERMITTED BY
     *         APPLICABLE LAW, THE SOFTWARE IS PROVIDED "AS IS" AND NVIDIA
     *         AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES, EITHER EXPRESS
     *         OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, IMPLIED
     *         WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
     *         PURPOSE AND NONINFRINGEMENT. 
     * 
     * 6.2  No Liability for Consequential Damages.  TO THE MAXIMUM
     *         EXTENT PERMITTED BY APPLICABLE LAW, IN NO EVENT SHALL
     *         NVIDIA OR ITS SUPPLIERS BE LIABLE FOR ANY SPECIAL,
     *         INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES WHATSOEVER
     *         (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF
     *         BUSINESS PROFITS, BUSINESS INTERRUPTION, LOSS OF BUSINESS
     *         INFORMATION, OR ANY OTHER PECUNIARY LOSS) ARISING OUT OF
     *         THE USE OF OR INABILITY TO USE THE SOFTWARE, EVEN IF NVIDIA
     *         HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.  THESE
     *         LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
     *         ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.  NVIDIA SHALL HAVE
     *         NO CONTRACTUAL OBLIGATION TO INDEMNIFY CUSTOMER UNDER
     *         THIS LICENSE. CUSTOMER AND/OR END-USERS OF THE SOFTWARE
     *         DISTRIBUTED BY CUSTOMER ASSUME THE ENTIRE COST OF ANY
     *         DAMAGE, LOSS, OR EXPENSE RESULTING FROM THEIR USE OR
     *         EXPLOITATION OF THE SOFTWARE.
     * 
     * 7.  MISCELLANEOUS
     * 
     * The United Nations Convention on Contracts for the International Sale of
     * Goods is specifically disclaimed.    This agreement is the final, complete
     * and exclusive agreement between the parties relating to the subject matter
     * hereof, and supersedes all prior or contemporaneous understandings and
     * agreements relating to such subject matter, whether oral or written.
     * Customer agrees that it will not ship, transfer or export the SOFTWARE
     * into any country, or use the SOFTWARE in any manner, prohibited by the
     * United States Bureau of Export Administration or any export laws,
     * restrictions or regulations.  
     * 
     * If any provision of this License is held to be invalid or unenforceable
     * for any reason, the remaining provisions will continue in full force
     * without being impaired or invalidated in any way.
     * 
     * No term or provisions hereof shall be deemed waived, and no breach excused,
     * unless such waiver or consent is in writing and signed by the party claimed
     * to have waived or consented. The waiver by either party of a breach of any
     * provision of this License will not operate or be interpreted as a waiver of
     * any other or subsequent breach.
     * 
     * This License may be changed only by mutual agreement in writing of the
     * authorized representatives of the parties.
     *
     * @section voodoo_license_terms_lgpl21 GNU Lesser General Public License, version 2.1
     * 
     *  GNU LESSER GENERAL PUBLIC LICENSE
     * Version 2.1, February 1999
     * 
     * Copyright (C) 1991, 1999 Free Software Foundation, Inc.
     * 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
     * Everyone is permitted to copy and distribute verbatim copies
     * of this license document, but changing it is not allowed.
     * 
     * [This is the first released version of the Lesser GPL.  It also counts
     * as the successor of the GNU Library Public License, version 2, hence
     * the version number 2.1.]
     * 
     * Preamble
     * 
     * The licenses for most software are designed to take away your
     * freedom to share and change it.  By contrast, the GNU General Public
     * Licenses are intended to guarantee your freedom to share and change
     * free software--to make sure the software is free for all its users.
     * 
     * This license, the Lesser General Public License, applies to some
     * specially designated software packages--typically libraries--of the
     * Free Software Foundation and other authors who decide to use it.  You
     * can use it too, but we suggest you first think carefully about whether
     * this license or the ordinary General Public License is the better
     * strategy to use in any particular case, based on the explanations below.
     * 
     * When we speak of free software, we are referring to freedom of use,
     * not price.  Our General Public Licenses are designed to make sure that
     * you have the freedom to distribute copies of free software (and charge
     * for this service if you wish); that you receive source code or can get
     * it if you want it; that you can change the software and use pieces of
     * it in new free programs; and that you are informed that you can do
     * these things.

     * To protect your rights, we need to make restrictions that forbid
     * distributors to deny you these rights or to ask you to surrender these
     * rights.  These restrictions translate to certain responsibilities for
     * you if you distribute copies of the library or if you modify it.

     * For example, if you distribute copies of the library, whether gratis
     * or for a fee, you must give the recipients all the rights that we gave
     * you.  You must make sure that they, too, receive or can get the source
     * code.  If you link other code with the library, you must provide
     * complete object files to the recipients, so that they can relink them
     * with the library after making changes to the library and recompiling
     * it.  And you must show them these terms so they know their rights.

     * We protect your rights with a two-step method: (1) we copyright the
     * library, and (2) we offer you this license, which gives you legal
     * permission to copy, distribute and/or modify the library.

     * To protect each distributor, we want to make it very clear that
     * there is no warranty for the free library.  Also, if the library is
     * modified by someone else and passed on, the recipients should know
     * that what they have is not the original version, so that the original
     * author's reputation will not be affected by problems that might be
     * introduced by others.
     * 
     * Finally, software patents pose a constant threat to the existence of
     * any free program.  We wish to make sure that a company cannot
     * effectively restrict the users of a free program by obtaining a
     * restrictive license from a patent holder.  Therefore, we insist that
     * any patent license obtained for a version of the library must be
     * consistent with the full freedom of use specified in this license.
     * 
     * Most GNU software, including some libraries, is covered by the
     * ordinary GNU General Public License.  This license, the GNU Lesser
     * General Public License, applies to certain designated libraries, and
     * is quite different from the ordinary General Public License.  We use
     * this license for certain libraries in order to permit linking those
     * libraries into non-free programs.
     * 
     * When a program is linked with a library, whether statically or using
     * a shared library, the combination of the two is legally speaking a
     * combined work, a derivative of the original library.  The ordinary
     * General Public License therefore permits such linking only if the
     * entire combination fits its criteria of freedom.  The Lesser General
     * Public License permits more lax criteria for linking other code with
     * the library.
     * 
     * We call this license the "Lesser" General Public License because it
     * does Less to protect the user's freedom than the ordinary General
     * Public License.  It also provides other free software developers Less
     * of an advantage over competing non-free programs.  These disadvantages
     * are the reason we use the ordinary General Public License for many
     * libraries.  However, the Lesser license provides advantages in certain
     * special circumstances.
     * 
     * For example, on rare occasions, there may be a special need to
     * encourage the widest possible use of a certain library, so that it becomes
     * a de-facto standard.  To achieve this, non-free programs must be
     * allowed to use the library.  A more frequent case is that a free
     * library does the same job as widely used non-free libraries.  In this
     * case, there is little to gain by limiting the free library to free
     * software only, so we use the Lesser General Public License.
     * 
     * In other cases, permission to use a particular library in non-free
     * programs enables a greater number of people to use a large body of
     * free software.  For example, permission to use the GNU C Library in
     * non-free programs enables many more people to use the whole GNU
     * operating system, as well as its variant, the GNU/Linux operating
     * system.
     * 
     * Although the Lesser General Public License is Less protective of the
     * users' freedom, it does ensure that the user of a program that is
     * linked with the Library has the freedom and the wherewithal to run
     * that program using a modified version of the Library.
     * 
     * The precise terms and conditions for copying, distribution and
     * modification follow.  Pay close attention to the difference between a
     * "work based on the library" and a "work that uses the library".  The
     * former contains code derived from the library, whereas the latter must
     * be combined with the library in order to run.
     * 
     * GNU LESSER GENERAL PUBLIC LICENSE
     * TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
     * 
     * 0. This License Agreement applies to any software library or other
     * program which contains a notice placed by the copyright holder or
     * other authorized party saying it may be distributed under the terms of
     * this Lesser General Public License (also called "this License").
     * Each licensee is addressed as "you".
     * 
     * A "library" means a collection of software functions and/or data
     * prepared so as to be conveniently linked with application programs
     * (which use some of those functions and data) to form executables.
     * 
     * The "Library", below, refers to any such software library or work
     * which has been distributed under these terms.  A "work based on the
     * Library" means either the Library or any derivative work under
     * copyright law: that is to say, a work containing the Library or a
     * portion of it, either verbatim or with modifications and/or translated
     * straightforwardly into another language.  (Hereinafter, translation is
     * included without limitation in the term "modification".)
     * 
     * "Source code" for a work means the preferred form of the work for
     * making modifications to it.  For a library, complete source code means
     * all the source code for all modules it contains, plus any associated
     * interface definition files, plus the scripts used to control compilation
     * and installation of the library.
     * 
     * Activities other than copying, distribution and modification are not
     * covered by this License; they are outside its scope.  The act of
     * running a program using the Library is not restricted, and output from
     * such a program is covered only if its contents constitute a work based
     * on the Library (independent of the use of the Library in a tool for
     * writing it).  Whether that is true depends on what the Library does
     * and what the program that uses the Library does.
     * 
     * 1. You may copy and distribute verbatim copies of the Library's
     * complete source code as you receive it, in any medium, provided that
     * you conspicuously and appropriately publish on each copy an
     * appropriate copyright notice and disclaimer of warranty; keep intact
     * all the notices that refer to this License and to the absence of any
     * warranty; and distribute a copy of this License along with the
     * Library.
     * 
     * You may charge a fee for the physical act of transferring a copy,
     * and you may at your option offer warranty protection in exchange for a
     * fee.
     * 
     * 2. You may modify your copy or copies of the Library or any portion
     * of it, thus forming a work based on the Library, and copy and
     * distribute such modifications or work under the terms of Section 1
     * above, provided that you also meet all of these conditions:
     * 
     * a) The modified work must itself be a software library.
     * 
     * b) You must cause the files modified to carry prominent notices
     * stating that you changed the files and the date of any change.
     * 
     * c) You must cause the whole of the work to be licensed at no
     * charge to all third parties under the terms of this License.
     * 
     * d) If a facility in the modified Library refers to a function or a
     * table of data to be supplied by an application program that uses
     * the facility, other than as an argument passed when the facility
     * is invoked, then you must make a good faith effort to ensure that,
     * in the event an application does not supply such function or
     * table, the facility still operates, and performs whatever part of
     * its purpose remains meaningful.
     * 
     * (For example, a function in a library to compute square roots has
     * a purpose that is entirely well-defined independent of the
     * application.  Therefore, Subsection 2d requires that any
     * application-supplied function or table used by this function must
     * be optional: if the application does not supply it, the square
     * root function must still compute square roots.)
     * 
     * These requirements apply to the modified work as a whole.  If
     * identifiable sections of that work are not derived from the Library,
     * and can be reasonably considered independent and separate works in
     * themselves, then this License, and its terms, do not apply to those
     * sections when you distribute them as separate works.  But when you
     * distribute the same sections as part of a whole which is a work based
     * on the Library, the distribution of the whole must be on the terms of
     * this License, whose permissions for other licensees extend to the
     * entire whole, and thus to each and every part regardless of who wrote
     * it.
     * 
     * Thus, it is not the intent of this section to claim rights or contest
     * your rights to work written entirely by you; rather, the intent is to
     * exercise the right to control the distribution of derivative or
     * collective works based on the Library.
     * 
     * In addition, mere aggregation of another work not based on the Library
     * with the Library (or with a work based on the Library) on a volume of
     * a storage or distribution medium does not bring the other work under
     * the scope of this License.
     * 
     * 3. You may opt to apply the terms of the ordinary GNU General Public
     * License instead of this License to a given copy of the Library.  To do
     * this, you must alter all the notices that refer to this License, so
     * that they refer to the ordinary GNU General Public License, version 2,
     * instead of to this License.  (If a newer version than version 2 of the
     * ordinary GNU General Public License has appeared, then you can specify
     * that version instead if you wish.)  Do not make any other change in
     * these notices.
     * 
     * Once this change is made in a given copy, it is irreversible for
     * that copy, so the ordinary GNU General Public License applies to all
     * subsequent copies and derivative works made from that copy.
     * 
     * This option is useful when you wish to copy part of the code of
     * the Library into a program that is not a library.
     * 
     * 4. You may copy and distribute the Library (or a portion or
     * derivative of it, under Section 2) in object code or executable form
     * under the terms of Sections 1 and 2 above provided that you accompany
     * it with the complete corresponding machine-readable source code, which
     * must be distributed under the terms of Sections 1 and 2 above on a
     * medium customarily used for software interchange.
     * 
     * If distribution of object code is made by offering access to copy
     * from a designated place, then offering equivalent access to copy the
     * source code from the same place satisfies the requirement to
     * distribute the source code, even though third parties are not
     * compelled to copy the source along with the object code.
     * 
     * 5. A program that contains no derivative of any portion of the
     * Library, but is designed to work with the Library by being compiled or
     * linked with it, is called a "work that uses the Library".  Such a
     * work, in isolation, is not a derivative work of the Library, and
     * therefore falls outside the scope of this License.
     * 
     * However, linking a "work that uses the Library" with the Library
     * creates an executable that is a derivative of the Library (because it
     * contains portions of the Library), rather than a "work that uses the
     * library".  The executable is therefore covered by this License.
     * Section 6 states terms for distribution of such executables.
     * 
     * When a "work that uses the Library" uses material from a header file
     * that is part of the Library, the object code for the work may be a
     * derivative work of the Library even though the source code is not.
     * Whether this is true is especially significant if the work can be
     * linked without the Library, or if the work is itself a library.  The
     * threshold for this to be true is not precisely defined by law.
     * 
     * If such an object file uses only numerical parameters, data
     * structure layouts and accessors, and small macros and small inline
     * functions (ten lines or less in length), then the use of the object
     * file is unrestricted, regardless of whether it is legally a derivative
     * work.  (Executables containing this object code plus portions of the
     * Library will still fall under Section 6.)
     * 
     * Otherwise, if the work is a derivative of the Library, you may
     * distribute the object code for the work under the terms of Section 6.
     * Any executables containing that work also fall under Section 6,
     * whether or not they are linked directly with the Library itself.
     * 
     * 6. As an exception to the Sections above, you may also combine or
     * link a "work that uses the Library" with the Library to produce a
     * work containing portions of the Library, and distribute that work
     * under terms of your choice, provided that the terms permit
     * modification of the work for the customer's own use and reverse
     * engineering for debugging such modifications.
     * 
     * You must give prominent notice with each copy of the work that the
     * Library is used in it and that the Library and its use are covered by
     * this License.  You must supply a copy of this License.  If the work
     * during execution displays copyright notices, you must include the
     * copyright notice for the Library among them, as well as a reference
     * directing the user to the copy of this License.  Also, you must do one
     * of these things:
     * 
     * a) Accompany the work with the complete corresponding
     * machine-readable source code for the Library including whatever
     * changes were used in the work (which must be distributed under
     * Sections 1 and 2 above); and, if the work is an executable linked
     * with the Library, with the complete machine-readable "work that
     * uses the Library", as object code and/or source code, so that the
     * user can modify the Library and then relink to produce a modified
     * executable containing the modified Library.  (It is understood
     * that the user who changes the contents of definitions files in the
     * Library will not necessarily be able to recompile the application
     * to use the modified definitions.)
     * 
     * b) Use a suitable shared library mechanism for linking with the
     * Library.  A suitable mechanism is one that (1) uses at run time a
     * copy of the library already present on the user's computer system,
     * rather than copying library functions into the executable, and (2)
     * will operate properly with a modified version of the library, if
     * the user installs one, as long as the modified version is
     * interface-compatible with the version that the work was made with.
     * 
     * c) Accompany the work with a written offer, valid for at
     * least three years, to give the same user the materials
     * specified in Subsection 6a, above, for a charge no more
     * than the cost of performing this distribution.
     * 
     * d) If distribution of the work is made by offering access to copy
     * from a designated place, offer equivalent access to copy the above
     * specified materials from the same place.
     * 
     * e) Verify that the user has already received a copy of these
     * materials or that you have already sent this user a copy.
     * 
     * For an executable, the required form of the "work that uses the
     * Library" must include any data and utility programs needed for
     * reproducing the executable from it.  However, as a special exception,
     * the materials to be distributed need not include anything that is
     * normally distributed (in either source or binary form) with the major
     * components (compiler, kernel, and so on) of the operating system on
     * which the executable runs, unless that component itself accompanies
     * the executable.
     * 
     * It may happen that this requirement contradicts the license
     * restrictions of other proprietary libraries that do not normally
     * accompany the operating system.  Such a contradiction means you cannot
     * use both them and the Library together in an executable that you
     * distribute.
     * 
     * 7. You may place library facilities that are a work based on the
     * Library side-by-side in a single library together with other library
     * facilities not covered by this License, and distribute such a combined
     * library, provided that the separate distribution of the work based on
     * the Library and of the other library facilities is otherwise
     * permitted, and provided that you do these two things:
     * 
     * a) Accompany the combined library with a copy of the same work
     * based on the Library, uncombined with any other library
     * facilities.  This must be distributed under the terms of the
     * Sections above.
     * 
     * b) Give prominent notice with the combined library of the fact
     * that part of it is a work based on the Library, and explaining
     * where to find the accompanying uncombined form of the same work.
     * 
     * 8. You may not copy, modify, sublicense, link with, or distribute
     * the Library except as expressly provided under this License.  Any
     * attempt otherwise to copy, modify, sublicense, link with, or
     * distribute the Library is void, and will automatically terminate your
     * rights under this License.  However, parties who have received copies,
     * or rights, from you under this License will not have their licenses
     * terminated so long as such parties remain in full compliance.
     * 
     * 9. You are not required to accept this License, since you have not
     * signed it.  However, nothing else grants you permission to modify or
     * distribute the Library or its derivative works.  These actions are
     * prohibited by law if you do not accept this License.  Therefore, by
     * modifying or distributing the Library (or any work based on the
     * Library), you indicate your acceptance of this License to do so, and
     * all its terms and conditions for copying, distributing or modifying
     * the Library or works based on it.
     * 
     * 10. Each time you redistribute the Library (or any work based on the
     * Library), the recipient automatically receives a license from the
     * original licensor to copy, distribute, link with or modify the Library
     * subject to these terms and conditions.  You may not impose any further
     * restrictions on the recipients' exercise of the rights granted herein.
     * You are not responsible for enforcing compliance by third parties with
     * this License.
     * 
     * 11. If, as a consequence of a court judgment or allegation of patent
     * infringement or for any other reason (not limited to patent issues),
     * conditions are imposed on you (whether by court order, agreement or
     * otherwise) that contradict the conditions of this License, they do not
     * excuse you from the conditions of this License.  If you cannot
     * distribute so as to satisfy simultaneously your obligations under this
     * License and any other pertinent obligations, then as a consequence you
     * may not distribute the Library at all.  For example, if a patent
     * license would not permit royalty-free redistribution of the Library by
     * all those who receive copies directly or indirectly through you, then
     * the only way you could satisfy both it and this License would be to
     * refrain entirely from distribution of the Library.
     * 
     * If any portion of this section is held invalid or unenforceable under any
     * particular circumstance, the balance of the section is intended to apply,
     * and the section as a whole is intended to apply in other circumstances.
     * 
     * It is not the purpose of this section to induce you to infringe any
     * patents or other property right claims or to contest validity of any
     * such claims; this section has the sole purpose of protecting the
     * integrity of the free software distribution system which is
     * implemented by public license practices.  Many people have made
     * generous contributions to the wide range of software distributed
     * through that system in reliance on consistent application of that
     * system; it is up to the author/donor to decide if he or she is willing
     * to distribute software through any other system and a licensee cannot
     * impose that choice.
     * 
     * This section is intended to make thoroughly clear what is believed to
     * be a consequence of the rest of this License.
     * 
     * 12. If the distribution and/or use of the Library is restricted in
     * certain countries either by patents or by copyrighted interfaces, the
     * original copyright holder who places the Library under this License may add
     * an explicit geographical distribution limitation excluding those countries,
     * so that distribution is permitted only in or among countries not thus
     * excluded.  In such case, this License incorporates the limitation as if
     * written in the body of this License.
     * 
     * 13. The Free Software Foundation may publish revised and/or new
     * versions of the Lesser General Public License from time to time.
     * Such new versions will be similar in spirit to the present version,
     * but may differ in detail to address new problems or concerns.
     * 
     * Each version is given a distinguishing version number.  If the Library
     * specifies a version number of this License which applies to it and
     * "any later version", you have the option of following the terms and
     * conditions either of that version or of any later version published by
     * the Free Software Foundation.  If the Library does not specify a
     * license version number, you may choose any version ever published by
     * the Free Software Foundation.
     * 
     * 14. If you wish to incorporate parts of the Library into other free
     * programs whose distribution conditions are incompatible with these,
     * write to the author to ask for permission.  For software which is
     * copyrighted by the Free Software Foundation, write to the Free
     * Software Foundation; we sometimes make exceptions for this.  Our
     * decision will be guided by the two goals of preserving the free status
     * of all derivatives of our free software and of promoting the sharing
     * and reuse of software generally.
     * 
     * NO WARRANTY
     * 
     * 15. BECAUSE THE LIBRARY IS LICENSED FREE OF CHARGE, THERE IS NO
     * WARRANTY FOR THE LIBRARY, TO THE EXTENT PERMITTED BY APPLICABLE LAW.
     * EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR
     * OTHER PARTIES PROVIDE THE LIBRARY "AS IS" WITHOUT WARRANTY OF ANY
     * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE
     * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
     * PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE
     * LIBRARY IS WITH YOU.  SHOULD THE LIBRARY PROVE DEFECTIVE, YOU ASSUME
     * THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION.
     * 
     * 16. IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN
     * WRITING WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY
     * AND/OR REDISTRIBUTE THE LIBRARY AS PERMITTED ABOVE, BE LIABLE TO YOU
     * FOR DAMAGES, INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR
     * CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OR INABILITY TO USE THE
     * LIBRARY (INCLUDING BUT NOT LIMITED TO LOSS OF DATA OR DATA BEING
     * RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD PARTIES OR A
     * FAILURE OF THE LIBRARY TO OPERATE WITH ANY OTHER SOFTWARE), EVEN IF
     * SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
     * DAMAGES.
     * 
     * END OF TERMS AND CONDITIONS
     * 
     * How to Apply These Terms to Your New Libraries
     * 
     * If you develop a new library, and you want it to be of the greatest
     * possible use to the public, we recommend making it free software that
     * everyone can redistribute and change.  You can do so by permitting
     * redistribution under these terms (or, alternatively, under the terms of the
     * ordinary General Public License).
     * 
     * To apply these terms, attach the following notices to the library.  It is
     * safest to attach them to the start of each source file to most effectively
     * convey the exclusion of warranty; and each file should have at least the
     * "copyright" line and a pointer to where the full notice is found.
     * 
     * &lt;one line to give the library's name and a brief idea of what it does.&gt;
     * Copyright (C) &lt;year&gt;  &lt;name of author&gt;
     * 
     * This library is free software; you can redistribute it and/or
     * modify it under the terms of the GNU Lesser General Public
     * License as published by the Free Software Foundation; either
     * version 2 of the License, or (at your option) any later version.
     * 
     * This library is distributed in the hope that it will be useful,
     * but WITHOUT ANY WARRANTY; without even the implied warranty of
     * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
     * Lesser General Public License for more details.
     * 
     * You should have received a copy of the GNU Lesser General Public
     * License along with this library; if not, write to the Free Software
     * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
     * 
     * Also add information on how to contact you by electronic and paper mail.
     * 
     * You should also get your employer (if you work as a programmer) or your
     * school, if any, to sign a "copyright disclaimer" for the library, if
     * necessary.  Here is a sample; alter the names:
     * 
     * Yoyodyne, Inc., hereby disclaims all copyright interest in the
     * library `Frob' (a library for tweaking knobs) written by James Random Hacker.
     * 
     * &lt;signature of Ty Coon&gt;, 1 April 1990
     * Ty Coon, President of Vice
     * 
     * That's all there is to it!
     *
     * 
     * @section voodoo_license_terms_lgpl3 GNU Lesser General Public License, version 3
     *
     *             GNU LESSER GENERAL PUBLIC LICENSE
     * Version 3, 29 June 2007
     * 
     * Copyright (C) 2007 Free Software Foundation, Inc. &lt;http://fsf.org/ &gt;
     * Everyone is permitted to copy and distribute verbatim copies
     * of this license document, but changing it is not allowed.
     * 
     * 
     * This version of the GNU Lesser General Public License incorporates
     * the terms and conditions of version 3 of the GNU General Public
     * License, supplemented by the additional permissions listed below.
     * 
     * 0. Additional Definitions.
     * 
     * As used herein, "this License" refers to version 3 of the GNU Lesser
     * General Public License, and the "GNU GPL" refers to version 3 of the GNU
     * General Public License.
     * 
     * "The Library" refers to a covered work governed by this License,
     * other than an Application or a Combined Work as defined below.
     * 
     * An "Application" is any work that makes use of an interface provided
     * by the Library, but which is not otherwise based on the Library.
     * Defining a subclass of a class defined by the Library is deemed a mode
     * of using an interface provided by the Library.
     * 
     * A "Combined Work" is a work produced by combining or linking an
     * Application with the Library.  The particular version of the Library
     * with which the Combined Work was made is also called the "Linked
     * Version".
     * 
     * The "Minimal Corresponding Source" for a Combined Work means the
     * Corresponding Source for the Combined Work, excluding any source code
     * for portions of the Combined Work that, considered in isolation, are
     * based on the Application, and not on the Linked Version.
     * 
     * The "Corresponding Application Code" for a Combined Work means the
     * object code and/or source code for the Application, including any data
     * and utility programs needed for reproducing the Combined Work from the
     * Application, but excluding the System Libraries of the Combined Work.
     * 
     * 1. Exception to Section 3 of the GNU GPL.
     * 
     * You may convey a covered work under sections 3 and 4 of this License
     * without being bound by section 3 of the GNU GPL.
     * 
     * 2. Conveying Modified Versions.
     * 
     * If you modify a copy of the Library, and, in your modifications, a
     * facility refers to a function or data to be supplied by an Application
     * that uses the facility (other than as an argument passed when the
     * facility is invoked), then you may convey a copy of the modified
     * version:
     * 
     * a) under this License, provided that you make a good faith effort to
     * ensure that, in the event an Application does not supply the
     * function or data, the facility still operates, and performs
     * whatever part of its purpose remains meaningful, or
     * 
     * b) under the GNU GPL, with none of the additional permissions of
     * this License applicable to that copy.
     * 
     * 3. Object Code Incorporating Material from Library Header Files.
     * 
     * The object code form of an Application may incorporate material from
     * a header file that is part of the Library.  You may convey such object
     * code under terms of your choice, provided that, if the incorporated
     * material is not limited to numerical parameters, data structure
     * layouts and accessors, or small macros, inline functions and templates
     * (ten or fewer lines in length), you do both of the following:
     * 
     * a) Give prominent notice with each copy of the object code that the
     * Library is used in it and that the Library and its use are
     * covered by this License.
     * 
     * b) Accompany the object code with a copy of the GNU GPL and this license
     * document.
     * 
     * 4. Combined Works.
     * 
     * You may convey a Combined Work under terms of your choice that,
     * taken together, effectively do not restrict modification of the
     * portions of the Library contained in the Combined Work and reverse
     * engineering for debugging such modifications, if you also do each of
     * the following:
     * 
     * a) Give prominent notice with each copy of the Combined Work that
     * the Library is used in it and that the Library and its use are
     * covered by this License.
     * 
     * b) Accompany the Combined Work with a copy of the GNU GPL and this license
     * document.
     * 
     * c) For a Combined Work that displays copyright notices during
     * execution, include the copyright notice for the Library among
     * these notices, as well as a reference directing the user to the
     * copies of the GNU GPL and this license document.
     * 
     * d) Do one of the following:
     * 
     * 0) Convey the Minimal Corresponding Source under the terms of this
     * License, and the Corresponding Application Code in a form
     * suitable for, and under terms that permit, the user to
     * recombine or relink the Application with a modified version of
     * the Linked Version to produce a modified Combined Work, in the
     * manner specified by section 6 of the GNU GPL for conveying
     * Corresponding Source.
     * 
     * 1) Use a suitable shared library mechanism for linking with the
     * Library.  A suitable mechanism is one that (a) uses at run time
     * a copy of the Library already present on the user's computer
     * system, and (b) will operate properly with a modified version
     * of the Library that is interface-compatible with the Linked
     * Version.
     * 
     * e) Provide Installation Information, but only if you would otherwise
     * be required to provide such information under section 6 of the
     * GNU GPL, and only to the extent that such information is
     * necessary to install and execute a modified version of the
     * Combined Work produced by recombining or relinking the
     * Application with a modified version of the Linked Version. (If
     * you use option 4d0, the Installation Information must accompany
     * the Minimal Corresponding Source and Corresponding Application
     * Code. If you use option 4d1, you must provide the Installation
     * Information in the manner specified by section 6 of the GNU GPL
     * for conveying Corresponding Source.)
     * 
     * 5. Combined Libraries.
     * 
     * You may place library facilities that are a work based on the
     * Library side by side in a single library together with other library
     * facilities that are not Applications and are not covered by this
     * License, and convey such a combined library under terms of your
     * choice, if you do both of the following:
     * 
     * a) Accompany the combined library with a copy of the same work based
     * on the Library, uncombined with any other library facilities,
     * conveyed under the terms of this License.
     * 
     * b) Give prominent notice with the combined library that part of it
     * is a work based on the Library, and explaining where to find the
     * accompanying uncombined form of the same work.
     * 
     * 6. Revised Versions of the GNU Lesser General Public License.
     * 
     * The Free Software Foundation may publish revised and/or new versions
     * of the GNU Lesser General Public License from time to time. Such new
     * versions will be similar in spirit to the present version, but may
     * differ in detail to address new problems or concerns.
     * 
     * Each version is given a distinguishing version number. If the
     * Library as you received it specifies that a certain numbered version
     * of the GNU Lesser General Public License "or any later version"
     * applies to it, you have the option of following the terms and
     * conditions either of that published version or of any later version
     * published by the Free Software Foundation. If the Library as you
     * received it does not specify a version number of the GNU Lesser
     * General Public License, you may choose any version of the GNU Lesser
     * General Public License ever published by the Free Software Foundation.
     * 
     * If the Library as you received it specifies that a proxy can decide
     * whether future versions of the GNU Lesser General Public License shall
     * apply, that proxy's public statement of acceptance of any version is
     * permanent authorization for you to choose that version for the
     * Library.
     */
}
