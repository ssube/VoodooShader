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

namespace VoodooShader
{
    /**
     * Simple wrapper to help manage objects. Automatically handles reference inc/dec (similar to boost's intrusive_ptr).
     * 
     * @tparam T The type to store (must derive from IObject).
     */
    template<typename T>
    class Reference
    {
    public:
        Reference()
            : m_Object(nullptr)
        {
        
        };

        Reference(_In_opt_ T * pObject)
        {
            m_Object = dynamic_cast<IObject*>(pObject);
            this->IncObj();
        };

        Reference(_In_ const Reference<T> & other)
        {
            m_Object = other.m_Object;
            this->IncObj();
        };

        Reference(_Inout_ Reference<T>&& other)
        {
            m_Object = other.m_Object;
            other.m_Object = nullptr;
        };

        ~Reference()
        {
            this->DecObj();
        };

        T * operator=(_In_opt_ T * pOther)
        {
            if (m_Object != pOther)
            {
                if (pOther)
                {
                    pOther->AddRef();
                }
                this->DecObj();
                m_Object = pOther;
            }
            return (*this);
        };

        T * operator=(_In_ const Reference<T> & other)
        {
            if (m_Object != other.m_Object)
            {
                other.m_Object->AddRef();
                this->DecObj();
                m_Object = other.m_Object;
            }
            return (*this);
        };

        operator T*() const
        {
            return m_Object;
        };
        
        T& operator*() const
        {
            assert(m_Object!=nullptr);
            return *p;
        };

        T** operator&()
        {
            assert(m_Object==nullptr);
            return &m_Object;
        };

        T* operator->() const
        {
            return m_Object;
        };
        
        bool operator!() const
        {
            return (m_Object==nullptr);
        };

        bool operator<(_In_opt_ T* pOther) const
        {
            return (m_Object < pOther);
        };

        bool operator!=(_In_opt_ T* pOther) const
        {
            return !operator==(pOther);
        };

        bool operator==(_In_opt_ T* pOther) const
        {
            return (m_Object == pOther);
        };
        
    private:
        Int32 IncObj()
        {
            if (m_Object)
            {
                return m_Object->AddRef();
            } else {
                return -1;
            }
        };

        Int32 DecObj()
        {
            if (m_Object)
            {
                Int32 count = m_Object->Release();
                if (count < 1)
                {
                    m_Object = nullptr;
                }
            } else {
                return -1;
            }
        };

        IObject * m_Object;
    };
}
