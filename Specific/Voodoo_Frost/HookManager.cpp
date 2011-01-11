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

#include "FrostIncludes.hpp"
#include "GL/gl.h"

TRACED_HOOK_HANDLE 
	hook_glGetString = NULL, 
	hook_glViewport = NULL, 
	hook_wglCreateContext = NULL,
	hook_wglDeleteContext = NULL, 
	hook_wglGetProcAddress = NULL, 
	hook_wglMakeCurrent = NULL,
	hook_glClear = NULL, 
	hook_wglSwapLayerBuffers = NULL, 
	hook_glBegin = NULL, 
	hook_glBindTexture = NULL,
	hook_glDeleteTextures = NULL, 
	hook_glDrawElements = NULL, 
	hook_glEnd = NULL, 
	hook_glImage = NULL, 
	hook_glEnable = NULL, 
	hook_glFogf = NULL, 
	hook_glFogfv = NULL,

	hook_Scene_fogFunc = NULL, 
	hook_Cam_clipFunc = NULL, 
	hook_screenFunc = NULL,
	hook_GetName = NULL;

void HookOpenGL(void)
{
	BOOL success = NULL;

	Log->logSimple(_STR(142));

	ULONG threadIDs[1];
	threadIDs[0] = NULL;
	LhSetGlobalInclusiveACL(threadIDs, 1);

	DWORD retVal = NULL;

	hook_glGetString = new HOOK_TRACE_INFO();
	retVal = LhInstallHook( &glGetString, &PLATFORMNAME(glGetString), NULL, hook_glGetString) == 0;

	hook_glViewport = new HOOK_TRACE_INFO();
	retVal &= LhInstallHook( &(glViewport), &PLATFORMNAME(glViewport), NULL, hook_glViewport) == 0;

	hook_wglCreateContext = new HOOK_TRACE_INFO();
	retVal &= LhInstallHook( &(wglCreateContext), &PLATFORMNAME(wglCreateContext), NULL, hook_wglCreateContext) == 0;

	hook_wglDeleteContext = new HOOK_TRACE_INFO();
	retVal &= LhInstallHook( &(wglDeleteContext), &PLATFORMNAME(wglDeleteContext), NULL, hook_wglDeleteContext) == 0;

	hook_wglGetProcAddress = new HOOK_TRACE_INFO();
	retVal &= LhInstallHook( &(wglGetProcAddress), &PLATFORMNAME(wglGetProcAddress), NULL, hook_wglGetProcAddress) == 0;

	hook_wglMakeCurrent = new HOOK_TRACE_INFO();
	retVal &= LhInstallHook( &(wglMakeCurrent), &PLATFORMNAME(wglMakeCurrent), NULL, hook_wglMakeCurrent) == 0;

	if ( nwshader->settings->use_shaders )
	{
		hook_glClear = new HOOK_TRACE_INFO();
		retVal &= LhInstallHook( &(glClear), &PLATFORMNAME(glClear), NULL, hook_glClear) == 0;

		hook_wglSwapLayerBuffers = new HOOK_TRACE_INFO();
		retVal &= LhInstallHook( &(wglSwapLayerBuffers), &PLATFORMNAME(wglSwapLayerBuffers), NULL, hook_wglSwapLayerBuffers) == 0;
	}

	if ( nwshader->settings->hook_textures || nwshader->settings->use_perpixel )
	{

		hook_glBindTexture = new HOOK_TRACE_INFO();
		retVal &= LhInstallHook( &(glBindTexture), &PLATFORMNAME(glBindTexture), NULL, hook_glBindTexture) == 0;

		hook_glDeleteTextures = new HOOK_TRACE_INFO();
		retVal &= LhInstallHook( &(glDeleteTextures), &PLATFORMNAME(glDeleteTextures), NULL, hook_glDeleteTextures) == 0;
	}

	if ( nwshader->settings->use_shaders || nwshader->settings->hook_textures || nwshader->settings->use_perpixel )
	{
		hook_glBegin = new HOOK_TRACE_INFO();
		retVal &= LhInstallHook( &(glBegin), &PLATFORMNAME(glBegin), NULL, hook_glBegin) == 0;

		hook_glDrawElements = new HOOK_TRACE_INFO();
		retVal &= LhInstallHook( &(glDrawElements), &PLATFORMNAME(glDrawElements), NULL, hook_glDrawElements) == 0;

		hook_glEnd = new HOOK_TRACE_INFO();
		retVal &= LhInstallHook( &(glEnd), &PLATFORMNAME(glEnd), NULL, hook_glEnd) == 0;
	}

	if ( !nwshader->settings->use_fog || ( nwshader->settings->fog_mult != 1.00f ) )
	{
		hook_glEnable = new HOOK_TRACE_INFO();
		retVal &= LhInstallHook( &(glEnable), &PLATFORMNAME(glEnable), NULL, hook_glEnable) == 0;

		hook_glFogf = new HOOK_TRACE_INFO();
		retVal &= LhInstallHook( &(glFogf), &PLATFORMNAME(glFogf), NULL, hook_glFogf) == 0;

		hook_glFogfv = new HOOK_TRACE_INFO();
		retVal &= LhInstallHook( &(glFogfv), &PLATFORMNAME(glFogfv), NULL, hook_glFogfv) == 0;
	}

	if ( retVal )
	{
		LhSetInclusiveACL(threadIDs, 1, hook_glGetString);
		LhSetInclusiveACL(threadIDs, 1, hook_glViewport);

		LhSetInclusiveACL(threadIDs, 1, hook_wglCreateContext);
		LhSetInclusiveACL(threadIDs, 1, hook_wglDeleteContext);
		LhSetInclusiveACL(threadIDs, 1, hook_wglGetProcAddress);
		LhSetInclusiveACL(threadIDs, 1, hook_wglMakeCurrent);

		// Fullscreen shader stuff
		if ( nwshader->settings->use_shaders )
		{
			LhSetInclusiveACL(threadIDs, 1, hook_glClear);

			LhSetInclusiveACL(threadIDs, 1, hook_wglSwapLayerBuffers);
		}

		if ( nwshader->settings->hook_textures || nwshader->settings->use_perpixel )
		{
			LhSetInclusiveACL(threadIDs, 1, hook_glBegin);
			LhSetInclusiveACL(threadIDs, 1, hook_glBindTexture);
			LhSetInclusiveACL(threadIDs, 1, hook_glDeleteTextures);
			LhSetInclusiveACL(threadIDs, 1, hook_glDrawElements);
			LhSetInclusiveACL(threadIDs, 1, hook_glEnd);
			Log->logSimple(_STR(144));
		}

		if ( nwshader->settings->use_shaders || nwshader->settings->hook_textures || nwshader->settings->use_perpixel )
		{
			// Needed for drawn-geom flag
			LhSetInclusiveACL(threadIDs, 1, hook_glBegin);
			LhSetInclusiveACL(threadIDs, 1, hook_glDrawElements);
			LhSetInclusiveACL(threadIDs, 1, hook_glEnd);
		}

		if ( !nwshader->settings->use_fog || ( nwshader->settings->fog_mult != 1.00f ) )
		{
			LhSetInclusiveACL(threadIDs, 1, hook_glEnable);
			LhSetInclusiveACL(threadIDs, 1, hook_glFogf);
			LhSetInclusiveACL(threadIDs, 1, hook_glFogfv);
		}

		Log->logSimple(_STR(143));
		//engineHooked = true;
	}
}
