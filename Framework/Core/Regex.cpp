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

#include <string>
#include <regex>

#include "Regex.hpp"

namespace VoodooShader
{
    class Regex::RegexImpl
    {
    public:
        RegexImpl()
            : m_Regex(), m_Expr()
        { };

        RegexImpl(String expr)
            : m_Regex(expr.GetData()), m_Expr(expr)
        { };

    public:
        std::wregex m_Regex;
        String m_Expr;
    };

    Regex::Regex()
    {
        m_Impl = new RegexImpl();
    }

    Regex::Regex(String expr)
    {
        m_Impl = new RegexImpl(expr);
    }

    Regex::~Regex()
    {
        delete m_Impl;
    }

    void Regex::SetExpr(String expr)
    {
        m_Impl->m_Expr = expr;
    }

    String Regex::GetExpr() const
    {
        return m_Impl->m_Expr;
    }

    bool Regex::Match(const String & string) const
    {
        return std::regex_match(string.ToString(), m_Impl->m_Regex);
    }

    bool Regex::Find(const String & find) const
    {
        return std::regex_search(find.ToString(), m_Impl->m_Regex);
    }

    String Regex::Replace(const String & find, const String & replace) const
    {
        return std::regex_replace(find.ToString(), m_Impl->m_Regex, replace.ToString());
    }
}
