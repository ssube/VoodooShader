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

    String Parser::ToLower(_In_ String input)
    {
        String ret = input;
        std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
        return ret;
    }

    String Parser::ToUpper(_In_ String input)
    {
        String ret = input;
        std::transform(ret.begin(), ret.end(), ret.begin(), ::toupper);
        return ret;
    }

    void Parser::AddVariable(_In_ String name, _In_ String value, _In_ bool system)
    {
        ILoggerRef logger = mCore->GetLogger();
        if ( logger.get() )
        {
            logger->Log(LL_Debug, VOODOO_CORE_NAME, "Adding variable \"%s\" with value \"%s\".", name.c_str(), value.c_str());
        }

        String finalname = this->ParseString(name, PF_VarName);

        if ( !system )
        {
            mVariables[finalname] = value;
        } else {
            Dictionary::iterator varIter = mSysVariables.find(finalname);
            if ( varIter == mSysVariables.end() )
            {
                mSysVariables[finalname] = value;
            } else {
                if ( logger.get() )
                {
                    logger->Log(LL_Warning, VOODOO_CORE_NAME, "Unable to add duplicate system variable \"%s\".", finalname.c_str());
                }
            }
        }
    }

    void Parser::RemoveVariable(_In_ String name)
    {
        ILoggerRef logger = mCore->GetLogger();
        if ( logger.get() )
        {
            mCore->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, "Removing variable \"%s\".", name.c_str());
        }

        String finalname = this->ParseString(name, PF_VarName);

        Dictionary::iterator varIter = mVariables.find(finalname);

        if ( varIter != mVariables.end() )
        {
            mVariables.erase(varIter);
        }
    }

    String Parser::ParseString(_In_ String input, _In_ ParseFlags flags)
    {
        Dictionary parseState;

        return this->ParseStringRaw(input, flags, 0, parseState);
    }

    String Parser::ParseStringRaw(_In_ String input, _In_ ParseFlags flags, _In_ int depth, _In_ Dictionary & state)
    {
        using namespace std;

        ILoggerRef logger = mCore->GetLogger();
        if ( logger.get() )
        {
            mCore->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, "Parsing string \"%s\" (%X).", input.c_str(), flags);
        }

        if ( depth > Parser::VarMaxDepth || input.length() < 3 )
        {
            return input;
        }

        // Parse out any variables in the filename, first
        String iteration = input;

        // Variable parsing loop
        bool loop = true;
        while ( loop )
        {
            const size_t itlen = iteration.length();
            if ( itlen < 3 )
            {
                // Stop parsing if the string is too small to contain variables 
                // (this should never be hit the first loop)
                break;
            }

            size_t endpos = iteration.find_first_of(Parser::VarDelimEnd);
            if ( endpos == string::npos )
            {
                // Stop parsing if no closing delimiter is found
                break;
            }
            
            size_t startpos = iteration.find_last_of(Parser::VarDelimStart, endpos);
            if ( startpos == string::npos || 
                 startpos == 0 || 
                 iteration[startpos-1] != Parser::VarDelimPre 
               )
            {
                // Stop parsing if no opening sequence is found, or there is no room for one
                break;
            }

            size_t varnamelen = endpos - startpos - 1;
            if ( varnamelen == 0 )
            {
                // Erase the variable sequence if it is the empty variable and restart the loop
                stringstream output;
                output << iteration.substr(0, startpos - 1);
                if ( endpos < itlen )
                {
                    output << iteration.substr(endpos + 1);
                }
                iteration = output.str();
                continue;
            }

            // We may actually have a variable here. Get the name
            String varname = iteration.substr(startpos + 1, varnamelen);

            // Check for state variables
            size_t statepos = varname.find(':');
            if ( statepos != string::npos )
            {
                // State set, handle
                String newvalue = varname.substr(statepos + 1);
                newvalue = this->ParseStringRaw(newvalue, flags, ++depth, state);

                varname = varname.substr(0, statepos);
                varname = this->ParseStringRaw(varname, PF_VarName, ++depth, state);

                state[varname] = newvalue;

                stringstream output;
                output << iteration.substr(0, startpos - 1) << iteration.substr(endpos + 1);
                iteration = output.str();

                continue;
            } 

            // Handle variable flags
            bool supress = false;
            bool parse = true;
            // The length of varname is > 0, guaranteed in line 134
            if ( varname[0] == '?' )
            {
                supress = true;
                varname = varname.substr(1);
            } else if ( varname[0] == '!' ) {
                parse = false;
                varname = varname.substr(1);
            }

            // Properly format the variable name (recursive call to simplify future syntax exts)
            varname = this->ParseStringRaw(varname, PF_VarName, ++depth, state);

            // Lookup and replace the variable
            bool foundvar = true;
            String varvalue;

            Dictionary::iterator variter = mSysVariables.find(varname);
            if ( variter != mSysVariables.end() )
            {
                varvalue = variter->second;
            } else {
                variter = state.find(varname);
                if ( variter != state.end() )
                {
                    varvalue = variter->second;
                } else {
                    variter = mVariables.find(varname);
                    if ( variter != mVariables.end() )
                    {
                        varvalue = variter->second;
                    } else {
                        // Unrecognized variable, try env
                        size_t reqSize = 0;
                        getenv_s(&reqSize, NULL, 0, varname.c_str());
                        if ( reqSize != 0 )
                        {
                            char * buffer = new char[reqSize];
                            getenv_s(&reqSize, buffer, reqSize, varname.c_str());
                            varvalue = buffer;
                            delete[] buffer;
                        } else {
                            foundvar = false;
                        }
                    }
                }
            }

            stringstream output;
            output << iteration.substr(0, startpos - 1);
            if ( parse && varvalue.length() > 0 )
            {
                output << this->ParseStringRaw(varvalue, flags, ++depth, state);
            } else if ( !foundvar && !supress ) {
                output << "--badvar:" << varname << "--";
            }
            output << iteration.substr(endpos + 1);

            iteration = output.str();
        }

        // Handle slash replacement
        if ( flags == PF_None )
        {
            return iteration;
        } else if ( flags == PF_VarName ) {
            return this->ToLower(iteration);
        } 
        
        if ( flags & PF_SlashFlags )
        {
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
        }

        if ( flags & PF_CaseFlags )
        {
            if ( flags & PF_Lowercase )
            {
                iteration = this->ToLower(iteration);
            } else if ( flags & PF_Uppercase ) {
                iteration = this->ToUpper(iteration);
            }
        }

        if ( logger.get() )
        {
            mCore->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, "Returning string %s from parser.", iteration.c_str());
        }

        return iteration;
    }
}