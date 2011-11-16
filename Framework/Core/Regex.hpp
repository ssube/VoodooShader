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

#include "Includes.hpp"
#include "String.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup voodoo_utility Voodoo Utility
     * @{
     */
    
    /**
     * @addtogroup voodoo_utility_regex Voodoo Regex
     * @ingroup voodoo_utility
     * 
     * Voodoo internal regex class, providing constant and compiler-safe regex handling between modules. This internally
     * uses <a href="http://www.boost.org/doc/libs/1_48_0/libs/regex/doc/html/index.html">Boost.Regex</a> in extended-syntax
     * mode for both expressions and match formatting.
     *
     * @warning This @b must use the MSVC wchar_t type, or bad things will happen. <b>GCC does not properly handle
     *      Unicode strings and will not provide valid inputs for these functions.</b>
     *
     */
    class VOODOO_API Regex
    {
        class RegexImpl;

    public:
        /**
         * @addtogroup voodoo_utility_regex_ctor Regex constructors
         * @ingroup voodoo_utility_regex
         * @{
         */
        Regex();

        Regex(String expr);
        /**
         * @}
         */

        ~Regex();
        
        /**
         * @addtogroup voodoo_utility_regex_expr Regex expression
         * @ingroup voodoo_utility_regex
         * @{
         */
        void SetExpr(String expr) throw();

        String GetExpr() const throw();
        /**
         * @}
         * 
         * @addtogroup voodoo_utility_regex_expr Regex expression
         * @ingroup voodoo_utility_regex
         * @{
         */
        RegexMatch Match(const String & string) const throw();
        /**
         * @}
         * 
         * @addtogroup voodoo_utility_regex_expr Regex expression
         * @ingroup voodoo_utility_regex
         * @{
         */
        bool Find(const String & find) const throw();
        /**
         * @}
         * 
         * @addtogroup voodoo_utility_regex_expr Regex expression
         * @ingroup voodoo_utility_regex
         * @{
         */
        String Replace(const String & find, const String & replace) const throw();
        /**
         * @}
         */

    private:
        RegexImpl * m_Impl;
    };

    class VOODOO_API RegexMatch
    {
        class RegexMatchImpl;
        friend class Regex;

    public:
        RegexMatch(const RegexMatch & other);

        ~RegexMatch();

        /**
         * Gets the number of submatches in this match.
         * 
         * @return Total number of matches.
         */
        uint32_t GetCount() const throw();

        /**
         * Check if any matches were found.
         * 
         * @return True if any matches are contained.
         */
        bool IsEmpty() const throw();

        /**
         * Retrieve a particular match within this group.
         * 
         * @return The specified submatch.
         */
        String GetMatch(uint32_t index) const throw();

        /**
         * Formats the found submatches into an output string. This uses the <code>$0</code> syntax to perform formatting,
         * all other text is copied.
         * 
         * @param fmt The format string to use.
         * @return A formatted copy using the submatches.
         */
        String Format(const String & fmt) const throw();

    private:
        RegexMatch();

        RegexMatchImpl * m_Impl;
    };
    /**
     * @}
     */
}
