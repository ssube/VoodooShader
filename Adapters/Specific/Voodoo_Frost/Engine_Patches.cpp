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

#include "Frost_Module.hpp"
// Voodoo Core
#include "Format.hpp"

using namespace VoodooShader;

/**
 * Applies a variation of the popular NWN camera hack to the engine in-memory. This technique is simpler to manipulate
 * and customize than the traditional byte-patcher.
 *
 * @todo Update this to use customizable settings when they're implemented.
 */
void CameraHack(void)
{
    ILoggerRef logger = gpVoodooCore->GetLogger();

    logger->LogMessage(VSLog_ModInfo, VOODOO_FROST_NAME, VSTR("Applying camera hack..."));

    float maxDist   = 120.0f;
    float minDist   = -0.2f;
    float angle     = 1000.0f;

    DWORD oldProtect, finalProtect;
    BOOL lockStatus = VirtualProtect((PVOID) 0x004A9000, 0x1000, PAGE_EXECUTE_READWRITE, &oldProtect);

    if (lockStatus != 0)
    {
        memcpy((PVOID) 0x004A93ED, &minDist, sizeof(float));
        memcpy((PVOID) 0x004A93F7, &maxDist, sizeof(float));
        memcpy((PVOID) 0x004A940B, &angle,   sizeof(float));
        memcpy((PVOID) 0x004A968B, &minDist, sizeof(float));
        memcpy((PVOID) 0x004A9695, &maxDist, sizeof(float));

        VirtualProtect((PVOID) 0x004A9000, 0x1000, oldProtect, &finalProtect);

        logger->LogMessage(VSLog_ModInfo, VOODOO_FROST_NAME, VSTR("Camera hack successfully applied."));
    }
    else
    {
        DWORD error = GetLastError();

        logger->LogMessage(VSLog_ModInfo, VOODOO_FROST_NAME, Format("Camera hack failed with code %1%.") << error);
    }
}
