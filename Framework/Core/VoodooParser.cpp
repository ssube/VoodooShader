// VoodooParser.cpp : Implementation of CVoodooParser

#include "stdafx.h"
#include "VoodooParser.h"




// CVoodooParser
CVoodooParser::CVoodooParser()
{
    m_Refrs = 0;
    m_Core = NULL;
    m_Logger = NULL;
}

CVoodooParser::~CVoodooParser()
{
    m_Refrs = 0;
    m_Core = NULL;
    m_Logger = NULL;
}

IVoodooParser * CVoodooParser::Create(IVoodooCore * pCore)
{
    if ( pCore == NULL ) return NULL;

    CComPtr<CVoodooParser> ipParser = NULL;

    CComObject<CVoodooParser> * pParser = NULL;
    HRESULT hr = CComObject<CVoodooParser>::CreateInstance(&pParser);
    if ( SUCCEEDED(hr) )
    {
        pParser->AddRef();

        pParser->m_Core = pCore;
        pCore->get_Logger(&pParser->m_Logger);

        hr = pParser->QueryInterface(IID_IVoodooParser, (void**)&ipParser);
        pParser->Release();
    }

    return ipParser.Detach();
}

STDMETHODIMP CVoodooParser::QueryInterface(REFIID iid, void ** pp) throw()
{
    if ( pp == NULL )
    {
        return E_POINTER;
    } else if ( iid == IID_IUnknown || iid == IID_IVoodooParser ) {
        this->AddRef();
        *pp = this;
        return S_OK;
    } else {
        *pp = NULL;
        return E_NOINTERFACE;
    }
}

STDMETHODIMP_(ULONG) CVoodooParser::AddRef()
{
    return (++m_Refrs);
}

STDMETHODIMP_(ULONG) CVoodooParser::Release()
{
    --m_Refrs;
    if ( m_Refrs == 0 )
    {
        delete this;
        return 0;
    } else {
        return m_Refrs;
    }
}

STDMETHODIMP CVoodooParser::Add(BSTR pName, BSTR pValue, DWORD Type)
{
    CComBSTR name(pName);
    this->Parse(name, PF_VarName, &name);
    CComBSTR value(pValue);

    LogFormat(m_Logger, LL_Debug|LL_Framework, VOODOO_CORE_NAME, VSTR("Adding variable \"%s\" with value \"%s\"."), (LPWSTR)name, (LPWSTR)value);

    if ( Type == VT_System )
    {
        if ( m_SysVariables.PLookup(name) == NULL )
        {
            m_SysVariables.SetAt(name, value);
        } else {
            if ( m_Logger )
            {
                LogFormat(m_Logger, LL_Warning|LL_Framework, VOODOO_CORE_NAME, VSTR("Unable to add duplicate system variable \"%s\"."), name);
            }
            return VSFERR_IS_SYSVAR;
        }
    } else {
        m_Variables.SetAt(name, value);
    }

    return S_OK;
}

STDMETHODIMP CVoodooParser::Remove(BSTR pName)
{
    LogFormat(m_Logger, LL_Debug|LL_Framework, VOODOO_CORE_NAME, VSTR("Removing variable \"%s\"."), pName);

    CComBSTR name(pName);
    if ( SUCCEEDED(this->Parse(name, PF_VarName, &name)) )
    {
        if ( m_Variables.RemoveKey(name) == TRUE )
        {
            return VSF_OK;
        } else {
            return VSFOK_NOT_FOUND;
        }
    } else {
        return VSF_FAIL;
    }
}

STDMETHODIMP CVoodooParser::Parse(BSTR pString, DWORD Flags, LPBSTR pParsed)
{
    Dictionary parseState;

    CComBSTR input(pString);

    HRESULT hr = this->ParseRaw(&input, Flags, 0, &parseState);
    if ( SUCCEEDED(hr) )
    {
        return input.CopyTo(pParsed);
    } else {
        return hr;
    }
}

