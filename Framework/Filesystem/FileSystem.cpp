#include "FileSystem.hpp"

#include "Filesystem_Version.hpp"

#include "IL/il.h"
#include "pugixml.hpp"

// The MS shlobj header contains a few functions that cause errors in analysis
// under /W4 (and cause the build to fail under /WX). This disables the warning
// for only that header.
#pragma warning(push)
#pragma warning(disable : 6387)
#include <shlobj.h>
#pragma warning(pop)
#include <strsafe.h>

namespace VoodooShader
{
 namespace VoodooWFS
{
 const char *FileSystemName = "WFileSystem";

 /**
  *
  */
 Version API_ModuleVersion(void)
 {

  Version version = VOODOO_META_VERSION_STRUCT(FILESYSTEM);

  return version;
 }

 /**
  *
  */
 int API_ClassCount(void)
 {
  return 1;
 }

 /**
  *
  */
 const char *API_ClassInfo(int number)
 {
  if (number == 0)
  {
   return FileSystemName;
  }
  else
  {
   return nullptr;
  }
 }

 /**
  *
  */
 IObject *API_ClassCreate(_In_ int number, _In_ ICore *core)
 {
  if (number == 0)
  {
   return new FileSystem(core);
  }
  else
  {
   return nullptr;
  }
 }

 /**
  *
  */
 FileSystem::FileSystem(_In_ ICore *core) :
  m_Core(core)
 {

  using namespace pugi;
  // Create builtin vars
  IParser* parser = m_Core->GetParser();
  char cvar[MAX_PATH];

  if (SHGetFolderPath(nullptr, CSIDL_COMMON_DOCUMENTS, nullptr, SHGFP_TYPE_CURRENT, cvar) == S_OK)
  {
   StringCchCatA(cvar, MAX_PATH, "\\My Games\\");
   parser->Add("allgames", cvar, VT_System);
  }

  if (SHGetFolderPath(nullptr, CSIDL_PERSONAL, nullptr, SHGFP_TYPE_CURRENT, cvar) == S_OK)
  {
   StringCchCatA(cvar, MAX_PATH, "\\My Games\\");
   parser->Add("mygames", cvar, VT_System);
  }

  if (SHGetFolderPath(nullptr, CSIDL_SYSTEM, nullptr, SHGFP_TYPE_CURRENT, cvar) == S_OK)
  {
   parser->Add("systemroot", cvar, VT_System);
  }

  // Init DevIL
  ilInit();


  // Load paths from the config
  xml_document *config = (xml_document *) m_Core->GetConfig();
  xpath_query pathsQuery("/VoodooConfig/WFileSystem/SearchPaths/Path");
  xpath_query pathValueQuery("./text()");
  xpath_node_set pathNodes = pathsQuery.evaluate_node_set(*config);
  xpath_node_set::const_iterator pathIter = pathNodes.begin();

  while (pathIter != pathNodes.end())
  {

   String path = pathValueQuery.evaluate_string(*pathIter);

   this->AddDirectory(path);

   ++pathIter;
  }
 }

 /**
  *
  */
 FileSystem::~FileSystem(void)
 {
  m_Directories.clear();
 }

 /**
  *
  */
 String FileSystem::ToString(void)
 {
  return FileSystemName;
 }

 /**
  *
  */
 ICore *FileSystem::GetCore(void)
 {
  return m_Core;
 }

 /**
  *
  */
 void FileSystem::AddDirectory(String name)
 {

  String realname = m_Core->GetParser()->Parse(name);
  size_t splitter = realname.find(';');

  while (splitter != String::npos)
  {

   String partname = realname.substr(0, splitter);

   this->m_Directories.push_front(partname);
   realname = realname.substr(splitter + 1);
   splitter = realname.find(';');
  }

  this->m_Directories.push_front(realname);
 }

 /**
  *
  */
 void FileSystem::RemoveDirectory(String name)
 {

  String realname = m_Core->GetParser()->Parse(name) + "\\";

  this->m_Directories.remove_if([&](String current)
 {
  return (current == realname);
 }
  );
 }

