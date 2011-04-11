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

        String finalname = this->ParseString(name, PF_Lowercase);

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

        String finalname = this->ParseString(name, PF_Lowercase);

        Dictionary::iterator varIter = mVariables.find(finalname);

        if ( varIter != mVariables.end() )
        {
            mVariables.erase(varIter);
        }
    }

    String Parser::ParseString(_In_ String input, _In_ ParseFlags flags)
    {
        Dictionary parseState;

        return this->ParseStringRaw(input, (ParseFlags)(flags | PF_RetainState), 0, parseState);
    }

    String Parser::ParseStringRaw(_In_ String input, _In_ ParseFlags flags, _In_ int depth, _In_ Dictionary & state)
    {
        using namespace std;

        if ( depth > Parser::mMaxDepth )
        {
            return input;
        }

        ILoggerRef logger = mCore->GetLogger();
        if ( logger.get() )
        {
            mCore->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, "Parsing string \"%s\" (%X).", input.c_str(), flags);
        }

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

                        output << iteration.substr(0, startpos - 1);

                        size_t varnamelen = endpos - startpos - 1;
                        if ( varnamelen == 0 )
                        {
                            output << iteration.substr(endpos + 1);

                            iteration = output.str();
                            foundvar = true;
                            continue;
                        }

                        String varname = this->ToLower(iteration.substr(startpos + 1, endpos - startpos - 1));
                        bool supress = false;

                        if ( varname[0] == '?' )
                        {
                            supress = true;
                            varname = varname.substr(1);
                        }

                        size_t equalpos = varname.find(':');
                        if ( equalpos != string::npos )
                        {
                            // State set, handle
                            String newvalue = varname.substr(equalpos + 1);
                            if ( newvalue.length() > 0 )
                            {
                                newvalue = this->ParseStringRaw(newvalue, flags, ++depth, state);
                            }

                            varname = varname.substr(0, equalpos - 1);
                            if ( varname.length() > 0 )
                            {
                                varname = this->ParseStringRaw(varname, flags, ++depth, state);
                            }

                            //if ( varname == "depth" )
                            //{
                            //    newvalue >> depth;
                            //} else {
                                state[varname] = newvalue;
                            //}
                        } else {
                            // Lookup and place the variable
                            Dictionary::iterator variter = mSysVariables.find(varname);
                            String varvalue;

                            // Find variable's value
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
                                            varvalue = variter->second;
                                            delete[] buffer;
                                        } else {
                                            if ( !supress )
                                            {
                                                output << "--badvar:" << varname << "--";
                                            }
                                        }
                                    }
                                }
                            }

                            if ( varvalue.length() > 0 )
                            {
                                output << this->ParseStringRaw(variter->second, flags, ++depth, state);
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

        if ( flags & PF_Lowercase )
        {
            iteration = this->ToLower(iteration);
        } else if ( flags & PF_Uppercase ) {
            iteration = this->ToUpper(iteration);
        }

        if ( logger.get() )
        {
            mCore->GetLogger()->Log(LL_Debug, VOODOO_CORE_NAME, "Returning string %s from parser.", iteration.c_str());
        }

        return iteration;
    }
}