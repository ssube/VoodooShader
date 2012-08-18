/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2012 by Sean Sube
 *
 * The Voodoo Shader Framework is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option)
 * any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this program; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 US
 *
 * Support and more information may be found at
 *   http://www.voodooshader.com
 * or by contacting the lead developer at
 *   peachykeen@voodooshader.com
 */
#pragma once

/*
 * This header is essentially a shim for MSVC 10 and previous, until XP support is added to MSVC 11.
 */
#if defined(VOODOO_COMPAT_MSVC10)

#define _In_reads_(x)
#define _In_reads_z_(x)
#define _In_reads_opt_(x)
#define _Inout_updates_opt_(x)
#define _Out_writes_(x)
#define _Out_writes_opt_(x)
#define _Outptr_result_maybenull_

#endif