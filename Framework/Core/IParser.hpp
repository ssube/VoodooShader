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

#include "Includes.hpp"

#include "String.hpp"

#include "IObject.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup VoodooCore @{
     */

    /**
     * Provides extensive variable handling and string parsing.
     */
    class IParser
    {
    public:
        IParser(_In_ ICore * const pCore) throw();

        virtual ~IParser(void) throw();
        
        /**
         * Add a reference to this object.
         * 
         * @return The new reference count.
         */
        virtual int32_t AddRef(void) const throw();

        /**
         * Release a reference from this object.
         * 
         * @return The new reference count.
         */
        virtual int32_t Release(void) const throw();

        /**
         * Get the name of this object.
         * 
         * @return The name.
         */
        virtual String ToString(void) const throw();

        /** 
         * Get the core this object was associated with. 
         * 
         * @return The core.
         */
        virtual ICore * GetCore(void) const throw();

        /**
         * Adds a variable to the internal dictionary. 
         * 
         * @param name The variable name (may contain variables, they will be resolved immediately). 
         * @param value The variable's value (may contain variables, they will be resolved when this variable is used).
         * @param system Marks the variable as a system variable. These cannot be changed or removed.
         */
        virtual void Add(_In_ const String & name, _In_ const String & value, _In_ VariableType type = VT_Normal) throw();

        /**
         * Removes a variable from the internal dictionary. 
         * 
         * @param name The variable name (may contain variables, they will be resolved immediately).
         */
        virtual void Remove(_In_ const String & name) throw();

        /**
         * Parses a string, replacing any variables with their values. Variables are resolved when found, so it is 
         * possible to have variables within variables and trigger recursion. 
         *
         * @sa @ref varsyntax for details on how variables work
         */
        virtual String Parse(_In_ String input, _In_ ParseFlags flags = PF_None) const throw();

        static const int VarMaxDepth = 8;
        static const char VarDelimPre    = '$';
        static const char VarDelimStart  = '(';
        static const char VarDelimEnd    = ')';
        static const char VarMarkerState = ':';
        static const char VarMarkerRaw   = '!';
        static const char VarMarkerSupp  = '?';

    private:
        String ParseStringRaw(_In_ String input, _In_ ParseFlags flags, _In_ int depth, _In_ Dictionary & state) const;

        mutable int32_t m_Refs;
        ICore * m_Core;
        Dictionary m_Variables;
        Dictionary m_SysVariables;
    };

    /**
     * @} 
     * 
     * @page varsyntax Variables 
     * The Voodoo core provides a simple variable parser for use in path resolution or other string usage. The 
     * syntax and use of these variables is described in this page. 
     * 
     * @section varssyntax Syntax 
     * Variables are used by inserting <code>\$(variable)</code> tokens into a string. Each variable will be removed 
     * and replaced with the value currently assigned to it. Variables are parsed when used, allowing delayed 
     * resolution and recursion. Variables are replaced innermost to outermost and left to right (see examples below). 
     * Four levels of priority are used in parsing. Builtin variables are searched first, then state variables, then 
     * regular variables, and finally environment variables supplied by the system. 
     * 
     * @subsection varssyntaxnames Variable Names 
     * Variable names may contain any characters but ':' and may start with any character but '?' or '!'. These 
     * characters have special meaning and will be stripped or will modify the name before it is used. Variable names 
     * may contain variables, which will be resolved using standard rules (this allows for variable variable names and
     * dynamic string creation). When a variable is added to the parser, the name is immediately parsed using 
     * @p PF_Lowercase and the result is used as the actual name. This contrasts with values, which are parsed when 
     * used. 
     * 
     * @subsubsection varssyntaxnamesspec Special Characters 
     * @li The ':' character indicates state variables, described below. 
     * @li The '?' character indicates optional variables, which are not replaced with an error note if they are not 
     *     found.
     * @li The '!' character indicates express variables, which do not have their value parsed (and so may contain 
     *     other  variables without replacements). This can be useful when building variable variables to avoid 
     *     recursion constraints. 
     * @li The remaining listed characters are reserved. 
     * 
     * @subsection varssyntaxvalue Variable Values
     * Variable values may contain any characters and embedded variables. Values are parsed when used, using the 
     * current state and flags (in contrast to names, which are parsed when added). 
     * 
     * @subsection varssyntexstate Variable States & Recursion 
     * Each parsing call creates a state block and uses a discrete depth counter. The depth counter handles recursion 
     * and operated very simply. Each time the parser encounters a string that requires parsing, the counter is
     * incremented and passed on. This prevents more than @ref IParser::VarMaxDepth levels of recursion (8, by default)
     * and infinite loops. When this limit is reached, variables will not be parsed.
     * 
     * State blocks are somewhat more Cthulhic in their logic. When the parser is called, a state block is created and
     * passed on, similar to the depth counter. However, if a variable name containing a ':' character is encountered,
     * the state may change. The variable name in question will be split at the ':' into a new name and value, both of
     * which will be parsed using the current state. The returned values will be inserted into the state block as a new
     * variable (or to overwrite an existing state variable). State variable may then be used in future parsing. While     
     * intended for relatively simple dynamic strings, state variables can (with sufficient cleverness and masochism) be 
     * used to perform simple logic operations. 
     * 
     * @par Examples: 
     * @code 
     * localroot = M:\VoodooShader\ 
     * gempath = M:\VoodooShader\Gem\ 
     * resourcedir = \resources\ 
     * 
     * $(localroot)\subdir\file.png = M:\VoodooShader\\subdir\file.png 
     * $(gempath)\shaders\test.cgfx = M:\VoodooShader\Gem\\shaders\test.cgfx 
     * $(localroot)\$(resourcedir)\image.dds = M:\VoodooShader\\\resources\\image.dds 
     * @endcode 
     * 
     * Variable names may also contain variables (as mentioned above). The syntax is identical, but the results are
     * slightly more complex. 
     * 
     * @par Examples: (with <code>PF_SingleSlash | PF_BackslashOnly</code>) 
     * @code 
     * basepath = M:\VoodooShader\ 
     * adapter = Frost
     * Frost_respath = /resources/frost/ 
     * 
     * $(basepath)/$($(adapter)_respath) = M:\VoodooShader\resources\frost\ 
     * @endcode 
     *
     * @note Repeated consecutive slashes will not cause errors with path parsing, although in some locations they 
     *     have special meanings. They may cause errors when using the string for other reasons; calling 
     *     @ref IParser::Parse() with the @ref PF_SingleSlash flag set will attempt to strip these. Finally, state 
     *     variables and suppression use a variation on the name syntax. 
     *     
     * @par Examples: 
     * @code 
     * basepath = M:\VoodooShader\ 
     * game.exe = $(adapter:Gem) 
     * Gem_Adapter = Voodoo_Gem.dll 
     * _Adapter = Voodoo_nullptr.dll
     * 
     * $(target) = game.exe 
     * $(?$(target)_Adapter) = $(Gem_Adapter) = Voodoo_Gem.dll
     * 
     * $(target) = test.exe 
     * $(?$(target)_Adapter) = $(_Adapter) = Voodoo_nullptr.dll
     * 
     * $($(target)_Adapter) = $(--badvar:test.exe--_Adapter) = --badvar:--badvar:test.exe--_Adapter-- 
     * @endcode 
     *
     * @section varssys System Variables 
     * Some variables, typically built-ins, are marked as system variables. These may be added once and may not be 
     * overwritten or later changed (all other variables may be changed by overwriting the name). The 
     * @ref IParser::Remove() method does not remove system variables. 
     * 
     * @section varsflags Flags 
     * The variable parser provides a small set of flags to control parsing modes. 
     *
     * @subsection varsflagssingleslash PF_SingleSlash 
     * Removes repeated slashes, both forward and back. This processes each stage of the string before it is returned
     * up a level, and is typically very reliable (even if variables place new slashes).
     * 
     * @subsection varsflagsslashtype PF_SlashOnly & PF_BackslashOnly 
     * Replaces all slashes with a single type (forward or back). This may be necessary; XPath syntax, for example, 
     * uses forward slashes while Windows paths tend to prefer backslashes. 
     * 
     * @subsection varsflagscase PF_Lowercase & PF_Uppercase
     * Transforms the string into a specific case. All alphabetic characters (a-z and A-Z) will be replaced with the 
     * given case. 
     * 
     * @subsection varsflagsstateblock PF_RetainState 
     * This flag has no effect 
     * 
     * @section varserror Errors 
     * If a variable cannot be resolved, an error value will be used in place of the variable. This value is designed
     * to cause the path to fail in any operations. The error value will replace @p varname with 
     * <code>--badvar:varname--</code>. 
     * 
     * @section varsbuiltin Built-In Variables 
     * A small assortment of built-in variables are provided for use. These represent system or environment paths that
     * may be important. These will be marked here with a note of the earliest version of the framework in which they
     * are provided. Adapters and other modules may also add system variables, but these can never overwrite other 
     * system variables. 
     * 
     * @subsection varsbuiltinlocal $(localroot) 
     * The local root path is the location of the target executable, that is, the program that Voodoo is loaded into. 
     * This is the absolute path to the file. This path is retrieved from the Windows API during startup. The value is
     * \\-terminated. 
     * 
     * @par Example: 
     * @code 
     * TargetEXE = H:\Program\Program.exe
     * $(localroot) = H:\Program\ 
     * @endcode 
     * 
     * @subsection varsbuiltinglobal $(globalroot)
     * The global root path of Voodoo. This is the main Voodoo folder, containing most global resources and binaries. 
     * This path is retrieved from the registry by the Voodoo loader. The value is \\-terminated. 
     * 
     * @par Example: 
     * @code
     * $(globalroot)\bin\ = Voodoo binary path 
     * @endcode 
     * 
     * @subsection varsbuiltinrun $(runroot) 
     * The running root of Voodoo. This is the directory that the target was started in (the startup working 
     * directory). This path is retrieved from the Windows API by the loader during startup. This is the most variable
     * of the builtin variables; it may change each run, depending on how the target is started. The value is
     * \\-terminated. 
     * 
     * @subsection varsbuiltintarget $(target) 
     * The filename of the target application. This does not include the path (found in <code>\$(localroot)</code>), 
     * only the name. To help use this for variable varnames, this is forced to all lowercase. You can use it to change
     * the config settings based on the running executable, like so: 
     * 
     * @par Example: Two adapter names, for different games. For this example, the running target is
     * <code>nwmain.exe</code>.
     * @code 
     * globalroot = M:\VoodooShader\ 
     * target = nwmain.exe 
     * morrowind.exe = Gem 
     * nwmain.exe = Frost 
     * adapter = $($(target))
     * 
     * $(adapter)_Adapter = Frost_Adapter 
     * $(globalroot)\resources\$(adapter) = M:\VoodooShader\resources\Frost 
     * @endcode 
     * 
     * The variable used in adapter (and so, the value) change depending on the value of target. 
     * 
     * @section varsconfig Config Variables 
     * When the core loads, it retrieves any variables from the config (using the XPath query 
     * "/VoodooConfig/Variables/Variable"). These variables must have a @p name attribute and text. They are added to 
     * the variable list, but not parsed until used. 
     * 
     * @note Variables with identical names will overwrite their previous value. The built-in variables cannot be
     *     overwritten. 
     *     
     * @section varsenviron Environment Variables 
     * If a variable name is given that cannot be found in the list of loaded and built-in variables, it will be 
     * assumed to be an environment variable. These variables are pulled directly from the process' environment and so 
     * are system-dependent. They may or may not be useful. 
     * 
     * @warning Care should be taken while using environment variables; config variables are much preferred. Environment 
     *    variables should only be used when sharing paths between applications or throughout the system is needed. 
     */

    /**
     * @}
     */
}
