/**************************************************************************************************\
* This file is part of the Voodoo Shader Framework, a comprehensive shader support library.
* Copyright (c) 2010-2011 by Sean Sube
*
*
* This program is free software; you can redistribute it and/or modify it under the terms of the 
* GNU General Public License as published by the Free Software Foundation; either version 2 of the 
* License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
* even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; 
* if  not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
* Boston, MA  02110-1301 US
*
* Support and more information may be found at http://www.voodooshader.com, or by contacting the
* developer at peachykeen@voodooshader.com
\**************************************************************************************************/

#include "Meta.hpp"
#include "Core.hpp"

namespace VoodooShader
{
	/**
	 * Provides a unified file management system for loading shaders and other
	 * resources.
	 */
	class FileSystem
	{
	public:
		FileSystem(Core * core);
		~FileSystem();

		void RegisterDir(String dir);
		void RemoveDir(String dir);

		/**
		 * Resolves a relative filename (usually just filename and extension,
		 * but may include directories) into an absolute filename by searching
		 * the list of resource directories registered with this manager.
		 *
		 * @param name The filename to search for
		 * @return If the file is found, a reference to a file object pointing 
		 *		to the file with an absolute path. Otherwise, an empty 
		 *		reference.
		 */
		FileRef GetFile(String name);

	private:
		StringList mDirectories;
		Core * mCore;
	};

	class File
	{
	public:
		/**
		 * Creates a file object from a path (usually an absolute path). This
		 * should usually not be called directly, FileManager::GetFile(String)
		 * will automatically resolve and return paths to simplify things.
		 */
		File(Core * core, String name);

		bool Open(FileOpenMode mode);

	private:
		HANDLE mHandle;
		String mName;
		Core * mCore;
	};
}