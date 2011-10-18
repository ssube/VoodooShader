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

namespace VoodooShader
{
    namespace Xml
    {
        class DocumentImpl;
        class NodeImpl;
        class AttributeImpl;

        class VOODOO_API Document
        {
        public:
            // Ctors
            Document();
            ~Document();

            // Loading
            Bool LoadFile(IFile * pFile);
            Bool LoadMemory(const void * pBuffer, UInt32 size);
            Bool LoadString(const String & string);

            // Saving
            Bool SaveFile(IFile * pFile);
            String SaveString();

            // Root
            Node GetRoot() const;

        private:
            DocumentImpl * m_Impl;
        };

        class VOODOO_API Node
        {
            friend class Document;

        public:
            // Ctors
            ~Node();

            // Get/Set
            String GetName() const;
            Bool SetName(const String & name);
            String GetValue() const;
            Bool SetValue(const String & value);

            // Attributes
            Attribute GetFirstAttribute();
            Attribute GetLastAttribute();

            // Traversal
            Node GetParent() const;
            Node GetFirstChild() const;
            Node GetLastChild() const;
            Node GetNextSibling() const;
            Node GetPreviousSibling() const;

            // Named traversal
            Node GetChild(const String & name) const;
            Node GetNextSibling(const String & name) const;
            Node GetPreviousSibling(const String & name) const;

            // XPath accessors
            XPath::Node GetSingleNode(const String & expr) const;
            XPath::NodeSet GetNodes(const String & expr) const;

        private:
            Node();

            NodeImpl * m_Impl;
        };

        class VOODOO_API Attribute
        {
            friend class Node;

        public:
            // *tors
            ~Attribute();

            // Name
            String GetName() const;
            Bool SetName(const String & name);

            // Value
            String GetString() const;
            Bool SetString(const String & value);
            Int32 GetInt32() const;
            Bool SetInt32(const Int32 & value);
            UInt32 GetUInt32() const;
            Bool SetUInt32(const UInt32 & value);
            Float GetFloat() const;
            Bool SetFloat(const Float & value);
            Bool GetBool() const;
            Bool SetBool(const Bool & value);

            // Traversal
            Attribute GetNextAttribute() const;
            Attribute GetPreviousAttribute() const;

        private:
            Attribute();

            AttributeImpl * m_Impl;
        };
    }
}
