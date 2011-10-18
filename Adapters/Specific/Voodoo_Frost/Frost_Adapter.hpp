/**
 * \ This file is part of the Voodoo Shader Framework, a comprehensive shader
 * support library. Copyright (c) 2010-2011 by Sean Sube This program is free software;
 * you can redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2 of the License, or (at your option) any later version. This
 * program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details. You
 * should have received a copy of the GNU General Public License along with this program;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth
 * Floor, Boston, MA 02110-1301 US Support and more information may be found at
 * http://www.voodooshader.com, or by contacting the developer at
 * peachykeen@voodooshader.com \ 
 */
#include "Frost_Module.hpp"

namespace VoodooShader
{
 namespace Frost
{
 class Adapter :
  public IAdapter
 {

/**
 *
 */
public:
  Adapter(_In_ ICore *core);
  ~ Adapter(void);

  // IObject methods
  const char *GetObjectClass(void);

  // IAdapter methods
  bool LoadPass(_In_ IPass *pass);
  bool UnloadPass(_In_ IPass *pass);
  void BindPass(_In_ IPass* pass);
  void UnbindPass(void);
  void DrawQuad(Vertex *vertexData);
  void ApplyParameter(_In_ IParameter* param);
  void DrawShader(_In_ IShader* shader);
  ITexture* CreateTexture(_In_ String name, _In_ TextureDesc desc);
  bool ConnectTexture(_In_ IParameter* param, _In_ ITexture* texture);
  void HandleError(_In_ CGcontext context, _In_ CGerror error, _In_ void *core);

  // Frost-specific methods
  void SetDC(_In_opt_ HDC hdc);
  void SetGLRC(_In_opt_ HGLRC hglrc);

/**
 *
 */
private:
  ICore *mCore;
  ILogger* mLogger;

  CGcontext mCgContext;

  /* Handle to the device context used by OpenGL. */
  HDC mDC;

  /* Handle to the OpenGL render context. */
  HGLRC mGLRC;

  CGpass mLastPass;

  ITexture* mTexDepthFrame;
  ITexture* mTexThisFrame;
  ITexture* mTexLastPass;
  ITexture* mTexLastShader;
 };
}
}
