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
  
namespace VoodooShader
{
/**
 * @page modulespec Module Specifications
 * 
 * The Voodoo Core library (usually Voodoo_Core.dll) is designed to provide a generic framework
 * and certain Voodoo-specific features to help support shaders. To allow additional features
 * without breaking compatibility, Voodoo provides a class-based plugin interface. Modules may be 
 * added and classes provided by those modules created and used within Voodoo or adapters. To 
 * handle loading these modules, a known interface is required with a few specific features.
 * @since 0.2.0.0
 * 
 * @section moduleexports Module Exports
 * Each Voodoo module must export a set of 4 functions. These are used by the core to load and
 * query the module's version and what classes it adds. All these functions must be exported as 
 * their name only, no mangling or decorating of any kind.
 * 
 * @subsection moduleexportsmoduleversion ModuleVersion
 * @code 
 * Version ModuleVersion();
 * @endcode
 * The ModuleVersion function returns a Version struct with the module's name, version and debug 
 * build flag. This is used primarily for logging purposes.
 * 
 * @subsection moduleexportsclasscount ClassCount
 * @code 
 * int ClassCount();
 * @endcode
 * Returns the total number of classes provided by this module. Any class that needs to be created 
 * by other code must be listed in this function and the next.
 * 
 * @subsection moduleexportsclassinfo ClassInfo
 * @code 
 * const char * ClassInfo(int number);
 * @endcode
 * Returns the name of the given class. This is used to register the classes in the core
 * ModuleManager initially. As each module is loaded, the provided classes are iterated through 
 * from @p 0 to @p ClassCount-1 and added to the list of available classes.
 * @note For best compatibility, these may be GUIDs in string form.
 * 
 * @subsection moduleexportsclasscreate ClassCreate
 * @code 
 * IObject * ClassCreate(int number, Core * core);
 * @endcode
 * Creates a new instance of the given class, bound to the provided Core. For classes needing 
 * additional data, they can cache and/or use the core. 
 * 
 * @note Class constructors @em may throw exceptions. The @ref ModuleManager will catch and log 
 *    these when possible. When the class construction throws, null will be returned. 
 * 
 * @section moduleiobject IObject Interface
 * Every class provided through the above API @em must derive from the IObject interface and 
 * implement both methods.
 * 
 * @subsection moduleiobjectdtor Destructor
 * @code
 * virtual void IObject::~IObject() throw() { };
 * @endcode 
 * Destroys the object, cleaning up any resources. Overloads are called, cleaning up their
 * resources.
 * 
 * This virtual destructor is set to throw if it is called, and so must always be overloaded.
 * 
 * @subsection moduleiobjectgetobjectclass GetObjectClass
 * @code
 * virtual const char * IObject::GetObjectClass() = 0;
 * @endcode
 * Returns the object's class. This can be used to identify the source module or create another 
 * object of the same time. This should return the same value as @p ClassInfo for this class, so:
 * 
 * @code
 * const char * nameA = ClassInfo(1);
 * IObject * object = ClassCreate(1);
 * const char * nameB = object->GetObjectClass();
 * 
 * assert( nameA == nameB );            // May return the same string, but
 * assert( strcmp(nameA, nameB) == 0 ); // must return identical strings
 * @endcode
 * 
 * While @ref IObject does not provide reference-counting or fancier features, it 
 * does handle most vital features of dynamic module classes. Additional features may be added to 
 * later versions as needed.
 * 
 * @section moduledocs Documentation
 * It is generally recommended that module creators provide a list of class names and their
 * functions along with the module (especially if some can be used by the core). This is
 * particularly useful when classes use GUID names.
 *
 * @page adapterspec Adapter Specifications
 * The adapter serves as a binding between an unknown (by the core library) graphics API and the 
 * constant Voodoo framework. The adapter is responsible for handling any interaction between the 
 * two, performing translation as necessary, and handling any required details. 
 * 
 * While most classes provided by addon modules are free to provide whatever interface they wish
 * and are mostly autonomous, adapter classes must fit a very specific set of behavior rules and
 * perform exactly as expected by the Core.
 * 
 * In many senses, adapters are addon modules and they must follow the @ref modulespec as any other
 * module. However, only one adapter object is created at a time, and that object @em must derive 
 * from @ref IAdapter.
 * 
 * Adapter modules are free to provide other classes, in support of or unrelated to, the adapter
 * itself. Adapter modules may even provide more than one adapter class.
 * 
 * In most cases, the adapter and supporting classes should conform to a basic naming scheme.
 * Each Voodoo adapter is given a short one word somewhat-game-related name. It is recommended that
 * the adapter class be of the form name_Adapter and each supporting class use name_classname,
 * to easily identify them among the available classes. If the adapter or module is official, the
 * name scheme extends to the module's name (Voodoo/name) and often filename (Voodoo_name.dll).
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
 * @page voodoosystem How Voodoo Works
 * Voodoo provides a framework for constant features across a wide variety of games. Support
 * ranges from OpenGL 1.1 to DirectX 10, with a focus on translating older APIs to their newer
 * counterparts. To handle the huge differences, Voodoo is very modular and uses a number of
 * universal and specific components.
 * 
 * @section voodoosystemloader Voodoo/Loader
 * The chain of execution for Voodoo begins with the dynamic loader (Voodoo/Loader), which 
 * intercepts a single hook function in the target process. Because of how Window's search path 
 * for DLLs works, this loads the dynamic loader into the target's memory space.
 * 
 * The loader then retrieves the Voodoo path from the registry and dynamically loads the core
 * module (Voodoo/Core). With the core loaded, the loader creates a Core object and returns
 * execution to the target process.
 * 
 * @section voodoosystemcore Voodoo/Core
 * The Core object, during creation, starts Cg and then handles loading other modules. The Cg 
 * runtime is loaded, a Cg context created and error callbacks registered. The Core then parses 
 * the configuration file present in the current directory and begins loading any additional 
 * modules given in that file. The logger is created first, to  provide the most information 
 * possible in case of errors. The hook manager is created second and the adapter third. Any other 
 * modules (given as Module elements in the config file under /VoodooConfig/Core) are loaded, 
 * logged and their classes registered.
 * 
 * @section voodoosystemadapter Adapter
 * The adapter load process performs most of the detail work. In many cases, the adapter creates
 * hooks in a number of system functions (creation functions for D3D or functions for OpenGL)
 * and may load an API-specific module for the Cg runtime. The adapter's constructor may perform
 * immediate binding to the graphics API or simply install hooks so the API is bound during
 * creation.
 * 
 * @page codestyle Code Style and Testing
 * 
 * @section intro Intro
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
 * @section general General Rules
 * <ul>
 *    <li>All projects must be configured for Visual Studio 2010 (vcxproj files, all dependency
 *        paths set in the project settings, etc).</li>
 *    <li>\#pragma preprocessor directives and other VS-specific features should be avoided.</li>
 *    <ul>
 *       <li>Voodoo is designed to compile on Windows within Visual Studio. Compatibility with 
 *           other compilers is not guaranteed or tested for, compatibility with other systems is 
 *           highly unlikely. Voodoo relies on some Windows-specific features and APIs, as well as 
 *           targeting mainly Windows-specific software.</li>
 *       <li>\#pragma to disable DLL interface warnings (warning 4251) is excepted.</li>
 *    </ul>
 *    <li>All projects must have 3 standard build configurations and each config must follow the 
 *        appropriate rules.</li>
 *    <li>All projects must build in all configurations before any release.</li>
 *    <ul>
 *       <li>This is especially important for new projects. Building under Debug_Analysis can find
 *           many code issues before they run.</li>
 *       <li>For some adapters, building under Debug_Analysis may not always be possible. If at all
 *           possible, it should be done; if not, exceptions may be made only as 
 *           @em necessary.</li>
 *    </ul>
 *    <li>Project behavior must remain consistent between build configurations.</li>
 *    <ul>
 *       <li>\#ifdef directives dependent on the build mode should be avoided wherever possible. 
 *           Log messages should use LL_Debug instead (this has no notable performance hit). This 
 *           greatly simplifies debugging.</li>
 *    </ul>
 *    <li>C++ TR1 and C++0x features may be used <em>as necessary</em> or where they notably 
 *        improve code quality, readability or functionality.</li>
 *    <li>All functions must use SAL annotations (<code>_In_, _Check_return_, etc</code>) wherever
 *      appropriate.</li>
 *    <ul>
 *       <li>All functions and methods must have annotations on all parameters.</li>
 *    </ul>
 *    <li>All classes, methods, fields, functions, structs, enums, typedefs or other applicable 
 *        code constructs must be documented with their purpose and any vital notes.</li>
 *    <ul>
 *       <li>All documentation must be doxygen compatible and use the same style as the Core.</li>
 *       <li>Any function that throws must be marked with a <code>\@throws</code> note.</li>
 *       <li>Any function with specific requirements must have them noted in SAL annotations (as
 *           well as possible) and with @p \@note or @p \@warning documentation.</li>
 *       <li>All function parameters, global variables, enum members, class members, etc must be
 *           documented. <em>(Note: The project does not fully conform to this requirement 
 *           yet)</em></li>
 *    </ul>
 *    <li>Headers should use the HPP extension, code files CPP.</li>
 *    <li>Headers should include as few files as possible, source files should include only needed
 *        headers.</li>
 * </ul>
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
 *       <li>Use of MFC must be set to "Use Standard Windows Libraries".</li>
 *       <li>Use of ATL must be set to "Not using ATL".</li>
 *       <li>Character set must be set to "Not Set" at this time.</li>
 *       <li>Common language runtime support must be set to "No Common Language Runtime 
 *           support".</li>
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
}