STDMETHODIMP CVoodooParser::ParseRaw(LPBSTR pString, DWORD Flags, INT Depth, Dictionary * State)
{
    //ILoggerRef logger = m_Core->GetLogger();
    //if ( logger.get() )
    //{
        //m_Core->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, "Parsing string \"%s\" (%X).", input.c_str(), flags);
    //}
    if ( Depth > CVoodooParser::VarMaxDepth || SysStringLen(*pString) < 3 )
    {
        return S_OK;
    }

    // Variable parsing loop
    CString iteration(*pString);
    bool loop = true;

    while ( loop )
    {
        const UINT itlen = iteration.GetLength();
        if ( itlen < 3 )
        {
            // Stop parsing if the string is too small to contain variables 
            // (this should never be hit the first loop)
            break;
        }

        int endpos = iteration.Find(CVoodooParser::VarDelimEnd);
        if ( endpos == -1 )
        {
            // Stop parsing if no closing delimiter is found
            break;
        }
            
        CString varname = iteration.Left(endpos);
        int startpos = varname.ReverseFind(CVoodooParser::VarDelimStart);
        if ( startpos < 1 || varname[startpos-1] != CVoodooParser::VarDelimPre )
        {
            // Stop parsing if no opening sequence is found, or there is no room for one
            break;
        }

        varname = varname.Mid(startpos+1);

        // Check for state variables
        int statepos = varname.Find(':');
        if ( statepos > 0 )
        {
            // State set, handle
            CComBSTR svalue(varname.Mid(statepos + 1));
            this->ParseRaw(&svalue, Flags, ++Depth, State);

            CComBSTR sname(varname.Left(statepos));
            this->ParseRaw(&sname, PF_VarName, ++Depth, State);

            State->SetAt(sname, svalue);
            varname.Empty();
        } 

        if ( varname.GetLength() < 1 )
        {
            // Erase the variable sequence if it is an empty variable and restart the loop
            CString output = iteration.Left(startpos - 1);
            output += iteration.Mid(endpos+1);

            iteration = output;
            continue;
        }

        // Handle variable flags
        bool supress = false;
        bool parse = true;
        // The length of varname is > 0, guaranteed in line 134
        if ( varname[0] == L'$' )
        {
            varname = varname.Mid(1);        
        } else if ( varname[0] == L'?' ) {
            supress = true;
            varname = varname.Mid(1);
        } else if ( varname[0] == L'!' ) {
            parse = false;
            varname = varname.Mid(1);
        }

        // Properly format the variable name (recursive call to simplify future syntax exts)
        CComBSTR fname(varname);
        this->ParseRaw(&fname, PF_VarName, ++Depth, State);

        // Lookup and replace the variable
        CComBSTR fvalue;

        Dictionary::CPair * pPair = m_SysVariables.PLookup(fname);
        if ( pPair != NULL )
        {
            fvalue = pPair->value;
        } else {
            pPair = State->PLookup(fname);
            if ( pPair != NULL )
            {
                fvalue = pPair->value;
            } else {
                pPair = m_Variables.PLookup(fname);
                if ( pPair != NULL )
                {
                    fvalue = pPair->value;
                } else {
                    CString var;
                    if ( var.GetEnvironmentVariable(fname) == 0 && !supress )
                    {
                        // Not a variable
                        var = VSTR("badvar:");
                        var.Append(fname);
                    }
                    fvalue = var.AllocSysString();
                }
            }
        }

        // Put the final string together
        CString output = iteration.Left(startpos - 1);
        if ( parse && SysStringLen(fvalue) > 0 )
        {
            this->ParseRaw(&fvalue, Flags, ++Depth, State);
            output += CString(fvalue);
        }
        output += iteration.Mid(endpos+1);

        iteration = output;
    }

    // Handle slash replacement
    if ( Flags == PF_None )
    {
        iteration.SetSysString(pString);
        return S_OK;
    } else if ( Flags == PF_VarName ) {
        iteration.MakeLower();
        iteration.SetSysString(pString);
        return S_OK;
    } 
        
    if ( Flags & PF_SingleSlash )
    {
        while ( iteration.Replace(VSTR("//"), VSTR("/")) > 0 ) { }
        while ( iteration.Replace(VSTR("\\\\"), VSTR("\\")) > 0 ) { }
    }
    if ( Flags & PF_SlashOnly )
    {
        iteration.Replace(VSTR('\\'), VSTR('/'));
    } else if ( Flags & PF_BackslashOnly ) {
        iteration.Replace(VSTR('/'), VSTR('\\'));
    }

    if ( Flags & PF_Lowercase )
    {
        iteration.MakeLower();
    } else if ( Flags & PF_Uppercase ) {
        iteration.MakeUpper();
    }

    //if ( logger.get() )
    //{
        //m_Core->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, "Returning string %s from parser.", iteration.c_str());
    //}

    iteration.SetSysString(pString);
    return S_OK;
}