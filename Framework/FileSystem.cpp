
#include "FileSystem.hpp"

namespace VoodooShader
{
	template <class T> removeMatching
		: public std::binary_function<T, T, bool>
	{
	public:
		bool operator=(T & arg, T & match)
		{
			return ( arg == match );
		}
	};

	void FileSystem::RegisterDir(String name)
	{
		this->mDirectories.push_front(name);
	}

	void FileSystem::RemoveDir(String name)
	{
		this->mDirectories.remove_if(removeMatching(name));
	}

	FileRef FileSystem::GetFile(String name)
	{
		StringList::iterator curDir = mDirectories.begin();

		while ( curDir != mDirectories.end() )
		{
			// Try to find the file in each registered dir
			String fullname = (*curDir) + ( "\\" + name );

			HANDLE file = CreateFileA(fullname, 0, 0, NULL, OPEN_EXISTING, NULL, NULL);

			if ( file != INVALID_HANDLE_VALUE )
			{
				CloseHandle(file);

				return FileRef(new File(fullname));
			}

			++curDir;
		}

		mCore->GetLog()->Format("Voodoo Core: Unable to find file %s.\n")
			.With(name).Done();

		return FileRef();
	}

	bool File::Open(FileOpenMode mode)
	{
		DWORD access;

		switch ( mode )
		{
		case FM_Read:
			mode = GENERIC_READ;
			break;
		case FM_Write:
			mode = GENERIC_WRITE;
			break;
		case FM_Unknown:
		default:
			Throw("Voodoo Core: Attempted to open file with unknown mode.", mCore);
			break;
		}

		mHandle = CreateFileA(mName, access, 0, NULL, OPEN_EXISTING);

		return ( mHandle != INVALID_HANDLE_VALUE );
	}
}