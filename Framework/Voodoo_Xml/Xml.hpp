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

#define VOODOO_IMPORT
#include "Voodoo_Core.hpp"

#ifdef VOODOO_IMPORT_XML
#   define VOODOO_XML_API __declspec(dllimport)
#else
#   define VOODOO_XML_API __declspec(dllexport)
#endif

// Hide the DLL-interface warning
#pragma warning(disable:4251)

namespace VoodooShader
{
    namespace PugiXml
    {
        bool RegisterModule
        (
            _In_ Core * core, 
            _In_ Module * module
        );

        int API_ClassCount();

        const char * API_ClassInfo
        (
            _In_ int number
        );

        void * API_ClassCreate
        (
            _In_ int number, 
            _In_ Core * core
        );

        void API_ClassDestroy
        (
            _In_ int number, 
            _In_ void * inst 
        );

        class XmlParser
        {

        };
    }
}