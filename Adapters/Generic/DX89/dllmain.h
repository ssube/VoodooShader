// dllmain.h : Declaration of module class.

class CDX89Module : public ATL::CAtlDllModuleT< CDX89Module >
{
public :
	DECLARE_LIBID(LIBID_DX89Lib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DX89, "{525B7A78-8FF8-49AE-AFBE-BCB93531D6DD}")
};

extern class CDX89Module _AtlModule;
