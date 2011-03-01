#ifndef VOODOO_DX9_ADAPTER_HPP
#define VOODOO_DX9_ADAPTER_HPP

namespace VoodooShader
{
    namespace DirectX9
    {
        struct FSVert
        {
            FLOAT x, y, z, rhw;
            //DWORD color;
            FLOAT tu, tv;
        };

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_TEX1)

        LPDIRECT3DVERTEXBUFFER9 FSQuadVerts = NULL;

        class VOODOO_API_DX9 Adapter
            : VoodooShader::Adapter
        {

        public:
            Adapter(Core * core, IDirect3DDevice9 * device);

            Version GetVersion();

            bool LoadPass(Pass * pass);
            void BindPass(PassRef shader);
            void UnbindPass();

            void DrawQuad(Vertex * vertexData);
            void DrawShader(ShaderRef shader);

            void ApplyParameter(ParameterRef param);

            bool ConnectTexture(ParameterRef param, TextureRef texture);
            TextureRef CreateTexture(std::string name, size_t width, size_t height, size_t depth, 
                bool mipmaps, TextureFormat format);

            void HandleError(CGcontext context, CGerror error, void * core);

        private:
            Core * mCore;
            IDirect3DDevice9 * mDevice;

            CGprogram mBoundVP;
            CGprogram mBoundFP;

            IDirect3DSurface9 * mRenderTarget;
        };
    };
}

#endif /*VOODOO_DX9_ADAPTER_HPP*/