 /**
  *
  */
 IFile* FileSystem::FindFile(String name)
 {
  m_Core->GetLogger()->Log(LL_Debug, VOODOO_FILESYSTEM_NAME, "Searching for raw file \"%s\".", name.c_str());

  String filename = m_Core->GetParser()->Parse(name);

  m_Core->GetLogger()->Log
   (
    LL_Debug,
    VOODOO_FILESYSTEM_NAME,
    "Searching for parsed file \"%s\".",
    filename.c_str()
   );

  StringList::iterator curDir = m_Directories.begin();

  while (curDir != m_Directories.end())
  {


   // Try to find the file in each registered dir
   String fullname = (*curDir) + ("\\" + filename);
   HANDLE file = CreateFileA(fullname.c_str(), 0, 0, nullptr, OPEN_EXISTING, nullptr, nullptr);

   if (file != INVALID_HANDLE_VALUE)
   {
    CloseHandle(file);

    m_Core->GetLogger()->Log
     (
      LL_Debug,
      VOODOO_CORE_NAME,
      "File %s found in directory %s.",
      name.c_str(),
      (*curDir).c_str()
     );

    return IFile*(new File(m_Core, fullname));
   }

   ++curDir;
  }

  m_Core->GetLogger()->Log(LL_Warning, VOODOO_CORE_NAME, "Unable to find file %s.", name.c_str());

  return IFile*();
 }

 /**
  *
  */
 File::File(ICore *core, String name) :
  m_Name(name),
  m_Core(core),
  m_Handle(nullptr)
 { };

 /**
  *
  */
 File::~File(void)
 {
  this->Close();
 }

 /**
  *
  */
 String File::ToString(void)
 {
  return "WFile";
 }

 /**
  *
  */
 ICore *File::GetCore(void)
 {
  return m_Core;
 }

 /**
  *
  */
 String File::GetPath(void)
 {
  return m_Name;
 }

 /**
  *
  */
 bool File::Open(FileOpenMode mode)
 {

  DWORD access = 0;

  if (mode & FM_Read)
  {
   access |= GENERIC_READ;
  }

  if (mode & FM_Write)
  {
   access |= GENERIC_WRITE;
  }

  if (access == 0)
  {
   Throw(VOODOO_FILESYSTEM_NAME, "Attempted to open file with unknown mode.", m_Core);
  }

  m_Core->GetLogger()->Log
   (
    LL_Debug,
    VOODOO_FILESYSTEM_NAME,
    "Opening file %s with mode %u (underlying %u).",
    m_Name.c_str(),
    mode,
    access
   );

  m_Handle = CreateFileA(m_Name.c_str(), access, 0, nullptr, OPEN_EXISTING, nullptr, nullptr);

  if (m_Handle == INVALID_HANDLE_VALUE)
  {
   m_Core->GetLogger()->Log(LL_Warning, VOODOO_FILESYSTEM_NAME, "Unable to open file %s.", m_Name.c_str());
  }

  return (m_Handle != INVALID_HANDLE_VALUE);
 }

 /**
  *
  */
 IImage* File::OpenImage(void)
 {
  return Image::Load(m_Core, m_Name);
 }

 /**
  *
  */
 bool File::Close(void)
 {
  if (m_Handle && m_Handle != INVALID_HANDLE_VALUE)
  {

   BOOL retval = CloseHandle(m_Handle);

   return (retval == TRUE);
  }
  else
  {
   return false;
  }
 }

 /**
  *
  */
 int File::Read(_In_ int count, _In_opt_count_(count) void *buffer)
 {
  if (m_Handle)
  {

   DWORD readsize;

   if (count < 0)
   {
    readsize = GetFileSize(m_Handle, nullptr);

    if (buffer == nullptr)
    {
     return (int) readsize;
    }
   }
   else
   {
    readsize = (DWORD) count;
   }

   DWORD retsize = 0;
   BOOL success = ReadFile(m_Handle, buffer, readsize, &retsize, nullptr);

   if (success == 0)
   {
    return 0;
   }
   else
   {
    return (int) retsize;
   }
  }

  return 0;
 }

