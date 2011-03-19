#include "Gem_Module.hpp"

#include "Gem_Adapter.hpp"

HRESULT DefaultErrorCode = D3DERR_INVALIDCALL;

VoodooShader::Core * VoodooCore = NULL;
VoodooShader::HookManager * VoodooHooker = NULL;
VoodooShader::Gem::Adapter * VoodooGem = NULL;

//! @todo Shift most of these globals, except the core and adapter, into the adapter (as members).
//!        This may work well with functions to set some of the matrices and such. Shifting them
//!        properly is a right pain and will take some rethinking of parts of the system.

D3DCAPS8 d3d8Caps;

IVoodoo3D8 * VoodooObject = NULL;
IVoodoo3DDevice8 * VoodooDevice = NULL;

TextureTuple gBackbuffer;
TextureTuple gScratch;
TextureTuple gThisFrame;
std::map<VoodooShader::String, TextureTuple> gTextures;

VoodooShader::ParameterRef gMatrixView, gMatrixProj, gMatrixWorld;

D3DPRESENT_PARAMETERS gParams;

VoodooShader::ShaderRef testShader;

void * Gem_D3D8Create(UINT sdkVersion)
{
    IDirect3D9 * realObject = Direct3DCreate9(sdkVersion);
    IVoodoo3D8 * fakeObject = new IVoodoo3D8(realObject);

    return fakeObject;
}