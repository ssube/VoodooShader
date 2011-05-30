// WFS.cpp : Implementation of CWFS

#include "stdafx.h"
#include "WFS.h"

CWFS::CWFS()
    : m_Init(FALSE)
{ }

// CWFS
STDMETHODIMP CWFS::Initialize(IVoodooCore *pCore)
{
    if ( this->m_Init ) return VSFERR_ALREADY_INIT;
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

STDMETHODIMP CWFS::get_Core(IVoodooCore **ppCore)
{
    if ( !this->m_Init ) return VSFERR_NOT_INIT;
    if ( ppCore == NULL ) return VSFERR_INVALID_ARG;

    *ppCore = this->m_Core;
    return VSF_OK;
}

STDMETHODIMP CWFS::AddDirectory(BSTR pPath)
{
    if ( !this->m_Init ) return VSFERR_NOT_INIT;

    CComBSTR modPath(pPath);

    IVoodooParser * pParser = NULL;
    this->m_Core->get_Parser(&pParser);
    pParser->Parse(modPath, PF_None, &modPath);

    CString dir(modPath);

    int splitter = dir.Find(L";");
    if ( splitter > 0 )
    {
        CString partDir = dir.Left(splitter);
        this->m_Directories.AddHead(partDir);

        dir = dir.Mid(splitter);
        splitter = dir.Find(L";");
    }

    if ( dir.GetLength() > 0 )
    {
        this->m_Directories.AddHead(dir);
    }

    return VSF_OK;
}

STDMETHODIMP CWFS::RemoveDirectory(BSTR pPath)
{
    if ( !this->m_Init ) return VSFERR_NOT_INIT;

    CComBSTR modPath(pPath);

    IVoodooParser * pParser = NULL;
    this->m_Core->get_Parser(&pParser);
    pParser->Parse(modPath, PF_None, &modPath);
    
    CString dir(modPath);

    POSITION pos = this->m_Directories.Find(dir);

    if ( pos == NULL )
    {
        return VSFOK_NOT_FOUND;
    } else {
        this->m_Directories.Remove(pos);
        return VSF_OK;
    }
}

STDMETHODIMP CWFS::FindFile(BSTR pPath, IVoodooFile **ppFile)
{
    if ( !this->m_Init ) return VSFERR_NOT_INIT;
    if ( ppFile == NULL ) return VSFERR_INVALID_ARG;

    *ppFile = NULL;

    CComBSTR modFile(pPath);

    IVoodooParser * pParser = NULL;
    this->m_Core->get_Parser(&pParser);
    pParser->Parse(modFile, PF_None, &modFile);

    POSITION dirPos = this->m_Directories.GetHeadPosition();
    while ( dirPos )
    {
        CStringW filename = m_Directories.GetNext(dirPos);
        filename += modFile;

        CFile file(dir);
        if ( file.m_hFile != CFile::hFileNull )
        {
            // File exists. Test for image.
            ILuint image = ilGenImage();
            ilBindImage(image);

            ILboolean loaded = ilLoadImage(filename.);

            if ( loaded == IL_FALSE )
            {
                ilBindImage(0);
                ilDeleteImage(image);
                
                *ppFile = CWFile.Create(file)
            } else {
                *ppFile = CWImage.Create(file);
            }

            return VSF_OK;
        }
    }

    return VSFERR_NOT_FOUND;
}

