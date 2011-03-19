#ifndef VOODOO_GEM_ADAPTER_HPP
#define VOODOO_GEM_ADAPTER_HPP

#include "Gem_Includes.hpp"

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

        _Check_return_
            VOODOO_API_GEM VoodooShader::Gem::Adapter * CreateAdapter
        (
            _In_ Core * core
        );

        VOODOO_API_GEM void DestroyAdapter
        (
            _In_ VoodooShader::Gem::Adapter * adapter
        );

        class VOODOO_API_GEM Adapter
            : public VoodooShader::Adapter
        {

        public:
            Adapter
            (
                _In_ Core * core
            );

            ~Adapter();

            virtual Version GetVersion();

            virtual bool LoadPass(Pass * pass);
            virtual void BindPass(PassRef shader);
            virtual void UnbindPass();

            virtual void DrawQuad(Vertex * vertexData);

            virtual void DrawShader(ShaderRef shader);

            virtual void ApplyParameter(ParameterRef param);

            virtual bool ConnectTexture(ParameterRef param, TextureRef texture);
            virtual TextureRef CreateTexture(String name, TextureDesc desc);

            virtual void HandleError(CGcontext context, CGerror error, void * core);

            virtual void SetDevice
            (
                _In_ LPDIRECT3DDEVICE9 device
            );

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
