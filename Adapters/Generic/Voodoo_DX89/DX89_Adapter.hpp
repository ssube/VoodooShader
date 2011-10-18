#ifndef VOODOO_DX89_ADAPTER_HPP
#define VOODOO_DX89_ADAPTER_HPP

#include "DX89_Module.hpp"

namespace VoodooShader
{
 namespace DirectX89
{
 struct FSVert
 {
  FLOAT x, y, z, rhw;
  FLOAT tu, tv;
 };

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_TEX1)

 extern LPDIRECT3DVERTEXBUFFER9 FSQuadVerts;

 class VOODOO_API_DX89 Adapter :
  VoodooShader::Adapter
 {

/**
 *
 */
public:
  Adapter(ICore *core, IDirect3DDevice9 *device);

  Version GetVersion(void);

  bool LoadPass(IPass *pass);
  void BindPass(IPass* shader);
  void UnbindPass(void);

  void DrawQuad(Vertex *vertexData);
  void DrawShader(IShader* shader);

  void ApplyParameter(IParameter* param);

  bool ConnectTexture(IParameter* param, ITexture* texture);
  ITexture* CreateTexture
     (
      std::string name,
      size_t width,
      size_t height,
      size_t depth,
      bool mipmaps,
      TextureFormat format
     );

  void HandleError(CGcontext context, CGerror error, void *core);

/**
 *
 */
private:
  ICore *mCore;
  IDirect3DDevice9 *mDevice;

  CGprogram mBoundVP;
  CGprogram mBoundFP;

  IDirect3DSurface9 *mRenderTarget;
 };
};
}
#endif /* VOODOO_DX9_ADAPTER_HPP */
