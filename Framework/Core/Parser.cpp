#include "Parser.hpp"

#include "Core.hpp"

#include <sstream>
#include <iostream>

namespace VoodooShader
{
    Parser::Parser(Core * core)
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

    void Parser::AddVariable(String name, String value)
    {
        // Check the string against a reserved name
        if ( ( name == "localroot" ) || ( name == "globalroot" ) || ( name == "runroot" ) )
        {
            return;
        }

        mVariables[name] = ParseString(value);
    }

    void Parser::RemoveVariable(String name)
    {
        //mVariables.remove_if()
    }

    String Parser::ParseString(String input, ParseFlags flags)
    {
        using namespace std;

        // Parse out any variables in the filename, first
        stringstream output;
        stringstream cvar;

        bool dest = false;

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

        size_t total = input.length();
        size_t pos = 0;
        while ( pos < total )
        {
            char inchar = input[pos];

            if ( inchar != '$' )
            {
                if ( inchar == '/' || inchar == '\\' )
                {
                    if ( singleslash && prevslash )
                    {
                        ++pos;
                        continue;
                    } else {
                        prevslash = true;
                        if ( slashrewrite )
                        {
                            output << slashchar;
                        } else {
                            if ( dest == false )
                            {
                                output << inchar;
                            } else {
                                cvar << inchar;
                            }
                        }
                    }
                } else {
                    // Not a slash
                    prevslash = false;
                    if ( dest == false )
                    {
                        output << inchar;
                    } else {
                        cvar << inchar;
                    }
                }
            } else {
                // Is a $
                if ( dest == false )
                {
                    // Start of a var name
                    cvar.clear();
                    dest = true;
                } else {
                    // End of a var name
                    String var = cvar.str();

                    if ( var.length() == 0 )
                    {
                        // Escaped $, put
                        output << "$";
                    } else {
                        // Variable name, find
                        Dictionary::iterator variter = mVariables.find(var);

                        if ( variter != mVariables.end() )
                        {
                            output << variter->second;
                        } else {
                            // Unrecognized variable, try env
                            size_t reqSize = 0;
                            getenv_s(&reqSize, NULL, 0, var.c_str());
                            if ( reqSize != 0 )
                            {
                                char * buffer = new char[reqSize];
                                getenv_s(&reqSize, buffer, reqSize, var.c_str());
                                output << buffer;
                                delete[] buffer;
                            } else {
                                output << "--badvar:" << var << "--";
                            }
                        }
                    }
                    dest = false;
                }
            }
            ++pos;
        }

        return output.str();
    }
}