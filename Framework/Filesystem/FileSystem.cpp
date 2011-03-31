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
                if ( rawpath[pos] == '/' ) 
                {
                    if ( dest == false )
                    {
                        finalname << "\\";
                    } else {
                        varname << "\\";
                    }
                } else if ( rawpath[pos] != '$' )
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
                String fullname = (*curDir) + filename;

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

                    return Image::Load(mCore, fullname);
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
            if ( mHandle )
            {
                DWORD readsize;
                if ( count < 0 )
                {
                    readsize = GetFileSize(mHandle, NULL);
                } else {
                    readsize = (DWORD)count;
                }

                DWORD retsize = 0;
                BOOL success = ReadFile(mHandle, buffer, readsize, &retsize, NULL);

                if ( success == 0 )
                {
                    return 0;
                } else {
                    return (int)retsize;
                }
            }

            return 0;
        }

        bool File::Write(_In_ int count, _In_opt_count_(count) void * buffer )
        {
            if ( mHandle )
            {
                if ( buffer == NULL )
                {
                    DWORD size = 0;
                    if ( count < 0 )
                    {
                        Throw(VOODOO_FILESYSTEM_NAME, "Unable to write a negative number of null bytes.", mCore);
                    } else {
                        size = (DWORD)count;
                    }

                    buffer = malloc(size);

                    if ( buffer == NULL )
                    {
                        Throw(VOODOO_FILESYSTEM_NAME, "Error allocating memory for null buffer.", mCore);
                    }

                    memset(buffer, 0, size);
                    DWORD retsize = 0;
                    BOOL success = WriteFile(mHandle, buffer, size, &retsize, NULL);

                    free(buffer);

                    if ( success == 0 )
                    {
                        return false;
                    } else if ( retsize != size ) {
                        return false;
                    } else {
                        return true;
                    }
                } else {
                    if ( count < 0 )
                    {
                        Throw(VOODOO_FILESYSTEM_NAME, "Unable to write a negative nubber of bytes.", mCore);
                    }

                    DWORD size = (DWORD)count;
                    DWORD retsize = 0;
                    BOOL success = WriteFile(mHandle, buffer, size, &retsize, NULL);

                    if ( sucess == 0 )
                    {
                        return false;
                    } else if ( retsize != size ) {
                        return false;
                    } else {
                        return true;
                    }
                }
            }

            return false;
        }

        IImageRef Image::Load(Core * core, String name)
        {
            ILuint image = ilGenImage();
            ilBindImage(image);

            ILboolean loaded = ilLoadImage(name.c_str());

            if ( loaded == IL_FALSE )
            {
                ilBindImage(0);
                ilDeleteImage(image);
                return IImageRef();
            } else {
                return IImageRef(new Image(core, name, image));
            }
        }

        Image::Image(Core * core, String name, ILuint image)
            : mCore(core), mName(name), mImage(image)
        {
            mDesc.Width = ilGetInteger(IL_IMAGE_WIDTH);
            mDesc.Height = ilGetInteger(IL_IMAGE_HEIGHT);
            mDesc.Depth = ilGetInteger(IL_IMAGE_DEPTH);
            mDesc.Mipmaps = false;
            mDesc.Format = TF_Unknown;

            ILint texFmt  = ilGetInteger(IL_IMAGE_FORMAT);
            ILint texType = ilGetInteger(IL_IMAGE_TYPE);

            if ( texFmt == IL_RGB )
            {
                if ( texType == IL_BYTE || texType == IL_UNSIGNED_BYTE )
                {
                    mDesc.Format = TF_RGB8;
                }
            } else if ( texFmt == IL_RGBA ) {
                if ( texType == IL_BYTE || texType == IL_UNSIGNED_BYTE )
                {
                    mDesc.Format = TF_RGBA8;
                } else if ( texType == IL_FLOAT ) {
                    mDesc.Format = TF_RGBA16F;
                } else if ( texType == IL_DOUBLE ) {
                    mDesc.Format = TF_RGBA32F;
                }
            }

            if ( mDesc.Format == TF_Unknown )
            {
                mCore->GetLogger()->Log(LL_Warning, VOODOO_FILESYSTEM_NAME, "Unable to resolve format for image %s (%u).", name.c_str(), image);
            }
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
            return mDesc;
        }

        size_t Image::CopyImageData(TextureRegion desc, void * buffer)
        {
            ILint ilFmt = 0;
            ILint ilType = 0;

            // Convert TextureFormat to DevIL format
            switch ( desc.Format )
            {
            case TF_RGB8:
                ilFmt = IL_RGB;
                ilType = IL_UNSIGNED_BYTE;
                break;
            case TF_RGBA8:
                ilFmt = IL_RGBA;
                ilType = IL_UNSIGNED_BYTE;
                break;
            case TF_RGBA16F:
                ilFmt = IL_RGBA;
                ilType = IL_FLOAT;
                break;
            case TF_RGBA32F:
                ilFmt = IL_RGBA;
                ilType = IL_DOUBLE;
                break;
            case TF_Unknown:
            default:
                Throw(VOODOO_FILESYSTEM_NAME, "Invalid texture format for data get.", mCore);
            }

            return ilCopyPixels(desc.OffX, desc.OffY, desc.OffZ, desc.Width, desc.Height, desc.Depth, ilFmt, ilType, buffer);
        }

        void * Image::GetImageData()
        {
            ilBindImage(mImage);
            return ilGetData();
        }

        void Image::FreeImageData()
        {
            ilBindImage(0);
        }
    }
}
