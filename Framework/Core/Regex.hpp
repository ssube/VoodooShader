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

#include "String.hpp"

#include "Includes.hpp"

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
     * @related String
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

        /**
         * Creates an empty regular expression.
         */
        Regex();
        
        /**
         * Creates a regular expression from the given string, using extended syntax.
         *
         * @param expr The string to use.
         */
        Regex(const String & expr);
        /**
         * @}
         */

        ~Regex();
        
        /**
         * @addtogroup voodoo_utility_regex_expr Regex expression
         * @ingroup voodoo_utility_regex
         * @{
         */

        /**
         * Sets the internal expression and recompiles.
         *
         * @param expr The string to use.
         */
        void SetExpr(const String & expr) ;

        /**
         * Retrieve the current expression.
         *
         * @return The expression string.
         */
        String GetExpr() CONST;

        /**
         * @}
         *
         * @addtogroup voodoo_utility_regex_expr Regex expression
         * @ingroup voodoo_utility_regex
         * @{
         */

        /**
         * Attempt to match the given string against the stored expression.
         *
         * @param string The string to search.
         * @return A RegexMatch for all match groups.
         */
        RegexMatch Match(const String & string) CONST;

        /**
         * @}
         *
         * @addtogroup voodoo_utility_regex_expr Regex expression
         * @ingroup voodoo_utility_regex
         * @{
         */
        
        /**
         * Test if the given string is matched by the expression.
         *
         * @param find The string to search.
         * @return True if the expression matches the string in full.
         */
        bool Find(const String & find) CONST;

        /**
         * @}
         *
         * @addtogroup voodoo_utility_regex_expr Regex expression
         * @ingroup voodoo_utility_regex
         * @{
         */
        
        /**
         * Perform a find and replace on all segments which this regex matches. Matches are replaced using the same rules as
         * RegexMatch::Replace().
         *
         * @param find The string to search.
         * @param replace The replace format string.
         * @return A copy of find with all matches replaced.
         */
        String Replace(const String & find, const String & replace) CONST;

        /**
         * @}
         */

    private:
        RegexImpl * m_Impl;
    };
    
    /**
     * @addtogroup voodoo_utility_regex Voodoo Regex
     * @ingroup voodoo_utility
     *
     * Voodoo internal regex match class, encapsulating matches found by @ref Regex. This class provides methods to read
     * each match group, or use them as arguments to a formatted string.
     */
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
        uint32_t GetCount() CONST;

        /**
         * Check if any matches were found.
         *
         * @return True if any matches are contained.
         */
        bool IsEmpty() CONST;

        /**
         * Retrieve a particular match within this group.
         *
         * @return The specified submatch.
         */
        String GetMatch(uint32_t index) CONST;

        /**
         * Formats the found submatches into an output string. This uses the <code>$0</code> syntax to perform formatting,
         * all other text is copied.
         *
         * @param fmt The format string to use.
         * @return A formatted copy using the submatches.
         */
        String Format(const String & fmt) CONST;

    private:
        RegexMatch();

        RegexMatchImpl * m_Impl;
    };

    /**
     * @}
     */
}
