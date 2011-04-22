// VoodooParser.cpp : Implementation of CVoodooParser

#include "stdafx.h"
#include "VoodooParser.h"

#include "Common.hpp"


// CVoodooParser
CVoodooParser::CVoodooParser()
{
    m_Refrs = 0;
    m_Core = NULL;
}

CVoodooParser::~CVoodooParser()
{
    m_Refrs = 0;
    m_Core = NULL;
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
        hr = pParser->QueryInterface(IID_IVoodooTechnique, (void**)&ipParser);
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

STDMETHODIMP CVoodooParser::AddVariable(BSTR pName, BSTR pValue, VariableType Type)
{
    /*ILoggerRef logger = m_Core->GetLogger();
    if ( logger.get() )
    {
        logger->Log(LL_Debug, VOODOO_CORE_NAME, "Adding variable \"%s\" with value \"%s\".", name.c_str(), value.c_str());
    }*/

    CComBSTR name(pName);
    this->Parse(name, VarName, &name);

    if ( Type == System )
    {
        if ( m_SysVariables.PLookup(name) == NULL )
        {
            m_SysVariables.SetAt(name, pValue);
        } else {
            /*if ( logger.get() )
            {
                logger->Log(LL_Warning, VOODOO_CORE_NAME, "Unable to add duplicate system variable \"%s\".", finalname.c_str());
            }*/
        }
    } else {
        m_Variables.SetAt(name, pValue);
    }
    return S_OK;
}

STDMETHODIMP CVoodooParser::RemoveVariable(BSTR pName)
{
    /*ILoggerRef logger = m_Core->GetLogger();
    if ( logger.get() )
    {
        m_Core->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, "Removing variable \"%s\".", name.c_str());
    }*/

    CComBSTR name(pName);
    this->Parse(name, VarName, &name);

    m_Variables.RemoveKey(name);
    return S_OK;
}

STDMETHODIMP CVoodooParser::Parse(BSTR pString, ParseFlags Flags, LPBSTR pParsed)
{
    Dictionary parseState;
    CComBSTR input(pString);
    HRESULT hr = this->ParseRaw(&input, Flags, 0, &parseState);
    input.CopyTo(pParsed);
    return hr;
}

STDMETHODIMP CVoodooParser::ParseRaw(LPBSTR pString, ParseFlags Flags, INT Depth, Dictionary * State)
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
    CStringW iteration(*pString);
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
            
        CStringW varname = iteration.Left(endpos);
        int startpos = varname.ReverseFind(CVoodooParser::VarDelimStart);
        if ( startpos < 1 || varname[startpos-1] != CVoodooParser::VarDelimPre )
        {
            // Stop parsing if no opening sequence is found, or there is no room for one
            break;
        }

        varname = varname.Mid(startpos);

        // Check for state variables
        int statepos = varname.Find(':');
        if ( statepos > 0 )
        {
            // State set, handle
            CComBSTR svalue(varname.Mid(statepos + 1));
            this->ParseRaw(&svalue, Flags, ++Depth, State);

            CComBSTR sname(varname.Left(statepos));
            this->ParseRaw(&sname, VarName, ++Depth, State);

            State->SetAt(sname, svalue);
            varname.Empty();
        } 

        if ( varname.GetLength() < 1 )
        {
            // Erase the variable sequence if it is an empty variable and restart the loop
            CStringW output = iteration.Left(startpos - 1);
            output += iteration.Right(endpos);

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
        this->ParseRaw(&fname, VarName, ++Depth, State);

        // Lookup and replace the variable
        BSTR fvalue;

        if ( 
            m_SysVariables.Lookup(fname, fvalue) == 0 &&
            State->Lookup(fname, fvalue) == 0 &&
            m_Variables.Lookup(fname, fvalue) == 0
            )
        {
            // Not a stored variable
            CStringW var;
            if ( var.GetEnvironmentVariable(fname) == 0 && !supress )
            {
                // Not a variable
                var = L"badvar:";
                var.Append(fname);
            }
            var.SetSysString(&fvalue);
        }

        // Put the final string together
        CStringW output = iteration.Left(startpos - 1);
        if ( parse && SysStringLen(fvalue) > 0 )
        {
            this->ParseRaw(&fvalue, Flags, ++Depth, State);
            output += CStringW(fvalue);
        }
        output += iteration.Right(endpos);

        iteration = output;
    }

    // Handle slash replacement
    if ( Flags == NoFlags )
    {
        iteration.SetSysString(pString);
        return S_OK;
    } else if ( Flags == VarName ) {
        iteration.MakeLower();
        iteration.SetSysString(pString);
        return S_OK;
    } 
        
    if ( Flags & SlashOnly )
    {
        iteration.Replace(L'\\', L'/');
        if ( Flags & SingleSlash )
        {
            while ( iteration.Replace(L"//", L"/") > 0 ) { }
        }
    } else if ( Flags & BackslashOnly ) {
        iteration.Replace(L'/', L'\\');
        if ( Flags & SingleSlash )
        {
            while ( iteration.Replace(L"\\\\", L"\\") > 0 ) { }
        }
    }

    if ( Flags & Lowercase )
    {
        iteration.MakeLower();
    } else if ( Flags & Uppercase ) {
        iteration.MakeUpper();
    }

    //if ( logger.get() )
    //{
        //m_Core->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, "Returning string %s from parser.", iteration.c_str());
    //}

    iteration.SetSysString(pString);
    return S_OK;
}