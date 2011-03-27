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

#include "Frost_Module.hpp"

namespace VoodooShader
{
    namespace Frost
    {
        class Adapter
            : public IAdapter
        {
        public:
            Adapter(_In_ Core * core);
            ~Adapter();

            // IObject methods
            virtual const char * GetObjectClass();

            // IAdapter methods
            virtual bool LoadPass( _In_ Pass * pass );
            virtual void BindPass( _In_ PassRef pass );
            virtual void UnbindPass();
            virtual void DrawQuad(Vertex * vertexData);
            virtual void ApplyParameter( _In_ ParameterRef param );
            virtual void DrawShader( _In_ ShaderRef shader );
            virtual TextureRef CreateTexture( _In_ String name, _In_ TextureDesc desc );
            virtual bool ConnectTexture( _In_ ParameterRef param, _In_ TextureRef texture );
            virtual void HandleError( _In_ CGcontext context, _In_ CGerror error, _In_ void * core );

            // Frost-specific methods
            virtual void SetDC(_In_ HDC hdc);
            virtual void SetGLRC(_In_ HGLRC hglrc);

        private:
            Core * mCore;

            /**
             * Handle to the device context used by OpenGL.
             */
            HDC mDC;

            /**
             * Handle to the OpenGL render context.
             */
            HGLRC mGLRC;

            CGpass mLastPass;
        };
    }
}