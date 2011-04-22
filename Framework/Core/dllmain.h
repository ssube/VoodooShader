// dllmain.h : Declaration of module class.

class CCoreModule : public ATL::CAtlDllModuleT< CCoreModule >
{
public :
	DECLARE_LIBID(LIBID_Voodoo_Core)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_CORE, "{223E452D-A072-40F7-9A54-A93C6C504B85}")
};

extern class CCoreModule _AtlModule;
