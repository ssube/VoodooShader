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
#pragma once
#include "Meta.hpp"

namespace VoodooShader
{

 /**
  ===================================================================================================================
  * @addtogroup VoodooCore @{ ;
  * Provides a clean framework for loading and unloading modules in core, as well as retrieving symbols and modules from
  * memory.
  ===================================================================================================================
  */
 class ModuleManager
 {

/**
 -----------------------------------------------------------------------------------------------------------------------
 *
 -----------------------------------------------------------------------------------------------------------------------
 */
public:
  ModuleManager(_In_ Core *core);

  virtual ~ModuleManager(void);

  /* Loads a set of modules given a path. @note Only loads DLLs from in the path. */
  virtual bool LoadPath(_In_ String path, _In_ String filter);

  virtual bool LoadFile(_In_ String file);

  /* Tests to see if a class exists in the list provided by all loaded modules. */
  virtual bool ClassExists(_In_ String name);

  /**
   ===============================================================================================================
   * Wraps ModuleManager::CreateClass to return a shared pointer of a specific type. The type <em>must</em> be derived
   * from IObject.
   ===============================================================================================================
   */
  template<class T>
  inline std::shared_ptr<T> CreateClass(_In_ String name)
  {

   /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
   /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
   IObject *object = this->CreateObject(name);
   /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
   std::shared_ptr<T> retval((T *) object);

   return retval;
  };

  /**
   * Create a new instance of the given class. This object will be created in a
   * dynamic module and a pointer given. It should be cast to the actual type before
   * being used. @param name The class name to create. @return New object or null if
   * the class wasn't found or couldn't be created.
   */
  _Check_return_ virtual IObject *CreateObject(_In_ String name);

  /**
   * Finds the address of a function in a module. The module must be loaded into the
   * process and export the symbol, otherwise this will return null. If the module
   * name does not have an extension, ".dll" will be appended during the lookup. It
   * is recommended you <em>do not</em> use an absolute path here. @param module The
   * module name to look up. @param name The function name to find. @return The
   * function's address if found, null otherwise.
   */
  _Check_return_ virtual void *FindFunction(_In_ String module, _In_ String name);

/**
 -----------------------------------------------------------------------------------------------------------------------
 *
 -----------------------------------------------------------------------------------------------------------------------
 */
private:
  Core *m_Core;
  ModuleMap m_Modules;
  ClassMap m_Classes;
 };

 /**
  ===================================================================================================================
  * Contains the handle to a loaded library and function pointers for creation and destruction.
  ===================================================================================================================
  */
 class Module
 {

/**
 -----------------------------------------------------------------------------------------------------------------------
 *
 -----------------------------------------------------------------------------------------------------------------------
 */
public:

  /**
   * Attempt to load a module and locate all necessary functions. This will only
   * succeed for modules meeting the @ref modulespec, otherwise the module will fail
   * to load. @param path The path to the module (may be relative or absolute).
   * @return A new Module object if the module loads successfully, null otherwise.
   * @note If an absolute path is used, the directory portion will be searched for
   * dependencies. The underlying load mechanism is <code>LoadLibraryEx(path, NULL,
   * LOAD_WITH_ALTERED_SEARCH_PATH)</code>
   */
  static ModuleRef Load(_In_ String path);

  Module(_In_ HMODULE hmodule);

  virtual ~Module(void);

  /**
   * Get the current version of this module. @return The version, including name and
   * debug attribute.
   */
  virtual Version ModuleVersion(void);

  /* Get the class count from this module. */
  virtual int ClassCount(void);

  virtual const char *ClassInfo(_In_ int number);

  virtual IObject *CreateClass(_In_ int number, _In_ Core *core);

/**
 -----------------------------------------------------------------------------------------------------------------------
 *
 -----------------------------------------------------------------------------------------------------------------------
 */
private:
  HMODULE m_Handle;
  Functions::VersionFunc m_ModuleVersion;
  Functions::CountFunc m_ClassCount;
  Functions::InfoFunc m_ClassInfo;
  Functions::CreateFunc m_ClassCreate;
 };

 /* @} */
}
