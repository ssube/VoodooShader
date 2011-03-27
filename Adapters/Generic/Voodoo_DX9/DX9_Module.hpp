
#ifndef VOODOO_DX9_MODULE_HPP
#define VOODOO_DX9_MODULE_HPP

#define VOODOO_IMPORT
#include "Voodoo_Core.hpp"

#include "D3D9.h"
#include "Cg/cgD3D9.h"

//#ifdef _DEBUG
//#    pragma comment(lib, "cgD3D9d.lib")
//#else
#    pragma comment(lib, "cgD3D9.lib")
//#endif

#ifndef VOODOO_IMPORT_DX9
#    define VOODOO_API_DX9 __declspec(dllexport)
#else
#    define VOODOO_API_DX9 __declspec(dllimport)
#endif

#include "DX9_Adapter.hpp"

class IVoodoo3D9;
class IVoodoo3DDevice9;

extern VoodooShader::Core * VoodooCore;
extern VoodooShader::Adapter * VoodooDX9;

extern IVoodoo3D9 * VoodooObject;
extern IVoodoo3DDevice9 * VoodooDevice;

extern VoodooShader::ShaderRef testShader;

extern IDirect3DSurface9 * backbufferSurf;

extern IDirect3DSurface9 * surface_ThisFrame;
extern VoodooShader::TextureRef texture_ThisFrame;

typedef IDirect3D9 * (__stdcall *D3DFunc9)(UINT);


#endif /*VOODOO_DX9_MODULE_HPP*/
