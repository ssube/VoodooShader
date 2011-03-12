#include "Includes.hpp"
#include "FileSystem.hpp"
#include "Core.hpp"
#include "Version.hpp"

namespace VoodooShader
{
    void FileSystem::AddDirectory(String name)
    {
        this->mDirectories.push_front(name);
    }

    void FileSystem::RemoveDirectory(String name)
    {
        //! @todo Look into alternatives for lambdas in FileSystem::RemoveDirectory().
        this->mDirectories.remove_if([&](String current){return (current == name);});
    }

    FileRef FileSystem::GetFile(String name)
    {
        StringList::iterator curDir = mDirectories.begin();

        while ( curDir != mDirectories.end() )
        {
            // Try to find the file in each registered dir
            String fullname = (*curDir) + ( "\\" + name );

            HANDLE file = CreateFileA(fullname.c_str(), 0, 0, NULL, OPEN_EXISTING, NULL, NULL);

            if ( file != INVALID_HANDLE_VALUE )
            {
                CloseHandle(file);

                mCore->Log
                (
                    LL_Debug,
                    VOODOO_CORE_NAME,
                    "File %s found in directory %s.",
                    name.c_str(), (*curDir).c_str()
                );

                return FileRef(new File(mCore, fullname));
            }

            ++curDir;
        }

        mCore->Log
        (
            LL_Warning,
            VOODOO_CORE_NAME,
            "Unable to find file %s.", 
            name.c_str()
        );

        return FileRef();
    }

    File::File(Core * core, String name)
        : mName(name), mCore(core), mHandle(NULL)
    {    };

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
            Throw(VOODOO_CORE_NAME, "Attempted to open file with unknown mode.", mCore);
        }

        mCore->Log
        (
            LL_Debug,
            VOODOO_CORE_NAME,
            "Opening file %s with mode %x (underlying %u).",
            mName.c_str(), mode, access
        );

        mHandle = CreateFileA(mName.c_str(), access, 0, NULL, OPEN_EXISTING, NULL, NULL);

        assert(mHandle);

        return ( mHandle != INVALID_HANDLE_VALUE );
    }
}
