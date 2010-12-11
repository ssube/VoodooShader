// Forces compilation

#ifdef _DEBUG
#	pragma comment(linker, "/export:Direct3DCreate8=Voodoo_DX89_d.Direct3DCreate8")
#else
#	pragma comment(linker, "/export:Direct3DCreate8=Voodoo_DX89.Direct3DCreate8")
#endif
