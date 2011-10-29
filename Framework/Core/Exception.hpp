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
    /**
     * @addtogroup VoodooCore 
     * @{
     */

    /**
     * Generic exception class for Voodoo applications. Combines a generic standard exception with extended error location 
     * data. 
     * 
     * @note If possible, thrown Exceptions will automatically log as much data as they contain in a formatted way. You can 
     *      retrieve this string with the Exception::what() method (compatible with std::exception).
     */
    class VOODOO_API Exception :
        public std::exception
    {
    public:
        Exception
        (
            _In_ wchar_t * Module,
            _In_ wchar_t * Message,
            _In_ ICore * pCore,
            _In_ char * File,
            _In_ char * Function,
            _In_ int Line
        );

        Exception
        (
            _In_ String Module,
            _In_ String Message,
            _In_ ICore * pCore,
            _In_ char * File,
            _In_ char * Function,
            _In_ int Line
        );

        virtual ~Exception(void);

        virtual const char * what(void);

    private:
        ICore * m_Core;
        String m_Module;
        String m_Message;
        char * m_File;
        char * m_Function;
        int m_Line;
        char * m_FmtMsg;
    };
    /** 
     * @} 
     */
}
