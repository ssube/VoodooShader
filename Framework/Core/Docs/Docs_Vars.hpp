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

/** 
 * @page page_vars Variables
 * The Voodoo Core provides a simple variable parser for use in path resolution or other string 
 * usage. The syntax and use of these variables is described in this page.
 *       
 * @section page_vars_sec_syntax Syntax
 * Variables are used by inserting <code>\$(variable)</code> tokens into a string. Each 
 * variable will be removed and replaced with the value currently assigned to it. Variables are 
 * parsed when used, allowing delayed resolving and recursion. Variables are replaced
 * innermost to outermost and left to right (see examples below). 
 * 
 * Four levels of priority are used in parsing. Builtin variables are searched first, then
 * state variables, then regular variables, and finally environment variables supplied by
 * Windows.
 * 
 * @subsection page_vars_ssec_syntaxnames Variable Names
 * Variable names may not contain any symbols (?, !, #, :, etc). Only alphanumerical characters may
 * be used within variable names. Using these may cause errors or undefined behavior; some symbol
 * characters are reserved for future functions and some have special meanings (given below).
 * 
 * Variable names may contain variables, which will be resolved using standard rules (this
 * allows for variable variable names and dynamic string creation).
 * 
 * When a variable is added to the parser, the name is immediately parsed using 
 * @p PF_Lowercase and the result is used as the actual name. This contrasts with values,
 * which are parsed when used.
 * 
 * @subsubsection page_vars_ssec_syntaxspec Special Characters
 * The ':' character indicates state variables, described below. 
 * 
 * The '?' character indicates optional variables, which are not replaced with an error note if 
 * they are not found. It is only recognized as the first character of a variable name; in all
 * other positions it has no special meaning.
 * 
 * The '!' character indicates express variables, which do not have their value parsed (and so
 * may contain other variables without replacements). This can be useful when building variable
 * variables to avoid recursion constraints. It is only recognized as the first character of a 
 * variable name; in all other positions it has no special meaning.
 * 
 * The '$' character indicates an escaped variable name. It may be used to escape variables
 * with '?' or '!' as their first character (the variable <code>$($?var)</code> has a name of
 * <code>?var</code>, instead of being optional). It is only recognized as the first character of a
 * variable name; in all other positions it has no special meaning.
 * 
 * @subsection page_vars_ssec_syntaxvalues Variable Values
 * Variable values may contain any characters and embedded variables. 
 * 
 * Values are parsed when used, using the current state and flags (in contrast to names, which 
 * are parsed when added).
 * 
 * @subsection page_vars_ssec_syntaxstates Variable States & Recursion
 * Each parsing call creates a state block and uses a discrete depth counter.
 * 
 * The depth counter handles recursion and operated very simply. Each time the parser
 * encounters a string that requires parsing, the counter is incremented and passed on. This
 * prevents more than @ref CVoodooParser::VarMaxDepth levels of recursion (8, by default) and 
 * prevents infinite loops. When this limit is reached, variables will no longer be parsed.
 * 
 * State blocks are somewhat more Cthulhic in their logic. When the parser is called, a state
 * block is created and passed on, similar to the depth counter. However, if a variable name
 * containing a ':' character is encountered, the state may change. The variable name in
 * question will be split at the ':' into a new name and value, both of which will be parsed
 * using the current state. The returned values will be inserted into the state block as a new
 * variable (or to overwrite an existing state variable). State variable may then be used in
 * future parsing. While intended for relatively simple dynamic strings, state variables can
 * (with sufficient cleverness and masochism) be used to perform simple logic operations.
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
 * Variable names may also contain variables (as mentioned above). The syntax is identical, but
 * the results are slightly more complex.
 * 
 * @par Examples:
 * (with <code>PF_SingleSlash | PF_BackslashOnly</code>)
 * @code
 * basepath = M:\VoodooShader\
 * adapter = Frost
 * Frost_respath = /resources/frost/
 * 
 * $(basepath)/$($(adapter)_respath) = M:\VoodooShader\resources\frost\
 * @endcode
 * 
 * @note Repeated consecutive slashes will not cause errors with path parsing, although in some 
 *    locations they have special meanings. They may cause errors when using the string for 
 *    other reasons; calling @ref IVoodooParser::Parse() with the @ref PF_SingleSlash flag set 
 *    will attempt to strip these.
 *    
 * Finally, state variables and suppression use a variation on the name syntax.
 * 
 * @par Examples:
 * @code
 * basepath = M:\VoodooShader\
 * game.exe = $(adapter:Gem)
 * Gem_Adapter = Voodoo_Gem.dll
 * _Adapter = Voodoo_Null.dll
 *  
 * $(target) = game.exe
 * $(?$(target)_Adapter) = $(Gem_Adapter) = Voodoo_Gem.dll
 * 
 * $(target) = test.exe
 * $(?$(target)_Adapter) = $(_Adapter) = Voodoo_Null.dll
 * 
 * $($(target)_Adapter) = $($(badvar:test.exe)_Adapter) = $(_Adapter) = Voodoo_Null.dll [and]
 * $(badvar) = test.exe
 * @endcode
 *    
 * @section page_vars_sec_sysvars System Variables
 * Some variables, typically built-ins, are marked as system variables. These may be added once
 * and may not be overwritten or later changed (all other variables may be changed by 
 * overwriting the name). The @ref IVoodooParser::RemoveVariable() method does not remove system 
 * variables.
 * 
 * System variables take priority over all other variables during parsing, including state and
 * regular variables. No error is given if a state or regular variable is created with the same
 * name as a system variable (an error @em is given if a system variable is created), but the
 * second variable will be ignored and only the system variable used.
 *        
 * @section page_vars_sec_flags Flags
 * The variable parser provides a small set of flags to control parsing modes. Flags are listed in
 * the order they operate (PF_SingleSlash will be handled before PF_Lowercase, for example), with
 * the exception of PF_None and PF_VarName which are exclusive with all other flags and cause early
 * parsing and return.
 * 
 * @subsection page_vars_ssec_flagssingleslash PF_SingleSlash
 * Attempts to remove repeated slashes. This considers both forward slash and backslashes and 
 * uses standard MFC string replace functions.
 * 
 * @subsection page_vars_ssec_flagsslashtype PF_SlashOnly & PF_BackslashOnly
 * Replaces all slashes with a single type (forward or back). This may be useful when working with
 * file paths, XPath or other slash-delimited strings.
 * 
 * @subsection page_vars_ssec_flagscase PF_Lowercase & PF_Uppercase
 * Transforms the string into a specific case. All alphabetic characters (a-z and A-Z) will be
 * replaced with the given case.
 * 
 * @subsection page_vars_ssec_flagsname PF_VarName
 * This is a meta-flag, forcing a set of flags and special behavior appropriate for variable names. 
 * At the moment, this simply forces PF_Lowercase.
 * 
 * @section page_vars_sec_errors Errors
 * If a variable cannot be resolved, an error value will be used in place of the variable. This 
 * value is designed to cause the path to fail in any operations. The error value will replace 
 * @p varname with <code>$(badvar:varname)</code>. This sets an internal state variable, which
 * contains the name of the last error variable.
 * 
 * @section page_vars_sec_builtin Built-In Variables
 * A small assortment of built-in variables are provided for use. These represent paths or 
 * other values that could be difficult to resolve and may vary each run. Various versions of 
 * the Core may add built in variables, these will be marked here with a note of the earliest 
 * version providing them. Adapters and other modules may also add system variables, but these
 * can never overwrite other system variables.
 * 
 * @note Built-in variables will not be present if the parser was created manually or externally
 *    (not automatically by a core).
 * 
 * @subsection page_vars_ssec_localroot $(localroot)
 * The local root path is the location of the target executable, that is, the program that 
 * Voodoo is loaded into. This is the absolute path to the file. This path is retrieved from 
 * the Windows API during startup. The value is \\-terminated.
 * 
 * @par Example:
 * @code
 * TargetEXE = H:\Program\Program.exe
 * $(localroot) = H:\Program\
 * @endcode
 * 
 * @subsection page_vars_ssec_globalroot $(globalroot)
 * The global root path of Voodoo. This is the main Voodoo folder, containing most global 
 * resources and binaries. This path is retrieved from the registry by the Voodoo loader. The 
 * value is \\-terminated.
 * 
 * @par Example:
 * @code
 * $(globalroot)\bin\ = Voodoo binary path
 * @endcode
 * 
 * @subsection page_vars_ssec_runroot $(runroot)
 * The running root of Voodoo. This is the directory that the target was started in (the 
 * startup working directory). This path is retrieved from the Windows API by the loader during 
 * startup. This is the most variable of the builtin variables; it may change each run, 
 * depending on how the target is started. The value is \\-terminated.
 * 
 * @subsection page_vars_ssec_target $(target)
 * The filename of the target application. This does not include the path (found in 
 * <code>\$(localroot)</code>), only the name. To help use this for variable varnames, this is
 * forced to all lowercase. You can use it to change the config settings based on the running
 * executable, like so:
 * 
 * @par Example:
 * Two adapter names, for different games. For this example, the running target is 
 * <code>nwmain.exe</code>.
 * @code
 * globalroot = M:\VoodooShader\
 * target = nwmain.exe
 * 
 * morrowind.exe = Gem
 * nwmain.exe = Frost
 * adapter = $($(target))
 * 
 * $(adapter)_Adapter = Frost_Adapter
 * $(globalroot)\resources\$(adapter) = M:\VoodooShader\resources\Frost
 * @endcode
 * The variable used in adapter (and so, the value) change depending on the value of target.
 * 
 * @section page_vars_sec_config Config Variables
 * When the filesystem module is loaded, it retrieves any variables from the config (using the 
 * XPath query "/VoodooConfig/Variables/Variable"). These variables must have a @p name 
 * attribute and text. They are added to the variable list, but not parsed until used.
 * 
 * @note Variables with identical names will overwrite their previous value. The built-in 
 *    variables cannot be overwritten.
 * 
 * @section page_vars_sec_environ Environment Variables
 * If a variable name is given that cannot be found in the list of loaded and built-in 
 * variables, it will be assumed to be an environment variable. These variables are pulled 
 * directly from the process' environment and so are system-dependent. They may or may not be 
 * useful.
 * 
 * @warning Care should be taken while using environment variables; config variables are much 
 *    preferred. Environment variables should only be used when sharing paths between 
 *    applications or throughout the system is needed.
 * @}
 */
