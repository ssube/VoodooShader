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
            name
        );

        return FileRef();
    }

    bool File::Open(FileOpenMode mode)
    {
        DWORD access;

        switch ( mode )
        {
        case FM_Read:
            access = GENERIC_READ;
            break;
        case FM_Write:
            access = GENERIC_WRITE;
            break;
        case FM_Count:
        case FM_Unknown:
        default:
            Throw(VOODOO_CORE_NAME, "Attempted to open file with unknown mode.", mCore);
            break;
        }

        mHandle = CreateFileA(mName.c_str(), access, 0, NULL, OPEN_EXISTING, NULL, NULL);

        return ( mHandle != INVALID_HANDLE_VALUE );
    }

    File::File(Core * core, String name)
        : mName(name), mCore(core), mHandle(NULL)
    {    };
}
