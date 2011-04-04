#include "Parser.hpp"

#include "Core.hpp"

#include <sstream>
#include <iostream>

namespace VoodooShader
{
    Parser::Parser(_In_ Core * core)
        : mCore(core)
    {
        mVariables["globalroot"] = core->GetGlobalRoot();
        mVariables["localroot"] = core->GetLocalRoot();
        mVariables["runroot"] = core->GetRunRoot();
    }

    Parser::~Parser()
    {
        mVariables.clear();
    }

    void Parser::AddVariable(_In_ String name, _In_ String value)
    {
        String finalname = this->ParseString(name);

        // Check the string against a reserved name
        if ( ( finalname == "localroot" ) || ( finalname == "globalroot" ) || ( finalname == "runroot" ) )
        {
            return;
        }

        mVariables[name] = value;
    }

    void Parser::RemoveVariable(_In_ String name)
    {
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
                        Dictionary::iterator variter = mVariables.find(varname);
                        if ( variter != mVariables.end() )
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

        return output.str();
    }
}