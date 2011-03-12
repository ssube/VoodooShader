#ifndef VOODOO_GEM_ADAPTER_HPP
#define VOODOO_GEM_ADAPTER_HPP

#include "Gem_Includes.hpp"

#include "Gem_Module.hpp"
#include "IVoodoo3DDevice8.hpp"

namespace VoodooShader
{
    namespace Gem
    {
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_TEX1)

        struct StandardQuadVert
        {
            FLOAT x, y, z, rhw;
            FLOAT tu, tv;
        };

        class VOODOO_API_GEM Adapter
            : VoodooShader::Adapter
        {

        public:
            Adapter(Core * core, LPDIRECT3DDEVICE9 device);
            ~Adapter();

            Version GetVersion();

            bool LoadPass(Pass * pass);
            void BindPass(PassRef shader);
            void UnbindPass();

            void DrawQuad(Vertex * vertexData);

            void DrawShader(ShaderRef shader);

            void ApplyParameter(ParameterRef param);

            bool ConnectTexture(ParameterRef param, TextureRef texture);
            TextureRef CreateTexture(String name, TextureDesc desc);

            void HandleError(CGcontext context, CGerror error, void * core);

        private:
            Core * mCore;
            LPDIRECT3DDEVICE9 mDevice;

            CGprogram mBoundVP;
            CGprogram mBoundFP;

            IDirect3DSurface9 * mRenderTarget;
            
            LPDIRECT3DVERTEXBUFFER9 mQuadVerts;
        };
    };
}

#endif /*VOODOO_GEM_ADAPTER_HPP*/
