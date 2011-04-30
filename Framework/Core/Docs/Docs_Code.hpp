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
 * @page page_modules Module Specifications
 * 
 * The Voodoo Core library (usually Voodoo_Core.dll) is designed to provide a generic framework
 * and certain Voodoo-specific features to help support shaders. To allow additional features
 * without breaking compatibility, Voodoo uses Microsoft's COM technology to allow a highly 
 * flexible plugin system. 
 * @since 0.10.0
 * 
 * To provide Voodoo plugins, modules must provide COM components matching one of the expected
 * interfaces and be registered with the system as providing these. Many tools, such as Visual 
 * Studio, provide relatively simple methods for creating new COM components. These components
 * should derive from the interface they wish to provide and must implement all methods. 
 * 
 * @section page_modules_sec_compat Compatibility
 * Voodoo uses the COM system to handle all plugins and provides little restriction on what may be
 * done within that. Any language or compiler capable of producing COM components may be used.
 * 
 * @subsection page_modules_ssec_lang Supported Languages
 * As a side benefit of using COM, Voodoo plugins may be written in a variety of languages. At this
 * time C++ may be used, as well as any language supported by the .Net framework. A partial list of 
 * common, and compatible, languages includes:
 * @ul
 *    @li C++
 *    @li C#
 *    @li F#
 *    @li Java 
 *    @li Lua
 *    @li Perl
 *    @li Python
 *    @li Ruby
 *    
 * @warning Due to the additional marshalling and conversion required, using .Net languages may
 *    cause a decrease in performance. The extent and amount depend greatly on the interface;
 *    rarely-used adapter plugins will have little effect, while replacing the logger may have a
 *    great hit. Voodoo's interfaces are designed to cause as little marshalling as possible
 *    (occasionally at the cost of more complex core code).
 *    
 * @note C is technically legal for use, but the Voodoo public headers do not provide C functions
 *    for the interfaces. If there is a demand for C compatibility with Voodoo, this will change.
 *    
 * To create a plugin, simply make sure the Voodoo core (or appropriate interface provider) is
 * registered on your system and inherit from an interface. Implement all methods and verify the 
 * class is both public and COM-visible (these depend slightly on your language). Build and 
 * register the module, then use the class' GUID in the config.
 * 
 * More extensive tutorials on writing COM components may be found in the MSDN documentation and
 * elsewhere on the internet. Some books on the subject are also available. A brief tutorial for
 * C++ and C# may be included in this documentation in the future.
 * 
 * @subsection page_modules_ssec_restrictions Restrictions
 * Not all languages may function properly with the Voodoo interfaces. This depends heavily on the
 * syntax and form of the language. Most .Net languages are compatible.
 * 
 * Multiple versions of the .Net runtime may not be loaded within a single process, @em unless the
 * .Net 4 runtime is installed. This means, for example, a system with .Net 3.5 installed
 * may use .Net 2.0 @em or .Net 3.5 plugins, but not both. However, a system with .Net 4.0 
 * installed may use 2.0 and 3.5 plugins simultaneously. This is a limitation of the .Net framework
 * and cannot be changed within Voodoo.
 * 
 * @section page_modules_sec_docs Documentation
 * It is highly recommended that module creators provide a list of class descriptions, GUIDS and 
 * ProgIDs (readable IDs) along with the module. The GUID or ProgID is required to reference the
 * class from the config, but are not externally visible from a compiled module.
 * 
 * @subsection page_modules_ssec_naming Naming Scheme
 * It is suggested that plugin modules conform to a basic naming scheme. This is not required for
 * third-party modules, but is for official Voodoo plugins.
 * 
 * Each plugin module is typically given a one-word name (for adapters, this is related to the
 * target application in some manner). It is recommended that the module filename be 
 * <code>Voodoo_[name].dll</code> for official plugins. Classes should use 
 * <code>Voodoo[name].[class]</code> for the ProgID. Documentation and the module's version 
 * resource should use <code>Voodoo/[name]</code> to refer to the module. Official plugins will be 
 * referenced by these name schemes in Voodoo documentation.
 * 
 * Third-party plugins should not use the word Voodoo in any names or IDs. Instead, they should use
 * a unique name or the developer's name to identify themselves.
 *
 * @page page_adapters Adapter Specifications
 * The adapter serves as a binding between an unknown (by the core library) graphics API and the 
 * constant Voodoo framework. The adapter is responsible for handling any interaction between the 
 * two, performing translation as necessary, and handling any required details. 
 * 
 * While most classes provided by modules are free to implement interfaces in whatever way they
 * wish and have few behavior restrictions, adapter classes must fit a very specific set of 
 * behavior rules and perform exactly as expected by the core.
 * 
 * In many senses, adapters are plugin modules and they must follow the @ref page_modules as any 
 * other module. However, only one adapter object is created at a time, and that object @em must 
 * derive from @ref IVoodooAdapter.
 * 
 * Adapter modules are free to provide other classes, in support of or unrelated to, the adapter
 * itself. Adapter modules may even provide more than one adapter class, possibly for debugging and
 * regular use. Only one adapter will be used at a time, however.
 * 
 * @section adapterspeccg Cg Context

 * @attention Due to varying needs between adapters regarding the management, and destruction, of 
 *    the Cg context, the Core now exposes methods to get and set the active context and no longer 
 *    creates a context. This allows more flexibility within adapters, but is a breaking change. 
 *    Adapters compiled for earlier versions will find themselves without a context at all.
 * @since 0.2.5.151
 *    
 * This can avoid some crashes with certain adapters due to context lifespan, and allows adapters 
 * to handle cleanup. If at all possible, the Cg context should be created in the adapter's 
 * constructor and destroyed in the adapter's destructor, but this is left up to the discretion of 
 * the adapter.
 * 
 * @warning Adapters may not create any shaders or parameters until they have provided the core
 *    with a valid context. Exceptions will be thrown if this is attempted. To fully support Voodoo
 *    features, adapter's must not directly use the context when the core provides methods to
 *    handle a particular feature.
 * 
 * The Core holds references to all created parameters and shaders, but will release these in either
 * of two cases:
 * 
 * @li The Core is destroyed (the destructor releases all resources, then destroys the adapter).
 * @li @ref Core::SetCgContext() is called with null as the context, indicating that the cached 
 *        context is to be cleared.
 * 
 * This will drop the strong references from the core and may free resources, if no other 
 * references are held.
 * 
 * @page page_howitworks How Voodoo Works
 * Voodoo provides a framework for constant features across a wide variety of games. Support
 * ranges from OpenGL 1.1 to DirectX 10, with a focus on translating older APIs to their newer
 * counterparts. To handle the huge differences, Voodoo is very modular and uses a number of
 * universal and specific components.
 * 
 * @section page_howitworks_sec_loader Voodoo/Loader
 * The chain of execution for Voodoo begins with the dynamic loader (Voodoo/Loader), which 
 * intercepts a single hook function in the target process. Because of how Window's search path 
 * for DLLs works, this loads the dynamic loader into the target's memory space.
 * 
 * The loader then retrieves the Voodoo path from the registry and dynamically loads the core
 * module (Voodoo/Core). With the core loaded, the loader creates a Core object and returns
 * execution to the target process.
 * 
 * @section page_howitworks_sec_core Voodoo/Core
 * The Core object, during creation, starts Cg and then handles loading other modules. The Cg 
 * runtime is loaded, a Cg context created and error callbacks registered. The Core then parses 
 * the configuration file present in the current directory and begins loading any additional 
 * modules given in that file. The logger is created first, to  provide the most information 
 * possible in case of errors. The hook manager is created second and the adapter third. Any other 
 * modules (given as Module elements in the config file under /VoodooConfig/Core) are loaded, 
 * logged and their classes registered.
 * 
 * @section page_howitworks_sec_adapter Adapter
 * The adapter load process performs most of the detail work. In many cases, the adapter creates
 * hooks in a number of system functions (creation functions for D3D or functions for OpenGL)
 * and may load an API-specific module for the Cg runtime. The adapter's constructor may perform
 * immediate binding to the graphics API or simply install hooks so the API is bound during
 * creation.
 * 
 * @page page_quality Code Style and Testing
 * 
 * @section page_quality_sec_intro Intro
 * The Voodoo Shader Framework is designed to work across many games, created across a wide 
 * variety of compilers, graphics libraries, code styles and time periods. To maximize stability 
 * and speed, both critical to Voodoo's purpose, all Voodoo projects must meet the following code 
 * rules. These are designed to eradicate bugs within Voodoo itself and provide the best user and 
 * developer experience possible, especially considering the high degree of complexity within 
 * Voodoo and its support libraries.
 * 
 * Third-party code that is used in Voodoo, or adapters that are not part of the official project, 
 * do not need to use these rules, but they can rely on Voodoo following them precisely.
 * 
 * Any bugs, potential bugs, performance bottlenecks, possible points of improvement or generally 
 * questionable code encountered in Voodoo should be reported as soon as possible. Please file an 
 * issue with as much information as possible in the official Voodoo bug tracker.
 * 
 * @section page_quality_sec_summary Summary
 * To help ensure quality, Voodoo's rules are summarized as follows:
 * @ul
 *    @li All modules must meet COM guidelines and applicable best-practice rules for their
 *        language.
 *    @li Modules must fully support Unicode and be able to interoperate with other modules using
 *        Unicode strings (UTF-8, in particular).
 *    @li Modules must meet COM interop guidelines, as various languages or runtimes may be mixed
 *        in the process.
 *    @li Modules should, with rare exceptions, compile with the highest typical warning level for
 *        their language and warnings-as-errors enabled (for C++ in VS, this is 
 *        <code>/W4 /WX</code>).
 *    @li Modules should, if possible, be run through full static analysis with warnings-as-errors
 *        enabled.
 *    @li Modules should use unit tests to verify functionality, and regression tests where needed.
 *        A full suite of unit tests should provide 100% <em>branch coverage for all module 
 *        code</em> (this does not include code used from other libraries, such as MFC or ATL).
 *    @li Modules must not be released if any compiler warnings, static analysis or unit tests 
 *        fail. 
 *    @li Modules should provide complete and up-to-date documentation, where applicable. For the
 *        Voodoo core, this is produced by the doxygen tool.
 * 
 * @section page_quality_sec_general General Rules
 * <ul>
 *    <li>All projects must be configured for Visual Studio 2010 (vcxproj files, all dependency
 *        paths set in the project settings, etc).</li>
 *    <ul>
 *       <li>Voodoo is designed to compile on Windows within Visual Studio. Compatibility with 
 *           other compilers or systems is not guaranteed or tested for and is highly unlikely. 
 *           Voodoo relies heavily Windows-specific features and APIs, as well as targeting mainly 
 *           Windows-specific software. Compatibility with Wine and Crossover will be tested in the
 *           future.</li>
 *    </ul>
 *    <li>\#pragma preprocessor directives should not be used to link libraries.</li>
 *    <li>Projects should have 3 standard build configurations and each config must follow the 
 *        appropriate rules.</li>
 *    <li>All projects must build in all configurations before any release.</li>
 *    <ul>
 *       <li>This is especially important for new projects. Building under Debug_Analysis can find
 *           many code issues before they run.</li>
 *    </ul>
 *    <li>Project behavior must remain consistent between build configurations. To help this, 
 *        \#ifdef _DEBUG and similar should be avoided.</li>
 *    <li>For C++ projects:
 *    <ul>
 *       <li>TR1 and C++0x/C++11 features may be used and are encouraged if they simplify code. 
 *           Visual Studio 2010 has limited support for C++0x features.</li>
 *       <li>All functions should use SAL annotations (<code>_In_, _Check_return_, etc</code>) 
 *           where appropriate. This improves static analysis quality.</li>
 *    <ul>
 *    <li>All classes, methods, fields, functions, structs, enums, typedefs or other applicable 
 *        code constructs must be documented with their purpose and any vital notes.</li>
 *    <ul>
 *       <li>Any function with unusual requirements or potential problems must have them 
 *           noted.</li>
 *       <li>All function parameters, global variables, enum members, class members, etc must be
 *           documented.</li>
 *    </ul>
 * </ul>
 * 
 * These rules apply primarily to C++ projects built within VS. For other languages, the closest 
 * equivalent should be followed.
 * 
 * @section events Events
 * 
 * @subsection commit Commit
 * <ul>
 *    <li>Version information must be updated to reflect the new revision.</li>
 *    <li>Every major commit or on the first commit of a week, a SourceMonitor entry must be made 
 *        with the current revision number as the name (e.g., rev111).</li>
 * </ul>
 * 
 * @subsection push Push
 * <ul>
 *    <li>Doxygen documentation must be generated and uploaded along with the push for all modified
 *        projects.</li>
 * </ul>
 * 
 * @subsection release Release
 * <ul>
 *    <li>Full build and static analysis must be run and code must pass.</li>
 *    <ul>
 *       <li>The Debug_Analysis build must succeed with no warnings or errors.</li>
 *       <li>CppCheck analysis must report no warnings or errors.</li>
 *       <li>Yasca analysis must find no warnings or errors.
 *       <ul>
 *          <li>Unused functions are excepted but should be avoided.</li>
 *          <li>Unsafe/banned functions should be avoided wherever possible.</li>
 *       </ul>
 *    </ul>
 *    <li>A SourceMonitor entry must be made with the release version (and name, if present).</li>
 *    <li>Version information must be updated.</li>
 *    <li>A release commit must be made and pushed. The commit message must include the release 
 *        version and name.</li>
 * </ul>
 * 
 * 
 * @section configs Build Configs
 * 
 * @subsection all All
 * <ul>
 *    <li>Compiler</li>
 *    <ul>
 *       <li>Use of MFC and ATL and CLR support should be set as applicable to the project, but 
 *           must be identical across configurations.</li>
 *       <li>Character set must be set to "Use Unicode Character Set" or equivalent (.Net uses 
 *           Unicode by default).</li>
 *    </ul>
 * </ul>
 *    
 * @subsection debug_analysis Debug_Analysis
 * <ul>
 *    <li>Compiler</li>
 *    <ul>
 *       <li>Debug mode must be turned on and _DEBUG must be defined.</li>
 *       <li>Debug information must be set to /Zi (program database).</li>
 *       <li>Warning level must be /W4.</li>
 *       <li>Warnings as errors (/WX) must be enabled.</li>
 *       <li>All optimizations must be disabled.</li>
 *       <li>Whole program optimizations must be set to "No whole program optimizations".</li>
 *       <li>Minimal rebuild must be off (/Gm-).</li>
 *       <li>Runtime checks must be both (/RTC1).</li>
 *       <li>Runtime library must be multithreaded debug DLL (/MDd).</li>
 *    </ul>
 *    <li>Linker</li>
 *    <ul>
 *       <li>Generate debug info must be on (/DEBUG).</li>
 *    </ul>
 *    <li>Analysis</li>
 *    <ul>
 *       <li>Code analysis must be enabled.</li>
 *       <li>Code analysis on build must be enabled.</li>
 *       <li>"Microsoft All Rules" ruleset must be used.</li>
 *    </ul>
 * </ul>
 *  
 * @subsection debug Debug
 * <ul>
 *    <li>Compiler</li>
 *    <ul>
 *       <li>Debug mode must be turned on and _DEBUG must be defined.</li>
 *       <li>Debug information must be set to /Zi (program database).</li>
 *       <li>Warning level must be /W4.</li>
 *       <li>Warnings as errors (/WX) must not be enabled.</li>
 *       <li>All optimizations must be disabled.</li>
 *       <li>Whole program optimizations must be set to "No whole program optimizations".</li>
 *       <li>Minimal rebuild must be off (/Gm-).</li>
 *       <li>Runtime checks must be both (/RTC1).</li>
 *       <li>Runtime library must be multithreaded debug DLL (/MDd).</li>
 *    </ul>
 *    <li>Linker</li>
 *    <ul>
 *       <li>Generate debug info must be on (/DEBUG).</li>
 *    </ul>
 *    <li>Analysis</li>
 *    <ul>
 *       <li>Code analysis must not be enabled.</li>
 *    </ul>
 * </ul>
 *  
 * @subsection release Release
 * <ul>
 *    <li>Compiler</li>
 *    <ul>
 *       <li>Debug mode must be turned off and _DEBUG must not be defined.</li>
 *       <li>Debug information must be set to /Zi (program database).</li>
 *       <li>Warning level must be /W4.</li>
 *       <li>Warnings as errors (/WX) must not be enabled.</li>
 *       <li>Whole program optimizations must be set to "Link-time code generation".</li>
 *       <li>Optimizations should be set to "Maximize speed" (/O2).</li>
 *       <li>Inline function expansion should be set to "Any suitable" (/Ob2).</li>
 *       <li>Enable Intrinsic Functions must be set to "Yes" (/Oi).</li>
 *       <li>Favor Size Or Speed must be set to "Favor fast code" (/Ot).</li>
 *       <li>Runtime Library must be set to multithreaded DLL (/MD).</li>
 *    </ul>
 *    <li>Linker</li>
 *    <ul>
 *       <li>Generate debug info must be off.</li>
 *    </ul>
 *    <li>Analysis</li>
 *    <ul>
 *       <li>Code analysis must not be enabled.</li>
 *    </ul>
 * </ul>
 */
