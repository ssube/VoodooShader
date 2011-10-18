/**
 * \ This file is part of the Voodoo Shader Framework, a comprehensive shader
 * support library. Copyright (c) 2010-2011 by Sean Sube This program is free software;
 * you can redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2 of the License, or (at your option) any later version. This
 * program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details. You
 * should have received a copy of the GNU General Public License along with this program;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth
 * Floor, Boston, MA 02110-1301 US Support and more information may be found at
 * http://www.voodooshader.com, or by contacting the developer at
 * peachykeen@voodooshader.com \ 
 */
#define VOODOO_IMPORT
#define VOODOO_NO_CG
#include "VoodooFramework.hpp"

namespace VoodooShader
{

 /**
  * Primary Voodoo filesystem implementation. This provides a thin wrapper for the
  * Windows file access API and DevIL, matching them to the Voodoo interfaces.
  * @note This module exports 1 class, named @p WFileSystem (@ref
  * VoodooWFS::FileSystem) and provides implementations of @ref IFileSystem, @ref
  * IFile and @ref IImage. The following global variables are added when the
  * filesystem object is created: @li <code>\$(mygames)</code> Current user's My
  * Games folder. @li <code>\$(allgames)</code> Shared (all user's) My Games
  * folder. @li <code>\$(systemroot)</code> System path (e.g.,
  * <code>C:\\Windows\\System32</code>) @addtogroup VoodooWFileSystem
  * Voodoo/WFileSystem @{
  */
 namespace VoodooWFS
{
 typedef std::list<String> StringList;

 Version API_ModuleVersion(void);
 int API_ClassCount(void);
 const char *API_ClassInfo(_In_ int number);
 IObject *API_ClassCreate(_In_ int number, _In_ ICore *core);

 /**
  * Provides a unified file management system for loading shaders and other resources. This file system implementation
  * is a thin wrapper for the Windows API, adding only a few nonstandard functions (directory searching and path
  * variables).
  */
 class FileSystem :
  public IFileSystem
 {

/**
 *
 */
public:
  FileSystem(_In_ ICore *core);

  virtual ~FileSystem(void);

  virtual String ToString(void);
  virtual ICore *GetCore(void);

  /**
   * Add a directory to the search path. Directories are pushed to the front of the
   * list, which is searched in order (last added has highest priority). @note This
   * function uses Parser::ParseString() for all paths. Variables are evaluated when
   * the path is added. @note This function will split paths at the ';
   * ' character, similar to how Windows treats the path variable. This (combined
   * with env vars) allows regular-form environment vars to be added to the
   * filesystem
   */
  virtual void AddDirectory(_In_ String dir);

  virtual void RemoveDirectory(_In_ String dir);

  /**
   * Resolves a relative filename (usually just filename and extension, but may
   * include directories) into an absolute filename by searching the list of
   * resource directories registered with this manager. @param name The filename to
   * search for. @return If the file is found, a reference to an unopened file
   * object is returned. If no file is found, an empty reference is returned. @note
   * This functions uses Parser::ParseString() on the path. It is evaluated once
   * when the function enters, then appended to each stored path until a valid file
   * is found
   */
  virtual IFile* FindFile(_In_ String name);

/**
 *
 */
private:
  StringList m_Directories;
  ICore *m_Core;
 };

 class File :
  public IFile
 {

/**
 *
 */
public:

  /**
   * Creates a file object from a path (usually an absolute path). This should
   * usually not be called directly, FileManager::GetFile(String) will automatically
   * resolve and return paths to simplify things.
   */
  File(_In_ ICore *core, _In_ String name);

  virtual ~File(void);

  virtual String ToString(void);
  virtual ICore *GetCore(void);

  /**
   * Retrieves the path this File was created with. This may be relative or
   * absolute. @return Internal path.
   */
  virtual String GetPath(void);

  /**
   * Opens the file for read-write access. @param mode The mode to open the file in.
   * @return Whether the file was successfully opened. @note If this file handle was
   * returned by a FileSystem and the absolute path is still valid, this function
   * should always succeed. In single-threaded or fullscreen scenarios with few user
   * tasks, this is usually the case. @warning If this file handle uses a relative
   * path, then it is subject to changes in working directory by the any module in
   * the process. FileSystem::GetFile() uses an absolute path in the constructor and
   * is not susceptible to this.
   */
  virtual bool Open(_In_ FileOpenMode mode);

  virtual IImage* OpenImage(void);

  /* Closes the file, preventing further access. @return Success of the operation */
  virtual bool Close(void);

  /**
   * Reads a chunk of data from the file. The file must have been opened for reading
   * previously, or this call will fail. @param count The number of bytes to read,
   * -1 for all. @param buffer The buffer to be read into (may be nullptr, see notes).
   * @return The number of bytes read. @note If @arg buffer is nullptr, the number of
   * bytes that would have been read is returned but the file position is unchanged.
   * If @arg count is -1, this returns the filesize remaining
   */
  virtual int Read(_In_ int count, _In_opt_count_(count) void *buffer);

  /**
   * Writes a chunk of data to the file. The file must have been opened for writing.
   * @param count The number of bytes to write. @param buffer The data to write.
   * @return Success of the write operation. @note If @p buffer is nullptr, @p count
   * zeros are written into the file. This is useful for padding binary formats.
   */
  virtual bool Write(_In_ int count, _In_opt_count_(count) void *buffer);

/**
 *
 */
private:
  HANDLE m_Handle;
  String m_Name;
  ICore *m_Core;
 };

 /**
  * Provides image loading, using the DevIL library. This class provides internal loading and conversion, and can manage
  * 1-3 dimensional images (regular textures and volumes). @todo Provide image saving. @todo Provide layer, cubemap and
  * animation handling.
  */
 class Image :
  public IImage
 {

/**
 *
 */
public:
  static IImage* Load(ICore *core, String name);

  Image(ICore *core, String name, unsigned int image);

  virtual ~Image(void);

  virtual String ToString(void);
  virtual ICore *GetCore(void);

  virtual String GetPath(void);

  /**
   * Retrieves texture format data from the image. The runtime can use this to set
   * up a texture or decide if it can handle the texture format. @return Texture
   * information.
   */
  virtual TextureDesc GetDesc(void);

  /**
   * Retrieves a portion of the texture data from the image. @param desc The region
   * and format to be returned. @param buffer The memory for the return data to be
   * placed in. Must already be allocated, or nullptr. @return The number of bytes
   * retrieved (or, if @p buffer is nullptr, the number that would be retrieved).
   * @throws Exception on invalid texture format. @warning Due to limitations in
   * this library (or DevIL, not sure which), the only texture formats this function
   * can convert into are @ref TF_RGB8, @ref TF_RGBA8, @ref TF_RGBA16F and @ref
   * TF_RGBA32F. Others are not supported and will cause this function to throw.
   * @note This can convert data between most formats, so the format given in @p
   * desc will be the returned format. This makes calculating the buffer size
   * relatively easy. @warning If this function converts formats or copies a large
   * region, it will be slow. Avoid calling often.
   */
  virtual size_t GetData(_In_ TextureRegion desc, _In_opt_ void *buffer);

/**
 *
 */
private:
  ICore *m_Core;
  IParser* m_Parser;
  String m_Name;
  unsigned int m_Image;
  TextureDesc m_Desc;
 };
}
}

/* @} */
