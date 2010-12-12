// Forces compilation

#ifdef _DEBUG
#	pragma comment(linker, "/export:Direct3DCreate9=Voodoo_DX9_d.Direct3DCreate9")
#else
#	pragma comment(linker, "/export:Direct3DCreate9=Voodoo_DX9.Direct3DCreate9")
#endif
