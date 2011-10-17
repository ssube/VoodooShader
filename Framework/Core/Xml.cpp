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
        class DocumentImpl
        {
        public:
            DocumentImpl()
                : m_Doc()
            {};

        public:
            pugi::xml_document m_Doc;
        };

        class NodeImpl
        {
        public:
            NodeImpl()
                : m_Node()
            {};

        public:
            pugi::xml_node m_Node;
        };

        class AttributeImpl
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

        Bool Document::LoadFile(IFile * pFile)
        {
            return (m_Impl->m_Doc.load_file(pFile->GetPath().GetData()));
        }

        Bool Document::LoadMemory(const void * pBuffer, UInt32 size)
        {
            return (m_Impl->m_Doc.load_buffer(pBuffer, size));
        }

        Bool Document::LoadString(const String & string)
        {
            return (m_Impl->m_Doc.load_buffer(string.GetData(), string.GetLength()));
        }

        Node * Document::GetRoot()
        {
            Node * node = new Node();
            node->m_Impl->m_Node = m_Impl->m_Doc;
            return node;
        }

        String Node::GetName()
        {
            return m_Impl->m_Node.name();
        }

        Bool Node::SetName(const String & name)
        {
            return m_Impl->m_Node.set_name(name.GetData());
        }

        String Node::GetValue()
        {
            return m_Impl->m_Node.value();
        }

        Bool Node::SetValue(const String & value)
        {
            return m_Impl->m_Node.set_value(value.GetData());
        }

        Attribute * Node::GetFirstAttribute()
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

        Attribute * Node::GetLastAttribute()
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

        Node::Node()
        {
            m_Impl = new NodeImpl();
        }

        Node::~Node()
        {
            delete m_Impl;
        }

        Node * Node::GetParent()
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

        Node * Node::GetFirstChild()
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

        Node * Node::GetLastChild()
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

        Node * Node::GetNextSibling()
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

        Node * Node::GetPreviousSibling()
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

        Node * Node::GetChild(const String & name)
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

        Node * Node::GetNextSibling(const String & name)
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

        Node * Node::GetPreviousSibling(const String & name)
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

        String Attribute::GetName()
        {
            return m_Impl->m_Attr.name();
        }

        Bool Attribute::SetName(const String & name)
        {
            return m_Impl->m_Attr.set_name(name.GetData());
        }

        String Attribute::GetString()
        {
            return m_Impl->m_Attr.value();
        }

        Bool Attribute::SetString(const String & value)
        {
            return m_Impl->m_Attr.set_value(value.GetData());
        }

        Int32 Attribute::GetInt32()
        {
            return m_Impl->m_Attr.as_int();
        }

        Bool Attribute::SetInt32(const Int32 & value)
        {
            return m_Impl->m_Attr.set_value(value);
        }

        UInt32 Attribute::GetUInt32()
        {
            return m_Impl->m_Attr.as_uint();
        }

        Bool Attribute::SetUInt32(const UInt32 & value)
        {
            return m_Impl->m_Attr.set_value(value);
        }

        Float Attribute::GetFloat()
        {
            return static_cast<float>(m_Impl->m_Attr.as_double());
        }

        Bool Attribute::SetFloat(const Float & value)
        {
            return m_Impl->m_Attr.set_value(value);
        }

        Bool Attribute::GetBool()
        {
            return m_Impl->m_Attr.as_bool();
        }

        Bool Attribute::SetBool(const Bool & value)
        {
            return m_Impl->m_Attr.set_value(value);
        }

        Attribute * Attribute::GetNextAttribute()
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

        Attribute * Attribute::GetPreviousAttribute()
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
