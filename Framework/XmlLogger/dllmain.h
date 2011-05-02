// dllmain.h : Declaration of module class.

class CXmlLoggerModule : public ATL::CAtlDllModuleT< CXmlLoggerModule >
{
public :
	DECLARE_LIBID(LIBID_XmlLoggerLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_XMLLOGGER, "{88342BC8-219D-4729-9413-ECF136DBDD66}")
};

extern class CXmlLoggerModule _AtlModule;
