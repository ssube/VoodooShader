/**************************************************************************************************\
 * This file is part of the Voodoo Shader Framework, a comprehensive shader support library.
 * Copyright (c) 2010-2011 by Sean Sube
 *
 *
 * While the Voodoo Shader Framework, as a whole, is licensed under the GNU General Public license,
 * adapters designed to work with the framework do not constitute derivative works. In the case an
 * adapter needs to use this file (Adapter.hpp) or Meta.hpp in order to work with the Framework, and
 * is not licensed under the GNU GPL or a compatible license, please contact the developer to figure
 * out alternate licensing arrangements.
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
    typedef std::multimap<String, INode *> NodeMap;
    typedef std::map<String, String> AttributeMap;

    /**
     * Defines a simple interface all Voodoo objects must inherit. This
     * interface tracks and handles destruction of objects, especially
     * from dynamic libraries.
     */
    class IParser
        : public IObject
    {
    public:
        virtual IDocument * LoadDocument
        (
            _In_ String filename
        ) = 0;
    };

    class IDocument
        : public IObject
    {
    public:
        virtual INode * GetRoot() = 0;
    };

    class INode
        : public IObject
    {
    public:
        virtual String GetName() = 0;
        virtual String GetValue() = 0;

        virtual NodeMap::iterator GetFirstChild() = 0;
        virtual bool GetNextChild(NodeMap::iterator & iter) = 0;
        virtual NodeMap GetChildren(String regex) = 0;
        virtual INode * GetSingleChild(String regex) = 0;

        virtual AttributeMap::iterator GetFirstAttribute() = 0;
        virtual bool GetNextAttribute(AttributeMap::iterator & iter) = 0;
        virtual String GetAttribute(String name) = 0;
    };
}

#endif /*VOODOO_PARSER_HPP*/