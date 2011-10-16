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

#include <regex>

#include "Regex.hpp"

namespace VoodooShader
{
    class RegexImpl
    {
    public:
        RegexImpl()
            : m_Expr(), m_Regex()
        { };

        RegexImpl(String expr)
            : m_Expr(expr), m_Regex(expr)
        { };

    public:
        String m_Expr;
        std::regex m_Regex;
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

    String Regex::GetExpr()
    {
        return m_Impl->m_Expr;
    }

    Bool Regex::Match(String string)
    {
        return regex_match(string, m_Impl->m_Regex);
    }
}
