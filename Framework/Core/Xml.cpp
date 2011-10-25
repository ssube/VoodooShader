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

#include <string>
#include "pugixml.hpp"

#include "IFilesystem.hpp"
#include "Xml.hpp"

namespace VoodooShader
{
    namespace Xml
    {
        class Document::DocumentImpl
        {
        public:
            DocumentImpl()
                : m_Doc()
            {};

        public:
            pugi::xml_document m_Doc;
        };

        class Node::NodeImpl
        {
        public:
            NodeImpl()
                : m_Node()
            {};

        public:
            pugi::xml_node m_Node;
        };

        class Attribute::AttributeImpl
        {
        public:
            AttributeImpl()
                : m_Attr()
            {};

        public:
            pugi::xml_attribute m_Attr;
        };

        Document::Document()
        {
            m_Impl = new DocumentImpl();
        }

        Document::~Document()
        {
            delete m_Impl;
        }

        bool Document::LoadFile(IFile * pFile)
        {
            return (m_Impl->m_Doc.load_file(pFile->GetPath().GetData()));
        }

        bool Document::LoadFile(const String & filename)
        {
            return (m_Impl->m_Doc.load_file(filename.GetData()));
        }

        bool Document::LoadMemory(const void * pBuffer, uint32_t size)
        {
            return (m_Impl->m_Doc.load_buffer(pBuffer, size));
        }

        bool Document::LoadString(const String & string)
        {
            return (m_Impl->m_Doc.load_buffer(string.GetData(), string.GetLength()));
        }

        Node Document::GetRoot() const
        {
            Node node;
            node.m_Impl->m_Node = m_Impl->m_Doc;
            return node;
        }

        Node::Node()
        {
            m_Impl = new NodeImpl();
        }

        Node::~Node()
        {
            delete m_Impl;
        }

        String Node::GetName() const
        {
            return m_Impl->m_Node.name();
        }

        String Node::GetValue() const
        {
            return m_Impl->m_Node.value();
        }

        Attribute Node::GetFirstAttribute()
        {
            Attribute * pAttr = nullptr;
            auto attr = m_Impl->m_Node.first_attribute();
            if (attr)
            {
                pAttr = new Attribute();
                pAttr->m_Impl->m_Attr = attr;
            }
            return pAttr;
        }

        Attribute Node::GetLastAttribute()
        {
            Attribute * pAttr = nullptr;
            auto attr = m_Impl->m_Node.last_attribute();
            if (attr)
            {
                pAttr = new Attribute();
                pAttr->m_Impl->m_Attr = attr;
            }
            return pAttr;
        }

        Node Node::GetParent() const
        {
            Node * pNode = nullptr;
            auto node = m_Impl->m_Node.parent();
            if (node)
            {
                pNode = new Node();
                pNode->m_Impl->m_Node = node;
            }
            return pNode;
        }

        Node Node::GetFirstChild() const
        {
            Node * pNode = nullptr;
            auto node = m_Impl->m_Node.first_child();
            if (node)
            {
                pNode = new Node();
                pNode->m_Impl->m_Node = node;
            }
            return pNode;
        }

        Node Node::GetLastChild() const
        {
            Node * pNode = nullptr;
            auto node = m_Impl->m_Node.last_child();
            if (node)
            {
                pNode = new Node();
                pNode->m_Impl->m_Node = node;
            }
            return pNode;
        }

        Node Node::GetNextSibling() const
        {
            Node * pNode = nullptr;
            auto node = m_Impl->m_Node.next_sibling();
            if (node)
            {
                pNode = new Node();
                pNode->m_Impl->m_Node = node;
            }
            return pNode;
        }

        Node Node::GetPreviousSibling() const
        {
            Node * pNode = nullptr;
            auto node = m_Impl->m_Node.previous_sibling();
            if (node)
            {
                pNode = new Node();
                pNode->m_Impl->m_Node = node;
            }
            return pNode;
        }

        Node Node::GetChild(const String & name) const
        {
            Node * pNode = nullptr;
            auto node = m_Impl->m_Node.child(name);
            if (node)
            {
                pNode = new Node();
                pNode->m_Impl->m_Node = node;
            }
            return pNode;
        }

        Node Node::GetNextSibling(const String & name) const
        {
            Node * pNode = nullptr;
            auto node = m_Impl->m_Node.next_sibling(name);
            if (node)
            {
                pNode = new Node();
                pNode->m_Impl->m_Node = node;
            }
            return pNode;
        }

        Node Node::GetPreviousSibling(const String & name) const
        {
            Node * pNode = nullptr;
            auto node = m_Impl->m_Node.previous_sibling(name);
            if (node)
            {
                pNode = new Node();
                pNode->m_Impl->m_Node = node;
            }
            return pNode;
        }

        Attribute::Attribute()
        {
            m_Impl = new AttributeImpl();
        }

        Attribute::~Attribute()
        {
            delete m_Impl;
        }

        String Attribute::GetName() const
        {
            return m_Impl->m_Attr.name();
        }

        String Attribute::GetString() const
        {
            return m_Impl->m_Attr.value();
        }

        int32_t Attribute::GetInt32() const
        {
            return m_Impl->m_Attr.as_int();
        }

        uint32_t Attribute::GetUInt32() const
        {
            return m_Impl->m_Attr.as_uint();
        }

        float Attribute::GetFloat() const
        {
            return static_cast<float>(m_Impl->m_Attr.as_double());
        }

        bool Attribute::GetBool() const
        {
            return m_Impl->m_Attr.as_bool();
        }

        Attribute * Attribute::GetNextAttribute() const
        {
            Attribute * pAttr = nullptr;
            auto attr = m_Impl->m_Attr.next_attribute();
            if (attr)
            {
                pAttr = new Attribute();
                pAttr->m_Impl->m_Attr = attr;
            }
            return pAttr;
        }

        Attribute * Attribute::GetPreviousAttribute() const
        {
            Attribute * pAttr = nullptr;
            auto attr = m_Impl->m_Attr.previous_attribute();
            if (attr)
            {
                pAttr = new Attribute();
                pAttr->m_Impl->m_Attr = attr;
            }
            return pAttr;
        }
    }
}
