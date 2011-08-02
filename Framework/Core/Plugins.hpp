/**************************************************************************************************\
 * This file is part of the Voodoo Shader Framework, a comprehensive shader support library.
 * Copyright (c) 2010-2011 by Sean Sube
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

#pragma once

#include "Meta.hpp"

std::vector<int> v;


namespace VoodooShader
{
    class VOODOO_API PluginManager
    {
    public:
        bool LoadPath(_In_ String path);

        template<typename T, pT = std::shared_ptr<T>>
        pT CreateClass(_In_ String name)
        {
            IObject * object = this->CreateObject(name);

            return pT((T*)object);
        };

        IObject * CreateObject(_In_ String name);

    private:
        Core * m_Core;
        ModuleMap m_Modules;
        ClassMap m_Classes;
    };
}
