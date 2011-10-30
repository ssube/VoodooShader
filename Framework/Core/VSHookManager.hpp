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

#include "IHookManager.hpp"

namespace VoodooShader
{
    class VSHookManager :
        public IHookManager
    {
    public:
        VSHookManager(_In_ ICore * pCore);
        virtual ~VSHookManager(void);

        virtual int32_t AddRef() const;
        virtual int32_t Release() const;
        virtual String ToString(void) const;
        virtual ICore * GetCore(void) const;

        bool Add(_In_ const String & name, _In_ void * pSrc, _In_ void * pDest);
        bool Remove(_In_ const String & name);
        void RemoveAll(void);

    private:
        mutable int32_t m_Refs;
        ICore * m_Core;
    };
}
