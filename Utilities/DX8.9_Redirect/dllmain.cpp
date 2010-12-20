// Forces compilation

#ifdef _DEBUG
#	pragma comment(linker, "/export:Direct3DCreate8=Voodoo_DX8_d.Direct3DCreate8")
#else
#	pragma comment(linker, "/export:Direct3DCreate8=Voodoo_DX8.Direct3DCreate8")
#endif
