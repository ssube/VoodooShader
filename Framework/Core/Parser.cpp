#include "Parser.hpp"

#include "Core.hpp"

#include "ILogger.hpp"

#include <sstream>
#include <iostream>

namespace VoodooShader
{
    Parser::Parser(_In_ Core * core)
        : mCore(core)
    {
        mSysVariables["globalroot"] = core->GetGlobalRoot();
        mSysVariables["localroot"] = core->GetLocalRoot();
        mSysVariables["runroot"] = core->GetRunRoot();
    }

    Parser::~Parser()
    {
        mVariables.clear();
    }

    void Parser::AddVariable(_In_ String name, _In_ String value, _In_ bool system)
    {
        mCore->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, "Adding variable \"%s\" with value \"%s\".", name.c_str(), value.c_str());

        String finalname = this->ParseString(name);

        if ( !system )
        {
            mVariables[finalname] = value;
        } else {
            Dictionary::iterator varIter = mSysVariables.find(finalname);
            if ( varIter == mVariables.end() )
            {
                mSysVariables[finalname] = value;
            } else {
                mCore->GetLogger()->Log(LL_Warning, VOODOO_CORE_NAME, "Unable to add duplicate system variable \"%s\".", finalname.c_str());
            }
        }
    }

    void Parser::RemoveVariable(_In_ String name)
    {
        mCore->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, "Removing variable \"%s\".", name.c_str());

        String finalname = this->ParseString(name);

        Dictionary::iterator varIter = mVariables.find(finalname);

        if ( varIter != mVariables.end() )
        {
            mVariables.erase(varIter);
        }
    }

    String Parser::ParseString(_In_ String input, _In_ ParseFlags flags)
    {
        using namespace std;
            
        mCore->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, "Parsing string \"%s\" (%X).", input.c_str(), flags);

        // Parse out any variables in the filename, first
        String iteration = input;

        // Variable parsing loop
        bool foundvar = true;
        while ( foundvar)
        {
            size_t endpos = iteration.find(')');
            if ( endpos != string::npos )
            {
                size_t startpos = iteration.find_last_of('(', endpos);
                if ( startpos != string::npos )
                {
                    if ( startpos > 0 && iteration[startpos-1] == '$' )
                    {
                        stringstream output;
                        String varname = iteration.substr(startpos + 1, endpos - startpos - 1);

                        output << iteration.substr(0, startpos - 1);
                        
                        // Lookup and place the variable
                        Dictionary::iterator variter = mSysVariables.find(varname);
                        if ( variter != mSysVariables.end() )
                        {
                            output << this->ParseString(variter->second, flags);
                        } else {
                            variter = mVariables.find(varname);
                            if ( variter != mSysVariables.end() )
                            {
                                output << this->ParseString(variter->second, flags);
                            } else {
                                // Unrecognized variable, try env
                                size_t reqSize = 0;
                                getenv_s(&reqSize, NULL, 0, varname.c_str());
                                if ( reqSize != 0 )
                                {
                                    char * buffer = new char[reqSize];
                                    getenv_s(&reqSize, buffer, reqSize, varname.c_str());
                                    output << this->ParseString(buffer, flags);
                                    delete[] buffer;
                                } else {
                                    output << "--badvar:" << varname << "--";
                                }
                            }
                        }

                        output << iteration.substr(endpos + 1);

                        iteration = output.str();
                        foundvar = true;
                    } else {
                        foundvar = false;
                    }
                } else {
                    foundvar = false;
                }
            } else {
                foundvar = false;
            }
        }

        // Handle slash replacement
        if ( flags == PF_None )
        {
            return iteration;
        }

        bool singleslash = ( flags & PF_SingleSlash );
        bool prevslash = false;
        bool slashrewrite = false;
        char slashchar = ' ';
        if ( flags & PF_SlashOnly )
        {
            slashrewrite = true;
            slashchar = '/';
        } else if ( flags & PF_BackslashOnly ) {
            slashrewrite = true;
            slashchar = '\\';
        }

        stringstream output;
        size_t total = iteration.length();
        size_t cur = 0;
        while ( cur < total )
        {
            char inchar = iteration[cur];
            ++cur;

            if ( inchar == '/' || inchar == '\\' )
            {
                if ( slashrewrite )
                {
                    inchar = slashchar;
                }

                if ( singleslash && prevslash )
                {
                    continue;
                }

                prevslash = true;
                output << inchar;
            } else {
                prevslash = false;
                output << inchar;
            }
        }

        iteration = output.str();

        mCore->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, "Returning string %s from parser.", iteration.c_str());

        return iteration;
    }
}