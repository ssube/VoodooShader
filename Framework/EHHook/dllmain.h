// dllmain.h : Declaration of module class.

class CEHHookModule : public ATL::CAtlDllModuleT< CEHHookModule >
{
public :
	DECLARE_LIBID(LIBID_EHHookLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_EHHOOK, "{E417B6C3-3AB2-4748-A7C7-7A455C990747}")
};

extern class CEHHookModule _AtlModule;
