/*
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
      * @page voodoo_spec_module Voodoo Module Specifications
      *
      * The Voodoo core library is designed to provide a generic framework and certain Voodoo-specific features to help
      * support shaders. To allow additional features without breaking compatibility with other compilers or systems, Voodoo
      * provides a class-based plugin interface. Modules may be added and classes provided by those modules created and used
      * within Voodoo or adapters. To handle loading these modules, a known interface is required with a few specific
      * features.
      *
      * @section voodoo_spec_module_exports Module Exports
      *
      * Each Voodoo module must export a set of 4 functions. These are used by the core to load and query the module's
      * version and what classes it provides. All these functions must be exported by name only, no mangling or decoration
      * of any kind.
      *
      * @subsection voodoo_spec_module_exports_version ModuleVersion
      *
      * @code const Version * ModuleVersion(); @endcode
      *
      * The ModuleVersion function returns a constant Version struct with the module's name, version and various flags. This
      * is used primarily for logging purposes, but can provide basic version-validation.
      *
      * @subsection voodoo_spec_module_exports_count ClassCount
      *
      * @code const uint32_t ClassCount(); @endcode
      *
      * Returns the total number of classes provided by this module. Any class that needs to be created by other code must
      * be listed in this function and the next.
      *
      * @subsection voodoo_spec_module_exports_info ClassInfo
      *
      * @code const wchar_t * ClassInfo(const uint32_t index, Uuid & clsid); @endcode
      *
      * Returns the name and CLSID of the given class. This is used to register the classes in the core ModuleManager when 
      * the module is loaded. The return value is a pointer to the class name or nullptr if the index is invalid, and the
      * clsid parameter is filled with the CLSID or unchanged if the index is invalid.
      * 
      *
      * @subsection voodoo_spec_module_exports_create ClassCreate
      *
      * @code IObject * ClassCreate(const uint32_t index, ICore * pCore); @endcode
      *
      * Creates a new instance of the given class, bound to the provided Core. For classes needing additional data, they can
      * cache and/or use the core.
      *
      * @note Class constructors @em may throw exceptions, but these must be caught within the create function and must not
      *     propagate between modules (this can cause crashes). If an error occurs, nullptr must be returned.
      *
      * @section voodoo_spec_iobject IObject Interface
      *
      * Every class used in Voodoo @em must derive from the IObject interface. The methods provided are vital for object
      * lifetime management and garbage collection. This is performed in a manner very much like COM, with each object
      * managing reference count internally.
      *
      * Every object, except one which implements ICore, must hold a non-incremented reference to its parent. This allows
      * tracking up without causing circular references. The object may both temporarily increment the reference, when it
      * needs a short-term guarantee of validity, or pass the reference (where it may be incremented).
      *
      * @subsection voodoo_spec_iobject_addref IObject::AddRef
      * @code virtual uint32_t IObject::AddRef() const throw() @endcode
      *
      * Increments the internal reference count by 1 and returns the new count. No other effects should be caused.
      *
      * @subsection voodoo_spec_iobject_release IObject::Release
      * @code virtual uint32_t IObject::Release() const throw() @endcode
      *
      * Decrements the internal reference count by 1 and returns the new count. If the new count is 0, the object should be
      * deleted internally (<tt>delete this;</tt>). This provides correct memory allocation and deallocation, safe from
      * compiler or module boundaries.
      * 
      * @subsection voodoo_spec_iobject_query IObject::QueryInterface
      * @code virtual bool IObject::QueryInterface(Uuid & refid, void ** ppOut) const throw() @endcode
      * 
      * Queries the object for a particular interface. 
      * 
      * If refid is an IID and the object implements the interface passed in refid or if refid is the object's CLSID, it
      * must place a pointer to itself providing that interface into ppOut and return true. The pointer must evaluate equal
      * to any other pointer to the same object, regardless of interface. The object's reference count must be incremented 
      * in this case, using the returned pointer.
      * 
      * If ppOut is a nullptr and refid is a null Uuid (all zeroes), refid must be filled must the object's CLSID and the 
      * method must return true. The object's reference count must not change in this case.
      * 
      * If the object does not implement the class or interface in refid, or refid and ppOut are not both null, it must
      * return false. The object's reference count must not change in this case.
      * 
      * All IObject-derived classes must provide interfaces for IObject, their parent interface, and their class. Calls to
      * QueryInterface must be symmetric (if @code QI(A)->B @endcode succeeds, then @code QI(QI(A)->B)->A @endcode must as 
      * well), transitive (if @code QI(QI(A)->B)->C @endcode succeeds, then @code QI(A)->C @endcode must as well), and 
      * reflexive (@code QI(A)->A @endcode must always succeed). 
      * 
      * This is not to say that all objects of type @code A @endcode must behave the same; these guarantees must hold for an
      * object within itself throughout its lifetime. Two objects of type @code A @endcode may be unable to provide an 
      * interface based on internal conditions (for example, if @code A1 @endcode is an IObject queried from an ICore and 
      * @code A2 @endcode is an IObject queried from an IParser, @code A2 @endcode will not be able to provide an interface 
      * to ICore, but must be able to do so for IParser).
      * 
      * For VSCore (which implements ICore and IObject) an example QueryInterface may be:
      * 
      * @code
      * if (!ppOut)
      * {
      *     if (clsid.is_nil())
      *     {
      *         clsid = IID_ICore;
      *         return true;
      *     } else {
      *         return false;
      *     }
      * } else {
      *     if (clsid == IID_IObject)
      *     {
      *         *ppOut = static_cast<const IObject*>(this);
      *     } else if (clsid == IID_ICore) {
      *         *ppOut = static_cast<const ICore*>(this);
      *     } else if (clsid == CLSID_VSCore) {
      *         *ppOut = static_cast<const VSCore*>(this);
      *     } else {
      *         *ppOut = nullptr;
      *         return false;
      *     }
      *     reinterpret_cast<const IObject*>(*ppOut)->AddRef();
      *     return true;
      * }
      * @endcode
      *
      * @subsection voodoo_spec_iobject_tostring IObject::ToString
      * @code virtual String IObject::ToString() const throw() @endcode
      *
      * Returns a formatted string made up of the object's type <em>name</em> (not necessarily UUID) and optionally some
      * data. This string must follow the form:
      *
      * @code
      * ImplName()
      * ImplName(Data, Data)
      * @endcode
      *
      * If no data is provided, the data section inside the parentheses must be empty but the parentheses must be present.
      *
      * @subsection voodoo_spec_iobject_getcore IObject::GetCore
      * @code virtual ICore * IObject::GetCore() const throw() @endcode
      *
      * Returns a <em>non-incremented</em> reference to the object's parent core. This should be stored internally, usually
      * set at construction.
      * 
      * @subsection voodoo_spec_module_throw throw()
      * <b>No method or function in a class or interface derived from IObject may throw exceptions.</b> If error reporting 
      * is necessary, an error value should be returned. This is a necessary safety precaution, as modules may be created
      * with different compilers or languages and it may not be possible to safely propagate exceptions. 
      * 
      * Methods which call other functions which may throw, such as standard library ones, must catch and handle any
      * potential exception and prevent it from propagating.
      *
      * @section voodoo_spec_module_docs Documentation
      *
      * It is generally recommended that module creators provide full documentation for modules, including class names and
      * the interfaces provided.
      *
      * The Voodoo project offers hosting for this documentation on the Github project. Modules may request a wiki page and
      * space for doxygen-generated documents, which will be fully linked into the main project. Modules not wishing to be
      * on the main project may instead request a link to other documentation.
      *
      * @page voodoo_spec_adapter Adapter Specifications
      *
      * The adapter serves as a binding between an unknown (by the core library) graphics API and the Voodoo framework. The
      * adapter is responsible for handling any interaction between the two, performing translation as necessary, and
      * generally polishing the details. In many senses, adapters are regular classes provided by modules and they must
      * follow the @ref voodoo_spec_module as any other module. However, adapter classes must fit a very specific set of
      * behavior rules and perform exactly as expected by the core, and only one adapter object is created at a time.
      *
      * Adapter modules are free to provide other classes, in support of or unrelated to, the adapter itself. Adapter
      * modules may even provide more than one adapter class. It is recommended that the adapter class be of the form
      * <tt>name_Adapter</tt> and each supporting class use <tt>name_classname</tt>, to easily identify them. If the adapter
      * or module is part of the framework, the name scheme extends to the module's name (<tt>Voodoo/name</tt>) and often
      * filename (<tt>Voodoo_name.dll</tt>). Each framework Voodoo adapter is given a short one word somewhat-game-related
      * name.
      *
      * For further details on the behavior required of adapters, please refer to the IAdapter interface documentation.
      *
      * @section voodoo_spec_adapter_cg Cg Context
      *
      * @attention Due to varying needs between adapters regarding the management, and destruction, of the Cg context,
      *     ICore now exposes methods to get and set the active context and no longer creates a context of its own. This
      *     allows more flexibility within adapters, but is a breaking change. Adapters compiled for earlier versions will
      *     find themselves without a context at all.
      * @since 0.2
      *
      * This can avoid some crashes with certain adapters due to context lifespan, and allows adapters to handle cleanup. If
      * at all possible, the Cg context should be created in the adapter's constructor and destroyed in the adapter's
      * destructor, but this is left up to the discretion of the adapter.
      *
      * @warning Adapters may not create any shaders or parameters until they have provided the core with a valid context.
      *     Errors will be returned if this is attempted. To fully support Voodoo features, adapter's must not directly use
      *     the context when the core provides methods to handle a particular feature.
      *
      * @page voodoo_quality Quality (Code and Testing)
      *
      * The Voodoo Shader Framework is designed to work across many games, created across a wide variety of compilers,
      * with numerous graphics libraries, code styles and time periods. To maximize stability and speed, both critical to
      * Voodoo's purpose, all Voodoo projects must meet extremely strict code quality and testing requirements. These are
      * designed to eradicate bugs within Voodoo itself and provide the best user and developer experience possible,
      * especially considering the high degree of complexity within Voodoo and wide scope.
      *
      * Third-party code that is used in Voodoo, or adapters that are not part of the official project, do not need to use
      * these rules, but they can rely on Voodoo to follow them. Any bugs, potential bugs, performance bottlenecks, possible
      * points of improvement or generally questionable code encountered in Voodoo should be reported as soon as possible.
      * Please file an issue with as much information as possible in the Voodoo bug tracker.
      *
      * @section voodoo_quality_code Code Quality
      *
      * To maintain high quality code within Voodoo, a number of systems are used.
      *
      * The simplest, and often most important, is proper use of compiler messages. The framework is compiled with level 4
      * warnings enabled (the strictest standard level, significantly higher than the default) and warnings-as-errors
      * enabled. These measures guarantee the code has no defects the compiler can locate and conforms to all best-practices
      * the compiler is aware of.
      *
      * The second measure, used to discover more complex issues, is full PREfast analysis. All Voodoo framework code is
      * fully annotated with SAL and is submitted to the static analyzer during compilation. Again, any warnings are treated
      * as fatal errors and all rules are enabled. PREfast is capable of catching a number of common errors and using SAL to
      * locate differences between expected and actual behavior.
      *
      * In addition to analysis with PREfast, Voodoo framework code is also run through CppCheck for general errors and
      * KWStyle for style evaluation.
      *
      * All Voodoo framework code has passed both the compiler and PREfast's analysis before release and builds with no
      * warnings from either.
      *
      * @section voodoo_quality_testing Testing
      *
      * To verify the results of the code quality requirements, all Voodoo framework code is run through an extensive set of
      * unit tests and test scenarios and detailed metrics collected. These methods are heavily inspired by the SQLite
      * project and their excellent testing.
      *
      * Unit tests are used to check specific behaviors or code segments, often a single function or object. These use a
      * number of methods to make sure the code behaves as expected, especially providing known-incorrect parameters or data
      * and verifying the error is caught.
      *
      * A more involved test harness is used to run full-scenario tests, where the framework is actually used to apply an
      * effect to a real graphics context, and the results tested. These provide a larger picture, making sure all
      * components work together well.
      *
      * Before running tests, all code is instrumented to provide various pieces of information. The most useful of these is
      * code coverage: what source code has been used in the tests. This is typically measured in two ways: statement
      * coverage and branch coverage. Statement coverage tests that each statement has been run, but does not test all
      * possible outcomes. This means each line of code has probably been run, but offers no guarantees as to actual code
      * used. Branch coverage tests every possible branch of every statement, providing much more complete coverage. Voodoo
      * strives for 100% branch coverage from unit tests and additional coverage of relevant code from scenario tests. This
      * means, simply, that all code is tested in theory and in use before actual in-game testing begins.
      *
      * @section voodoo_quality_docs Documentation
      *
      * The final quality requirement is comprehensive documentation. Voodoo provides a significant amount of documentation,
      * covering everything from technical details of interfaces to expected behavior to graphics theory to quality
      * requirements. This documentation is generated with the doxygen tool and provided in a number of formats, notably on
      * the web at the Github project pages.
      *
      * To make sure documentation is sufficiently complete, the framework must produce no errors or warnings from code
      * during doxygen generation. This performs basic checks, such as requiring every object and function be documented
      * with details for each parameter.
      */
}
