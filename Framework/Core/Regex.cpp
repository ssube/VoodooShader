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

#include <string>

#pragma warning(push)
#pragma warning(disable: 6334 6011)
#include <boost/regex.hpp>
#pragma warning(pop)

#include "Regex.hpp"

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

    Regex::Regex(const String & expr)
    {
        m_Impl = new RegexImpl(expr);
    }

    Regex::~Regex()
    {
        delete m_Impl;
    }

    void Regex::SetExpr(const String & expr)
    {
        m_Impl->m_Regex.set_expression(expr.ToString());
    }

    String Regex::GetExpr() const
    {
        return String(m_Impl->m_Regex.str());
    }

    RegexMatch Regex::Match(const String & string) const
    {
        RegexMatch match;
        boost::wsmatch mr;

        if (boost::regex_match(string.ToStdString(), mr, m_Impl->m_Regex))
        {
            match.m_Impl->m_Match = mr;
        }

        return match;
    }

    bool Regex::Find(const String & find) const
    {
        return boost::regex_search(find.ToStdString(), m_Impl->m_Regex);
    }

    String Regex::Replace(const String & find, const String & replace) const
    {
        return boost::regex_replace(find.ToStdString(), m_Impl->m_Regex, replace.ToStdString());
    }

    RegexMatch::RegexMatch()
    {
        m_Impl = new RegexMatchImpl();
    }

    RegexMatch::RegexMatch(const RegexMatch & other)
    {
        m_Impl = new RegexMatchImpl(other.m_Impl->m_Match);
    }

    RegexMatch::~RegexMatch()
    {
        delete m_Impl;
    }

    uint32_t RegexMatch::GetCount() const
    {
        return m_Impl->m_Match.size();
    }

    bool RegexMatch::IsEmpty() const
    {
        return m_Impl->m_Match.empty();
    }

    String RegexMatch::GetMatch(uint32_t index) const
    {
        return m_Impl->m_Match[index];
    }

    String RegexMatch::Format(const String & fmt) const
    {
        return m_Impl->m_Match.format(fmt.ToStdString(), boost::format_all);
    }
}
