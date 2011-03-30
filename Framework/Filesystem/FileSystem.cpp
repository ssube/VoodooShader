#include "FileSystem.hpp"

#include "Filesystem_Version.hpp"

#include "IL\il.h"
#include <sstream>
#include <iostream>

namespace VoodooShader
{
    namespace VoodooWVFS
    {
        Version API_ModuleVersion()
        {
            Version version = VOODOO_META_VERSION_STRUCT(FILESYSTEM);
            return version;
        }

        int API_ClassCount()
        {
            return 3;
        }

        const char * API_ClassInfo(int number)
        {
            if ( number == 0 )
            {
                return "Filesystem";
            } else {
                return NULL;
            }
        }

        IObject * API_ClassCreate( _In_ int number, _In_ Core * core )
        {
            if ( number == 0 )
            {
                return new FileSystem(core);
            } else {
                return NULL;
            }
        }

        FileSystem::FileSystem( _In_ Core * core )
            : mCore(core)
        {
            ilInit();
        }

        FileSystem::~FileSystem()
        {
            mDirectories.clear();
        }

        const char * FileSystem::GetObjectClass()
        {
            return "FileSystem";
        }

        void FileSystem::AddDirectory(String name)
        {
            String realname = this->ParsePath(name) + "\\";

            this->mDirectories.push_front(realname);
        }

        void FileSystem::RemoveDirectory(String name)
        {
            String realname = this->ParsePath(name) + "\\";

            this->mDirectories.remove_if([&](String current){return (current == realname);});
        }

        String FileSystem::ParsePath(_In_ String rawpath)
        {
            using namespace std;

            // Parse out any variables in the filename, first
            stringstream finalname;
            stringstream varname;
            bool dest = false;

            size_t total = rawpath.length();
            size_t pos = 0;
            while ( pos < total )
            {
                if ( rawpath[pos] != '$' )
                {
                    if ( dest == false )
                    {
                        finalname << rawpath[pos];
                    } else {
                        varname << rawpath[pos];
                    }
                } else {
                    if ( dest == false )
                    {
                        // Start of a var
                        varname.clear();
                        dest = true;
                    } else {
                        // End
                        String var = varname.str();
                        if ( var == "localroot" )
                        {
                            finalname << mCore->GetLocalRoot();
                            //"--localroot--";
                        } else if ( var == "globalroot" ) {
                            finalname << mCore->GetGlobalRoot();
                            //"--globalroot--";
                        } else {
                            finalname << "--error--";
                        }
                        dest = false;
                    }
                }
                ++pos;
            }

            return finalname.str();
        }

        IFileRef FileSystem::GetFile(String name)
        {
            mCore->GetLogger()->Log(LL_Debug, VOODOO_FILESYSTEM_NAME, "Searching for raw file \"%s\".", name.c_str());

            String filename = this->ParsePath(name);
            mCore->GetLogger()->Log(LL_Debug, VOODOO_FILESYSTEM_NAME, "Searching for parsed file \"%s\".", filename.c_str());

            // 
            StringList::iterator curDir = mDirectories.begin();

            while ( curDir != mDirectories.end() )
            {
                // Try to find the file in each registered dir
                String fullname = (*curDir) + ( "\\" + filename );

                HANDLE file = CreateFileA(fullname.c_str(), 0, 0, NULL, OPEN_EXISTING, NULL, NULL);

                if ( file != INVALID_HANDLE_VALUE )
                {
                    CloseHandle(file);

                    mCore->GetLogger()->Log
                    (
                        LL_Debug,
                        VOODOO_CORE_NAME,
                        "File %s found in directory %s.",
                        name.c_str(), (*curDir).c_str()
                    );

                    return IFileRef(new File(mCore, fullname));
                }

                ++curDir;
            }

            mCore->GetLogger()->Log
            (
                LL_Warning,
                VOODOO_CORE_NAME,
                "Unable to find file %s.", 
                name.c_str()
            );

            return IFileRef();
        }

