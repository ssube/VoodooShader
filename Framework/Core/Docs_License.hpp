/**
 * This file is part of the Voodoo Shader Framework, a comprehensive shader support library. 
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
     * (capable to dynamically loading the Voodoo core and an adapter into a running process), a simple filesystem manager 
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
     * The Voodoo Shader Framework is licensed under the terms of the GNU General Public License. This is in an effort to
     * make it useful to as many people as possible. Some elements of the framework and additional modules may use different
     * license terms, the terms below apply only to the core library. Differences in license terms are usually clearly marked,
     * please contact the developer(s) with any questions you may have. 
     * 
     * @subsection voodoo_license_summary_eula Application EULAs 
     *
     * The Voodoo Shader Framework is not a derivative work of any other work, nor is it bound under the terms of an other
     * application's EULA or other license restrictions. The framework is licensed solely under the terms of the GNU General
     * Public License, version 2, and any claims otherwise are false and unlawful. 
     * 
     * @note Portions of the Voodoo Shader Framework may be designed to operate aware of other applications. These
     *      applications may be licensed under different terms than the framework. Claims have been made that Voodoo is then
     *      bound by the terms of these target applications, or has no license terms. These are untrue, this is considered
     *      normal use of the target, and in no way changes the license terms of Voodoo or the target application. 
     *
     * @subsection voodoo_license_summary_contact Contact 
     * 
     * Questions about the Voodoo framework and its license terms should be directed to Sean Sube, at the email address
     * &lt;ahref="mailto:peachykeen@voodooshader.com"&gt;peachykeen@voodooshader.com&lt;/a&gt;. Questions about other modules and their 
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
     * The Voodoo Shader Framework core library uses the pugixml library to handle XML parsing and data retrieval.
     * This library is used under the terms of the MIT license. Thanks are given to Arseny Kapoulkine and all
     * others involved for this excellent library, it has made config files a @em lot easier to work with.
     *
     * @subsection voodoo_license_pugixml_terms pugixml License Terms 
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
     * @section voodoo_license_cg Cg Framework License 
     *
     * The Voodoo Shader Framework uses nVidia's Cg Toolkit for shader compiling and handling. This library is used under 
     * license terms provided below. Voodoo would not really be possible without Cg, so thanks are given to the Cg team for 
     * this library. 
     *
     * @subsection voodoo_license_cg_terms Cg License Terms 
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
     * @section voodoo_license_core Core License
     * 
     * The Voodoo Shader Framework is provided under the following legally binding license terms. These may or may not apply 
     * to modules and adapters, please contact the developer of each specific module with any questions. 
     *
     * @subsection voodoo_license_core_terms Core License Terms 
     *
     *             GNU GENERAL PUBLIC LICENSE
     *                    Version 2, June 1991
     * 
     * Copyright (C) 1989, 1991 Free Software Foundation, Inc.
     *   51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
     * Everyone is permitted to copy and distribute verbatim copies
     * of this license document, but changing it is not allowed.
     * 
     *                 Preamble
     * 
     *     The licenses for most software are designed to take away your
     * freedom to share and change it.  By contrast, the GNU General Public
     * License is intended to guarantee your freedom to share and change free
     * software--to make sure the software is free for all its users.  This
     * General Public License applies to most of the Free Software
     * Foundation's software and to any other program whose authors commit to
     * using it.  (Some other Free Software Foundation software is covered by
     * the GNU Library General Public License instead.)  You can apply it to
     * your programs, too.
     * 
     *     When we speak of free software, we are referring to freedom, not
     * price.  Our General Public Licenses are designed to make sure that you
     * have the freedom to distribute copies of free software (and charge for
     * this service if you wish), that you receive source code or can get it
     * if you want it, that you can change the software or use pieces of it
     * in new free programs; and that you know you can do these things.
     * 
     *     To protect your rights, we need to make restrictions that forbid
     * anyone to deny you these rights or to ask you to surrender the rights.
     * These restrictions translate to certain responsibilities for you if you
     * distribute copies of the software, or if you modify it.
     * 
     *     For example, if you distribute copies of such a program, whether
     * gratis or for a fee, you must give the recipients all the rights that
     * you have.  You must make sure that they, too, receive or can get the
     * source code.  And you must show them these terms so they know their
     * rights.
     * 
     *     We protect your rights with two steps: (1) copyright the software, and
     * (2) offer you this license which gives you legal permission to copy,
     * distribute and/or modify the software.
     * 
     *     Also, for each author's protection and ours, we want to make certain
     * that everyone understands that there is no warranty for this free
     * software.  If the software is modified by someone else and passed on, we
     * want its recipients to know that what they have is not the original, so
     * that any problems introduced by others will not reflect on the original
     * authors' reputations.
     * 
     *     Finally, any free program is threatened constantly by software
     * patents.  We wish to avoid the danger that redistributors of a free
     * program will individually obtain patent licenses, in effect making the
     * program proprietary.  To prevent this, we have made it clear that any
     * patent must be licensed for everyone's free use or not licensed at all.
     * 
     *     The precise terms and conditions for copying, distribution and
     * modification follow.
     * 
     *             GNU GENERAL PUBLIC LICENSE
     *     TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
     * 
     *     0. This License applies to any program or other work which contains
     * a notice placed by the copyright holder saying it may be distributed
     * under the terms of this General Public License.  The "Program", below,
     * refers to any such program or work, and a "work based on the Program"
     * means either the Program or any derivative work under copyright law:
     * that is to say, a work containing the Program or a portion of it,
     * either verbatim or with modifications and/or translated into another
     * language.  (Hereinafter, translation is included without limitation in
     * the term "modification".)  Each licensee is addressed as "you".
     * 
     * Activities other than copying, distribution and modification are not
     * covered by this License; they are outside its scope.  The act of
     * running the Program is not restricted, and the output from the Program
     * is covered only if its contents constitute a work based on the
     * Program (independent of having been made by running the Program).
     * Whether that is true depends on what the Program does.
     * 
     *     1. You may copy and distribute verbatim copies of the Program's
     * source code as you receive it, in any medium, provided that you
     * conspicuously and appropriately publish on each copy an appropriate
     * copyright notice and disclaimer of warranty; keep intact all the
     * notices that refer to this License and to the absence of any warranty;
     * and give any other recipients of the Program a copy of this License
     * along with the Program.
     * 
     * You may charge a fee for the physical act of transferring a copy, and
     * you may at your option offer warranty protection in exchange for a fee.
     * 
     *     2. You may modify your copy or copies of the Program or any portion
     * of it, thus forming a work based on the Program, and copy and
     * distribute such modifications or work under the terms of Section 1
     * above, provided that you also meet all of these conditions:
     * 
     *     a) You must cause the modified files to carry prominent notices
     *     stating that you changed the files and the date of any change.
     * 
     *     b) You must cause any work that you distribute or publish, that in
     *     whole or in part contains or is derived from the Program or any
     *     part thereof, to be licensed as a whole at no charge to all third
     *     parties under the terms of this License.
     * 
     *     c) If the modified program normally reads commands interactively
     *     when run, you must cause it, when started running for such
     *     interactive use in the most ordinary way, to print or display an
     *     announcement including an appropriate copyright notice and a
     *     notice that there is no warranty (or else, saying that you provide
     *     a warranty) and that users may redistribute the program under
     *     these conditions, and telling the user how to view a copy of this
     *     License.  (Exception: if the Program itself is interactive but
     *     does not normally print such an announcement, your work based on
     *     the Program is not required to print an announcement.)
     * 
     * These requirements apply to the modified work as a whole.  If
     * identifiable sections of that work are not derived from the Program,
     * and can be reasonably considered independent and separate works in
     * themselves, then this License, and its terms, do not apply to those
     * sections when you distribute them as separate works.  But when you
     * distribute the same sections as part of a whole which is a work based
     * on the Program, the distribution of the whole must be on the terms of
     * this License, whose permissions for other licensees extend to the
     * entire whole, and thus to each and every part regardless of who wrote it.
     * 
     * Thus, it is not the intent of this section to claim rights or contest
     * your rights to work written entirely by you; rather, the intent is to
     * exercise the right to control the distribution of derivative or
     * collective works based on the Program.
     * 
     * In addition, mere aggregation of another work not based on the Program
     * with the Program (or with a work based on the Program) on a volume of
     * a storage or distribution medium does not bring the other work under
     * the scope of this License.
     * 
     *     3. You may copy and distribute the Program (or a work based on it,
     * under Section 2) in object code or executable form under the terms of
     * Sections 1 and 2 above provided that you also do one of the following:
     * 
     *     a) Accompany it with the complete corresponding machine-readable
     *     source code, which must be distributed under the terms of Sections
     *     1 and 2 above on a medium customarily used for software interchange; or,
     * 
     *     b) Accompany it with a written offer, valid for at least three
     *     years, to give any third party, for a charge no more than your
     *     cost of physically performing source distribution, a complete
     *     machine-readable copy of the corresponding source code, to be
     *     distributed under the terms of Sections 1 and 2 above on a medium
     *     customarily used for software interchange; or,
     * 
     *     c) Accompany it with the information you received as to the offer
     *     to distribute corresponding source code.  (This alternative is
     *     allowed only for noncommercial distribution and only if you
     *     received the program in object code or executable form with such
     *     an offer, in accord with Subsection b above.)
     * 
     * The source code for a work means the preferred form of the work for
     * making modifications to it.  For an executable work, complete source
     * code means all the source code for all modules it contains, plus any
     * associated interface definition files, plus the scripts used to
     * control compilation and installation of the executable.  However, as a
     * special exception, the source code distributed need not include
     * anything that is normally distributed (in either source or binary
     * form) with the major components (compiler, kernel, and so on) of the
     * operating system on which the executable runs, unless that component
     * itself accompanies the executable.
     * 
     * If distribution of executable or object code is made by offering
     * access to copy from a designated place, then offering equivalent
     * access to copy the source code from the same place counts as
     * distribution of the source code, even though third parties are not
     * compelled to copy the source along with the object code.
     * 
     *     4. You may not copy, modify, sublicense, or distribute the Program
     * except as expressly provided under this License.  Any attempt
     * otherwise to copy, modify, sublicense or distribute the Program is
     * void, and will automatically terminate your rights under this License.
     * However, parties who have received copies, or rights, from you under
     * this License will not have their licenses terminated so long as such
     * parties remain in full compliance.
     * 
     *     5. You are not required to accept this License, since you have not
     * signed it.  However, nothing else grants you permission to modify or
     * distribute the Program or its derivative works.  These actions are
     * prohibited by law if you do not accept this License.  Therefore, by
     * modifying or distributing the Program (or any work based on the
     * Program), you indicate your acceptance of this License to do so, and
     * all its terms and conditions for copying, distributing or modifying
     * the Program or works based on it.
     * 
     *     6. Each time you redistribute the Program (or any work based on the
     * Program), the recipient automatically receives a license from the
     * original licensor to copy, distribute or modify the Program subject to
     * these terms and conditions.  You may not impose any further
     * restrictions on the recipients' exercise of the rights granted herein.
     * You are not responsible for enforcing compliance by third parties to
     * this License.
     * 
     *     7. If, as a consequence of a court judgment or allegation of patent
     * infringement or for any other reason (not limited to patent issues),
     * conditions are imposed on you (whether by court order, agreement or
     * otherwise) that contradict the conditions of this License, they do not
     * excuse you from the conditions of this License.  If you cannot
     * distribute so as to satisfy simultaneously your obligations under this
     * License and any other pertinent obligations, then as a consequence you
     * may not distribute the Program at all.  For example, if a patent
     * license would not permit royalty-free redistribution of the Program by
     * all those who receive copies directly or indirectly through you, then
     * the only way you could satisfy both it and this License would be to
     * refrain entirely from distribution of the Program.
     * 
     * If any portion of this section is held invalid or unenforceable under
     * any particular circumstance, the balance of the section is intended to
     * apply and the section as a whole is intended to apply in other
     * circumstances.
     * 
     * It is not the purpose of this section to induce you to infringe any
     * patents or other property right claims or to contest validity of any
     * such claims; this section has the sole purpose of protecting the
     * integrity of the free software distribution system, which is
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
     *     8. If the distribution and/or use of the Program is restricted in
     * certain countries either by patents or by copyrighted interfaces, the
     * original copyright holder who places the Program under this License
     * may add an explicit geographical distribution limitation excluding
     * those countries, so that distribution is permitted only in or among
     * countries not thus excluded.  In such case, this License incorporates
     * the limitation as if written in the body of this License.
     * 
     *     9. The Free Software Foundation may publish revised and/or new versions
     * of the General Public License from time to time.  Such new versions will
     * be similar in spirit to the present version, but may differ in detail to
     * address new problems or concerns.
     * 
     * Each version is given a distinguishing version number.  If the Program
     * specifies a version number of this License which applies to it and "any
     * later version", you have the option of following the terms and conditions
     * either of that version or of any later version published by the Free
     * Software Foundation.  If the Program does not specify a version number of
     * this License, you may choose any version ever published by the Free Software
     * Foundation.
     * 
     *     10. If you wish to incorporate parts of the Program into other free
     * programs whose distribution conditions are different, write to the author
     * to ask for permission.  For software which is copyrighted by the Free
     * Software Foundation, write to the Free Software Foundation; we sometimes
     * make exceptions for this.  Our decision will be guided by the two goals
     * of preserving the free status of all derivatives of our free software and
     * of promoting the sharing and reuse of software generally.
     * 
     *                 NO WARRANTY
     * 
     *     11. BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY
     * FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.  EXCEPT WHEN
     * OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES
     * PROVIDE THE PROGRAM "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
     * OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
     * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS
     * TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE
     * PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING,
     * REPAIR OR CORRECTION.
     * 
     *     12. IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING
     * WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR
     * REDISTRIBUTE THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES,
     * INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING
     * OUT OF THE USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED
     * TO LOSS OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY
     * YOU OR THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER
     * PROGRAMS), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE
     * POSSIBILITY OF SUCH DAMAGES.
     * 
     *                 END OF TERMS AND CONDITIONS
     * 
     *         How to Apply These Terms to Your New Programs
     * 
     *     If you develop a new program, and you want it to be of the greatest
     * possible use to the public, the best way to achieve this is to make it
     * free software which everyone can redistribute and change under these terms.
     * 
     *     To do so, attach the following notices to the program.  It is safest
     * to attach them to the start of each source file to most effectively
     * convey the exclusion of warranty; and each file should have at least
     * the "copyright" line and a pointer to where the full notice is found.
     * 
     *     &lt;one line to give the program's name and a brief idea of what it does.&gt;
     *     Copyright (C) &lt;year&gt;  &lt;name of author&gt;
     * 
     *     This program is free software; you can redistribute it and/or modify
     *     it under the terms of the GNU General Public License as published by
     *     the Free Software Foundation; either version 2 of the License, or
     *     (at your option) any later version.
     * 
     *     This program is distributed in the hope that it will be useful,
     *     but WITHOUT ANY WARRANTY; without even the implied warranty of
     *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     *     GNU General Public License for more details.
     * 
     *     You should have received a copy of the GNU General Public License
     *     along with this program; if not, write to the Free Software
     *     Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
     * 
     * 
     * Also add information on how to contact you by electronic and paper mail.
     * 
     * If the program is interactive, make it output a short notice like this
     * when it starts in an interactive mode:
     * 
     *     Gnomovision version 69, Copyright (C) year  name of author
     *     Gnomovision comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
     *     This is free software, and you are welcome to redistribute it
     *     under certain conditions; type `show c' for details.
     * 
     * The hypothetical commands `show w' and `show c' should show the appropriate
     * parts of the General Public License.  Of course, the commands you use may
     * be called something other than `show w' and `show c'; they could even be
     * mouse-clicks or menu items--whatever suits your program.
     * 
     * You should also get your employer (if you work as a programmer) or your
     * school, if any, to sign a "copyright disclaimer" for the program, if
     * necessary.  Here is a sample; alter the names:
     * 
     *     Yoyodyne, Inc., hereby disclaims all copyright interest in the program
     *     `Gnomovision' (which makes passes at compilers) written by James Hacker.
     * 
     *     &lt;signature of Ty Coon&gt;, 1 April 1989
     *     Ty Coon, President of Vice
     * 
     * This General Public License does not permit incorporating your program into
     * proprietary programs.  If your program is a subroutine library, you may
     * consider it more useful to permit linking proprietary applications with the
     * library.  If this is what you want to do, use the GNU Library General
     * Public License instead of this License.
     */
}
