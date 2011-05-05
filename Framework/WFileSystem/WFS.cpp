// WFS.cpp : Implementation of CWFS

#include "stdafx.h"
#include "WFS.h"


// CWFS
STDMETHODIMP CWFS::Initialize(IVoodooCore *pCore)
{
    if ( pCore == NULL ) return VSFERR_INVALID_ARG;

    // Create builtin vars
    CComPtr<IVoodooParser> pParser;
    pCore->get_Parser(&pParser);

    WCHAR cvar[MAX_PATH] = { 0 };
    if ( SHGetFolderPath(NULL, CSIDL_COMMON_DOCUMENTS, NULL, SHGFP_TYPE_CURRENT, cvar) == S_OK )
    {
        StringCchCat(cvar, MAX_PATH, L"\\My Games\\");
        pParser->AddVariable(L"allgames", cvar, true);
    }
    cvar[0] = 0;
    if ( SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, cvar) == S_OK )
    {
        StringCchCat(cvar, MAX_PATH, L"\\My Games\\");
        pParser->AddVariable(L"mygames", cvar, true);
    }
    cvar[0] = 0;
    if ( SHGetFolderPath(NULL, CSIDL_SYSTEM, NULL, SHGFP_TYPE_CURRENT, cvar) == S_OK )
    {
        pParser->AddVariable(L"systemroot", cvar, true);
    }

    // Init DevIL
    ilInit();

    // Load paths from the config
    CComPtr<IUnknown> pUnkConfig;
    pCore->get_Config(&pUnkConfig);
    CComPtr<IXMLDOMDocument> pConfig;
    pUnkConfig->QueryInterface(IID_IXMLDocument, (void**)&pConfig);

    CComBSTR pathsQuery(L"/VoodooConfig/WFileSystem/SearchPaths/Path");
    CComBSTR pathValueQuery(L"./text()");

    CComPtr<IXMLDOMNodeList> pPathList = NULL;
    hr = pConfig->selectNodes(pathsQuery, &pPathList);
    if ( SUCCEEDED(hr) && pPathList != NULL )
    {
        CComPtr<IXMLDOMNode> pPathNode = NULL;
        while ( ( hr = pPathList->nextNode(&pPathNode) ) == S_OK && pPathNode != NULL )
        {
            VARIANT vpath;
            pPathNode->get_nodeValue(&vpath);

            this->AddDirectory(vpath.bstrVal);
        }
    }
}

STDMETHODIMP CWFS::get_Core(IVoodooCore **ppCore);
STDMETHODIMP CWFS::AddDirectory(BSTR pPath);
STDMETHODIMP CWFS::RemoveDirectory(BSTR pPath);
STDMETHODIMP CWFS::FindFile(BSTR pPath, IVoodooFile **ppFile);
STDMETHODIMP CWFS::FindImage(BSTR pPath, IVoodooImage **ppImage);

