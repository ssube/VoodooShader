// dllmain.h : Declaration of module class.

class CWFileSystemModule : public ATL::CAtlDllModuleT< CWFileSystemModule >
{
public :
	DECLARE_LIBID(LIBID_WFileSystemLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_WFILESYSTEM, "{6B61CA98-7413-420F-98C2-57308F555889}")
};

extern class CWFileSystemModule _AtlModule;
