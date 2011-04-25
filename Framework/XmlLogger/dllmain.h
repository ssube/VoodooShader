// dllmain.h : Declaration of module class.

class CXmlLoggerModule : public ATL::CAtlDllModuleT< CXmlLoggerModule >
{
public :
	DECLARE_LIBID(LIBID_XmlLoggerLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_XMLLOGGER, "{348F8298-9532-4C5E-8183-521254FF9FE1}")
};

extern class CXmlLoggerModule _AtlModule;
