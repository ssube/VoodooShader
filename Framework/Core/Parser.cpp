#include "Parser.hpp"

#include "Core.hpp"

#include "ILogger.hpp"

#include <sstream>
#include <iostream>

namespace VoodooShader
{
    Parser::Parser(_In_ Core * core)
        : mCore(core)
    {    }

    Parser::~Parser()
    {
        mVariables.clear();
    }

    HRESULT Parser::AddVariable(BSTR pName, BSTR pValue,BOOL System)
    {
        /*ILoggerRef logger = mCore->GetLogger();
        if ( logger.get() )
        {
            logger->Log(LL_Debug, VOODOO_CORE_NAME, "Adding variable \"%s\" with value \"%s\".", name.c_str(), value.c_str());
        }*/

        CComBSTR name(pName);
        this->Parse(name, PF_VarName);

        if ( system == TRUE )
        {
            if ( mSysVariables.PLookup(name) == NULL )
            {
                mSysVariables.SetAt(name, pValue);
            } else {
                /*if ( logger.get() )
                {
                    logger->Log(LL_Warning, VOODOO_CORE_NAME, "Unable to add duplicate system variable \"%s\".", finalname.c_str());
                }*/
            }
        } else {
            mVariables.SetAt(name, value);
        }
        return S_OK;
    }

    HRESULT Parser::RemoveVariable(BSTR pName)
    {
        /*ILoggerRef logger = mCore->GetLogger();
        if ( logger.get() )
        {
            mCore->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, "Removing variable \"%s\".", name.c_str());
        }*/

        CComBSTR name(pName);
        this->Parse(name, PF_VarName);
        mVariables.RemoveKey(name);
        return S_OK;
    }

    HRESULT Parser::Parse(LPBSTR pString, ParseFlags Flags)
    {
        Dictionary parseState;

        return this->ParseRaw(pString, Flags, 0, parseState);
    }

    HRESULT Parser::ParseRaw(LPBSTR pString, ParseFlags Flags, INT Depth, Dictionary & State)
    {
        //ILoggerRef logger = mCore->GetLogger();
        //if ( logger.get() )
        //{
            //mCore->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, "Parsing string \"%s\" (%X).", input.c_str(), flags);
        //}

        if ( Depth > Parser::VarMaxDepth || SysStringLen(pString) < 3 )
        {
            return S_OK;
        }

        // Parse out any variables in the filename, first
        CStringW iteration(pString);

        // Variable parsing loop
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

            int endpos = iteration.Find(Parser::VarDelimEnd);
            if ( endpos == -1 )
            {
                // Stop parsing if no closing delimiter is found
                break;
            }
            
            CStringW varname = iteration.Left(endpos);
            int startpos = varname.ReverseFind(Parser::VarDelimStart);
            if ( startpos < 1 || varname[startpos-1] != Parser::VarDelimPre )
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
                String newvalue = varname.Mid(statepos + 1);
                this->ParseRaw(newvalue, Flags, ++Depth, State);

                varname = varname.Left(statepos);
                this->ParseRaw(varname, PF_VarName, ++Depth, State);

                state.SetAt(varname, newvalue);
                varname.Empty();
            } 

            if ( varname.Length() < 1 )
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
            if ( varname[0] = L'$' )
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
            varname = this->ParseRaw(varname, PF_VarName, ++Depth, State);

            // Lookup and replace the variable
            bool foundvar = true;
            CStringW varvalue;

            
            if ( 
                mSysVariables.Lookup(varname, &varvalue) == 0 &&
                State.Lookup(varname, &varvalue) == 0 &&
                mVariables.Lookup(varname, &varvalue) == 0 &&
                varvalue.GetEnvironmentVariable(varname) == 0 &&
                !supress
               )
            {
                // Not a stored variable
                varvalue = L"badvar:" + varname;
            }

            CStringW output = iteration.Left(startpos - 1);
            if ( parse && varvalue.GetLength() > 0 )
            {
                CComBSTR varinternal(varvalue);
                this->ParseRaw(varinternal, Flags, ++Depth, State);
                output += CStringW(varinternal);
            }
            output += iteration.Right(endpos);

            iteration = output;
        }

        // Handle slash replacement
        if ( flags == PF_None )
        {
            return iteration;
        } else if ( flags == PF_VarName ) {
            iteration.ToLower();
            return iteration;
        } 
        
        if ( flags & PF_SlashFlags )
        {
            bool singleslash = ( flags & PF_SingleSlash );
            if ( flags & PF_SlashOnly )
            {
                iteration.Replace(L'\\', L'/');
                if ( singleslash )
                {
                    while ( iteration.Replace(L"//", L"/") > 0 ) { }
                }
            } else if ( flags & PF_BackslashOnly ) {
                iteration.Replace(L'/', L'\\');
                if ( singleslash )
                {
                    while ( iteration.Replace(L"\\\\", L"\\") > 0 ) { }
                }
            }

            iteration = output.str();
        }

        if ( flags & PF_CaseFlags )
        {
            if ( flags & PF_Lowercase )
            {
                iteration.ToLower();
            } else if ( flags & PF_Uppercase ) {
                iteration.ToUpper();
            }
        }

        //if ( logger.get() )
        //{
            //mCore->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, "Returning string %s from parser.", iteration.c_str());
        //}

        iteration.SetSysString(pString);

        return S_OK;
    }
}