        IImageRef FileSystem::GetImage(_In_ String name)
        {
            mCore->GetLogger()->Log(LL_Debug, VOODOO_FILESYSTEM_NAME, "Searching for raw image \"%s\".", name.c_str());

            String filename = this->ParsePath(name);
            mCore->GetLogger()->Log(LL_Debug, VOODOO_FILESYSTEM_NAME, "Searching for parsed image \"%s\".", filename.c_str());

            // 
            StringList::iterator curDir = mDirectories.begin();

            while ( curDir != mDirectories.end() )
            {
                // Try to find the file in each registered dir
                String fullname = (*curDir) + ( "\\" + filename );

                HANDLE file = CreateFileA(fullname.c_str(), 0, 0, NULL, OPEN_EXISTING, NULL, NULL);

                if ( file != INVALID_HANDLE_VALUE )
                {
                    CloseHandle(file);

                    mCore->GetLogger()->Log
                    (
                        LL_Debug,
                        VOODOO_CORE_NAME,
                        "File %s found in directory %s.",
                        name.c_str(), (*curDir).c_str()
                    );

                    // Do DevIL loading
                    ILuint image = ilGenImage();
                    ilBindImage(image);

                    ILboolean loaded = ilLoadImage(fullname.c_str());

                    if ( loaded == IL_FALSE )
                    {
                        return IImageRef();
                    } else {
                        return IImageRef(new Image(mCore, image));
                    }
                }

                ++curDir;
            }

            mCore->GetLogger()->Log
            (
                LL_Warning,
                VOODOO_CORE_NAME,
                "Unable to find file %s.", 
                name.c_str()
            );

            return IImageRef();
        }

        File::File(Core * core, String name)
            : mName(name), mCore(core), mHandle(NULL)
        {    };

        File::~File()
        {
            this->Close();
        }

        const char * File::GetObjectClass()
        {
            return "File";
        }

        String File::GetPath()
        {
            return mName;
        }

        bool File::Open(FileOpenMode mode)
        {
            DWORD access = 0;

            if ( mode & FM_Read )
            {
                access |= GENERIC_READ;
            }

            if ( mode & FM_Write )
            {
                access |= GENERIC_WRITE;
            }

            if ( access == 0 )
            {
                Throw(VOODOO_FILESYSTEM_NAME, "Attempted to open file with unknown mode.", mCore);
            }

            mCore->GetLogger()->Log
            (
                LL_Debug,
                VOODOO_FILESYSTEM_NAME,
                "Opening file %s with mode %u (underlying %u).",
                mName.c_str(), mode, access
            );

            mHandle = CreateFileA(mName.c_str(), access, 0, NULL, OPEN_EXISTING, NULL, NULL);

            if ( mHandle == INVALID_HANDLE_VALUE )
            {
                mCore->GetLogger()->Log
                (
                    LL_Warning,
                    VOODOO_FILESYSTEM_NAME,
                    "Unable to open file %s.",
                    mName.c_str()
                );
            }

            return ( mHandle != INVALID_HANDLE_VALUE );
        }

        bool File::Close()
        {
            if ( mHandle && mHandle != INVALID_HANDLE_VALUE )
            {
                BOOL retval = CloseHandle(mHandle);
                return ( retval == TRUE );
            } else {
                return false;
            }
        }

        int File::Read(_In_ int count, _In_opt_count_(count) void * buffer)
        {
            UNREFERENCED_PARAMETER(count);
            UNREFERENCED_PARAMETER(buffer);

            if ( mHandle )
            {
                //ReadFile(mHandle, buffer, )
            }

            return 0;
        }

        bool File::Write(_In_ int count, _In_opt_count_(count) void * buffer )
        {
            UNREFERENCED_PARAMETER(count);
            UNREFERENCED_PARAMETER(buffer);

            return false;
        }

        Image::Image(Core * core, ILuint image)
            : mCore(core), mImage(image)
        {

        }

        Image::~Image()
        {
            ilDeleteImage(mImage);
        }

        const char * Image::GetObjectClass()
        {
            return "Image";
        }

        TextureDesc Image::GetImageDesc()
        {
            TextureDesc desc;
            ZeroMemory(&desc, sizeof(desc));
            return desc;
        }

        void * Image::GetImageData()
        {
            ilBindImage(mImage);
            return ilGetData();
        }
    }
}
