/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2013 by Sean Sube
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

#include "VoodooFramework.hpp"
// Boost
#pragma warning(push,3)
#pragma warning(disable: 6334 6011)
#include <boost/regex.hpp>
// System
#include <string>
#pragma warning(pop)

namespace VoodooShader
{
    class Regex::RegexImpl
    {
    public:
        RegexImpl()
            : m_Regex()
        { };

        RegexImpl(String expr)
            : m_Regex(expr.GetData(), boost::regex::extended)
        { };

    public:
        boost::wregex m_Regex;
    };

    class RegexMatch::RegexMatchImpl
    {
    public:
        RegexMatchImpl()
            : m_Match()
        { };

        RegexMatchImpl(boost::wsmatch & match)
            : m_Match(match)
        { };

    public:
        boost::wsmatch m_Match;
    };

    Regex::Regex()
    {
        m_Impl = new RegexImpl();
    }

    Regex::Regex(_In_ CONST String & expr)
    {
        m_Impl = new RegexImpl(expr);
    }

    Regex::~Regex()
    {
        delete m_Impl;
        m_Impl = nullptr;
    }

    void Regex::SetExpr(_In_ CONST String & expr)
    {
        VOODOO_CHECK_IMPL;

        m_Impl->m_Regex.set_expression(expr.ToString());
    }

    String Regex::GetExpr() CONST
    {
        VOODOO_CHECK_IMPL;

        return String(m_Impl->m_Regex.str());
    }

    RegexMatch Regex::Match(_In_ CONST String & string) CONST
    {
        VOODOO_CHECK_IMPL;

        RegexMatch match;
        boost::wsmatch mr;

        if (boost::regex_match(string.ToString(), mr, m_Impl->m_Regex))
        {
            match.m_Impl->m_Match = mr;
        }

        return match;
    }

    bool Regex::Find(_In_ CONST String & find) CONST
    {
        VOODOO_CHECK_IMPL;

        return boost::regex_search(find.ToString(), m_Impl->m_Regex);
    }

    String Regex::Replace(_In_ CONST String & find, _In_ CONST String & replace) CONST
    {
        VOODOO_CHECK_IMPL;

        return boost::regex_replace(find.ToString(), m_Impl->m_Regex, replace.ToString());
    }

    RegexMatch::RegexMatch()
    {
        m_Impl = new RegexMatchImpl();
    }

    RegexMatch::RegexMatch(_In_ CONST RegexMatch & other)
    {
        m_Impl = new RegexMatchImpl(other.m_Impl->m_Match);
    }

    RegexMatch::~RegexMatch()
    {
        delete m_Impl;
        m_Impl = nullptr;
    }

    int32_t RegexMatch::GetCount() CONST
    {
        VOODOO_CHECK_IMPL;

        return (int32_t)m_Impl->m_Match.size();
    }

    bool RegexMatch::IsEmpty() CONST
    {
        VOODOO_CHECK_IMPL;

        return m_Impl->m_Match.empty();
    }

    String RegexMatch::GetMatch(_In_ CONST int32_t index) CONST
    {
        VOODOO_CHECK_IMPL;

        return m_Impl->m_Match[index];
    }

    String RegexMatch::Format(_In_ CONST String & fmt) CONST
    {
        VOODOO_CHECK_IMPL;

        return m_Impl->m_Match.format(fmt.ToString(), boost::format_all);
    }
}