 /**
  *
  */
 bool File::Write(_In_ int count, _In_opt_count_(count) void *buffer)
 {
  if (m_Handle)
  {
   if (buffer == nullptr)
   {

    DWORD size = 0;

    if (count < 0)
    {
     Throw(VOODOO_FILESYSTEM_NAME, "Unable to write a negative number of nullptr bytes.", m_Core);
    }
    else
    {
     size = (DWORD) count;
    }

    buffer = malloc(size);

    if (buffer == nullptr)
    {
     Throw(VOODOO_FILESYSTEM_NAME, "Error allocating memory for nullptr buffer.", m_Core);
    }

    memset(buffer, 0, size);

    DWORD retsize = 0;
    BOOL success = WriteFile(m_Handle, buffer, size, &retsize, nullptr);

    free(buffer);

    if (success == 0)
    {
     return false;
    }
    else if (retsize != size)
    {
     return false;
    }
    else
    {
     return true;
    }
   }
   else
   {
    if (count < 0)
    {
     Throw(VOODOO_FILESYSTEM_NAME, "Unable to write a negative nubber of bytes.", m_Core);
    }

    DWORD size = (DWORD) count;
    DWORD retsize = 0;
    BOOL success = 0; // WriteFile(m_Handle, buffer, size, &retsize,
              ///nullptr);
              ///

    //~~~~~~~~~~~~~~~~~~~~~~~~~
    if (success == 0)
    {
     return false;
    }
    else if (retsize != size)
    {
     return false;
    }
    else
    {
     return true;
    }
   }
  }

  return false;
 }

 /**
  *
  */
 IImage* Image::Load(Core *core, String name)
 {

  ILuint image = ilGenImage();

  ilBindImage(image);

  ILboolean loaded = ilLoadImage(name.c_str());

  if (loaded == IL_FALSE)
  {
   ilBindImage(0);
   ilDeleteImage(image);
   return IImage*();
  }
  else
  {
   return IImage*(new Image(core, name, image));
  }
 }

 /**
  *
  */
 Image::Image(ICore *core, String name, ILuint image) :
  m_Core(core),
  m_Name(name),
  m_Image(image)
 {
  m_Desc.Width = ilGetInteger(IL_IMAGE_WIDTH);
  m_Desc.Height = ilGetInteger(IL_IMAGE_HEIGHT);
  m_Desc.Depth = ilGetInteger(IL_IMAGE_DEPTH);
  m_Desc.Mipmaps = false;
  m_Desc.Format = TF_Unknown;

  ILint texFmt = ilGetInteger(IL_IMAGE_FORMAT);
  ILint texType = ilGetInteger(IL_IMAGE_TYPE);

  if (texFmt == IL_RGB)
  {
   if (texType == IL_BYTE || texType == IL_UNSIGNED_BYTE)
   {
    m_Desc.Format = TF_RGB8;
   }
  }
  else if (texFmt == IL_RGBA)
  {
   if (texType == IL_BYTE || texType == IL_UNSIGNED_BYTE)
   {
    m_Desc.Format = TF_RGBA8;
   }
   else if (texType == IL_FLOAT)
   {
    m_Desc.Format = TF_RGBA16F;
   }
   else if (texType == IL_DOUBLE)
   {
    m_Desc.Format = TF_RGBA32F;
   }
  }

  if (m_Desc.Format == TF_Unknown)
  {
   m_Core->GetLogger()->Log
    (
     LL_Warning,
     VOODOO_FILESYSTEM_NAME,
     "Unable to resolve format for image %s (%u).",
     name.c_str(),
     image
    );
  }
 }

 /**
  *
  */
 Image::~Image(void)
 {
  ilDeleteImage(m_Image);
 }

 /**
  *
  */
 String Image::ToString(void)
 {
  return "WDevilImage";
 }

 /**
  *
  */
 ICore *Image::GetCore(void)
 {
  return m_Core;
 }

 /**
  *
  */
 String Image::GetPath(void)
 {
  return m_Name;
 }

 /**
  *
  */
 TextureDesc Image::GetDesc(void)
 {
  return m_Desc;
 }

 /**
  *
  */
 size_t Image::GetData(TextureRegion desc, void *buffer)
 {

  ILint ilFmt = 0;
  ILint ilType = 0;


  // Convert TextureFormat to DevIL format
  switch (desc.Format)
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
   Throw(VOODOO_FILESYSTEM_NAME, "Invalid texture format for data get.", m_Core);
  }

  return ilCopyPixels
   (
    desc.OffX,
    desc.OffY,
    desc.OffZ,
    desc.Width,
    desc.Height,
    desc.Depth,
    ilFmt,
    ilType,
    buffer
   );
 }
}
}
