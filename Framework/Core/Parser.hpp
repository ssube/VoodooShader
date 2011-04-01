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

#ifndef VOODOO_PARSER_HPP
#define VOODOO_PARSER_HPP

#include "Meta.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup VoodooCore
     * @{
     */

    /**
     * Provides extensive variable handling and string parsing. 
     * 
     * This is used internally to handle file path resolution and Xml key
     * location.
     * 
     * @sa The @ref vars "variables" documentation has full details on how
     *     parsing is performed.
     */
    class VOODOO_API Parser
    {
    public:
        Parser(Core * core);
        ~Parser();

        void AddVariable(String name, String value);
        void RemoveVariable(String name);

        String ParseString(String input, ParseFlags flags = PF_None);

    private:
        Core * mCore;
        Dictionary mVariables;
    };

/**
 * @page vars Variables
 * <p>
 *    The Voodoo Core provides a simple variable parser for use in path resolution
 *    or other string usage. The syntax and use of these variables is described in 
 *    this page.
 * </p>
 *       
 * @section varssyntax Syntax
 * <p>
 *    Variables are used by surrounding the variable name with '$' symbols.
 *    For each variable, the <code>$varname$</code> portion will be removed and the
 *    value of <code>varname</code> used in place. Variables may be placed at any
 *    position in the path, but may not be nested.
 *    
 * @note The $ character is illegal in filenames, but may be useful when working
 *       with file streams.
 * </p>
 * <p>
 * Examples:
 * @code 
 * localroot = M:\VoodooShader\
 * gempath = M:\VoodooShader\Gem\
 * resourcedir = \resources\
 * 
 * $localroot$\subdir\file.png = M:\VoodooShader\\subdir\file.png
 * $gempath$\shaders\test.cgfx = M:\VoodooShader\Gem\\shaders\test.cgfx
 * $localroot$\$resourcedir$\image.dds = M:\VoodooShader\\\resources\\image.dds
 * @endcode
 * 
 * @note Repeated consecutive slashes will not cause errors with path parsing,
 *       although in some locations they have special meanings. They may cause
 *       errors when using the string for other reasons; calling Parser::ParseString()
 *       with the second parameter set to true will remove repeated slashes.
 * </p>
 *        
 * @subsection varsescape Escaping
 * To insert a $ into the path, place two $ symbols consecutively (an empty 
 * variable name). 
 * @code
 * $localroot$\file.txt:$$Stream = M:\VoodooShader\\file.txt:$Stream
 * @endcode
 * 
 * @section varserror Errors
 * <p>
 *    If a variable cannot be resolved, an error value will be used in place of the
 *    variable. This value is designed to cause the path to fail in any operations.
 *    The error value will replace the <code>$varname$</code> with 
 *    <code>--badvar:varname--</code>.
 * </p>
 * @section varsbuiltin Built-In Variables
 * <p>
 *    A small assortment of built-in variables are provided for use. These represent 
 *    paths or other values that could be difficult to resolve and may vary each run. 
 * </p>
 * @subsection varsbuiltinlocal $localroot$
 * <p>
 *    The local root path is the location of the target executable, that is, the
 *    program that Voodoo is loaded into. This is the absolute path to the file.
 *    This path is retrieved from the Windows API during startup. The value is 
 *    \\-terminated.
 * </p>
 * <p>
 *    Example:
 * @code
 * TargetEXE = H:\Program\Program.exe
 * $localroot$ = H:\Program\
 * @endcode
 * </p>
 * @subsection varsbuiltinglobal $globalroot$ 
 * <p>
 *    The global root path of Voodoo. This is the main Voodoo folder, containing
 *    most global resources and binaries. This path is retrieved from the registry
 *    by the Voodoo loader. The value is \\-terminated.
 * </p>
 * <p>
 *    Example:
 * @code
 * $globalroot$\bin\ = Voodoo binary path
 * @endcode
 * </p>
 * @subsection varsbuiltinrun $runroot$
 * <p>
 *    The running root of Voodoo. This is the directory that the target was
 *    started in (the startup working directory). This path is retrieved from the
 *    Windows API by the loader during startup. This is the most variable of the
 *    builtin variables; it may change each run, depending on how the target
 *    is started. The value is \\-terminated.
 * </p>
 * 
 * @section varsconfig Config Variables
 * <p>
 *    When the filesystem module is loaded, it retrieves any variables from the
 *    config (using the XPath query "/VoodooConfig/Variables/Variable").
 *    These variables must have a @p name attribute and text. They are added to the
 *    filesystem's variable list. The text of these variables may contain other
 *    variables, standard parsing rules apply.
 * </p>
 * @note Variables with identical names will overwrite their previous value. The
 *       builtin variables cannot be overwritten.
 * 
 * @section varsenviron Environment Variables
 * <p>
 *    If a variable name is given that cannot be found in the list of loaded and
 *    builtin variables, it will be assumed to be an environment variable. These
 *    variables are pulled directly from the process' environment and so are
 *    system-dependent. They may or may not be useful.
 * </p>
 * @warning Care should be taken while using environment variables; config variables
 *          are much preferred. Environment variables should only be used when
 *          sharing paths between applications or throughout the system is needed.
 *          
 * @}
 */
}

#endif /*VOODOO_PARSER_HPP*/