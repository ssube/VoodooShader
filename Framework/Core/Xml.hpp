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
            Node * GetRoot();

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
            String GetName();
            Bool SetName(const String & name);
            String GetValue();
            Bool SetValue(const String & value);

            // Attributes
            Attribute * GetFirstAttribute();
            Attribute * GetLastAttribute();

            // Traversal
            Node * GetParent();
            Node * GetFirstChild();
            Node * GetLastChild();
            Node * GetNextSibling();
            Node * GetPreviousSibling();

            // Named traversal
            Node * GetChild(const String & name);
            Node * GetNextSibling(const String & name);
            Node * GetPreviousSibling(const String & name);

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
            String GetName();
            Bool SetName(const String & name);

            // Value
            String GetString();
            Bool SetString(const String & value);
            Int32 GetInt32();
            Bool SetInt32(const Int32 & value);
            UInt32 GetUInt32();
            Bool SetUInt32(const UInt32 & value);
            Float GetFloat();
            Bool SetFloat(const Float & value);
            Bool GetBool();
            Bool SetBool(const Bool & value);

            // Traversal
            Attribute * GetNextAttribute();
            Attribute * GetPreviousAttribute();

        private:
            Attribute();

            AttributeImpl * m_Impl;
        };
    }
}
