/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2011 by Sean Sube
 *
 * The Voodoo Shader Framework is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option)
 * any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this program; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 US
 *
 * Support and more information may be found at
 *   http://www.voodooshader.com
 * or by contacting the lead developer at
 *   peachykeen@voodooshader.com
 */
using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Win32;

namespace VoodooNetClasses
{
    public class VoodooRegistry
    {
        private static char[] ArrayDelims = { ';' };

        public static char ArraysDelim { get { return ArrayDelims[0]; } }

        public static String GetRootPath()
        {
            String defaultRoot = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + @"\VoodooShader\";

            try
            {
                String regPath = Registry.CurrentUser.GetValue(@"Software\VoodooShader\Path") as String;

                if (regPath == null)
                {
                    Registry.CurrentUser.SetValue(@"Software\VoodooShader\Path", defaultRoot);
                    return defaultRoot;
                }
                else
                {
                    return regPath;
                }
            }
            catch (Exception)
            {
                return defaultRoot;
            }
        }

        #region Write Methods
        public static void Write(GlobalRegistry full)
        {
            if (Registry.CurrentUser.OpenSubKey(@"Software\VoodooShader") != null)
            {
                Registry.CurrentUser.DeleteSubKeyTree(@"Software\VoodooShader");
            }

            RegistryKey root = Registry.CurrentUser.CreateSubKey(@"Software\VoodooShader", RegistryKeyPermissionCheck.ReadWriteSubTree);

            root.SetValue("Path", full.Path);
            root.SetValue("Language", full.Language);
            root.SetValue("BinPrefix", full.BinPrefix);

            VoodooRegistry.Write(full.Remotes, root);
            VoodooRegistry.Write(full.Packages, root);
            VoodooRegistry.Write(full.Modules, root);
            VoodooRegistry.Write(full.Classes, root);
            VoodooRegistry.Write(full.Hooks, root);
            VoodooRegistry.Write(full.Defaults, root);

            root.Close();
        }
        #region Remotes
        public static void Write(IEnumerable<Remote> remotes, RegistryKey root)
        {
            try
            {
                RegistryKey remoteRootKey = root.CreateSubKey("Remotes", RegistryKeyPermissionCheck.ReadWriteSubTree);
                foreach (Remote remote in remotes)
                {
                    RegistryKey remoteKey = remoteRootKey.CreateSubKey(VoodooHash.Hash(remote.Uri), RegistryKeyPermissionCheck.ReadWriteSubTree);
                    VoodooRegistry.Write(remote, remoteKey);
                    remoteKey.Close();
                }
                remoteRootKey.Close();
            }
            catch (Exception exc)
            {
                throw new Exception("Error writing remotes: " + exc.Message, exc);
            }
        }

        public static void Write(Remote remote, RegistryKey remoteKey)
        {
            try
            {
                remoteKey.SetValue("Name", remote.Name);
                remoteKey.SetValue("Uri", remote.Uri);
            }
            catch (Exception exc)
            {
                throw new Exception(String.Format("Error writing remote '{0}'.", remote.Name), exc);
            }
        }
        #endregion
        #region Packages
        public static void Write(IEnumerable<Package> packages, RegistryKey root)
        {
            try
            {
                RegistryKey packageRootKey = root.CreateSubKey("Packages", RegistryKeyPermissionCheck.ReadWriteSubTree);
                foreach (Package package in packages)
                {
                    RegistryKey packageKey = packageRootKey.CreateSubKey(package.PackId.ToString(), RegistryKeyPermissionCheck.ReadWriteSubTree);
                    VoodooRegistry.Write(package, packageKey);
                    packageKey.Close();
                }
                packageRootKey.Close();
            }
            catch (Exception exc)
            {
                throw new Exception("Error writing packages: " + exc.Message, exc);
            }
        }

        public static void Write(Package package, RegistryKey packageKey)
        {
            try
            {
                packageKey.SetValue("Name", package.Name);
                packageKey.SetValue("Version", package.Version);
                packageKey.SetValue("ManifestUri", package.ManifestUri);
            }
            catch (Exception exc)
            {
                throw new Exception(String.Format("Error writing package '{0}'.", package.PackId), exc);
            }
        }
        #endregion
        #region Modules
        public static void Write(IEnumerable<Module> modules, RegistryKey root)
        {
            try
            {
                RegistryKey moduleRootKey = root.CreateSubKey("Modules", RegistryKeyPermissionCheck.ReadWriteSubTree);
                foreach (Module module in modules)
                {
                    RegistryKey moduleKey = moduleRootKey.CreateSubKey(module.LibId.ToString(), RegistryKeyPermissionCheck.ReadWriteSubTree);
                    VoodooRegistry.Write(module, moduleKey);
                    moduleKey.Close();
                }
                moduleRootKey.Close();
            }
            catch (Exception exc)
            {
                throw new Exception("Error writing modules: " + exc.Message, exc);
            }
        }

        public static void Write(Module module, RegistryKey moduleKey)
        {
            try
            {
                moduleKey.SetValue("Name", module.Name);
                moduleKey.SetValue("Path", module.Path);
                moduleKey.SetValue("Config", module.Config);
                moduleKey.SetValue("Package", module.Package);
            }
            catch (Exception exc)
            {
                throw new Exception(String.Format("Error writing module '{0}'.", module.LibId), exc);
            }
        }
        #endregion
        #region Classes
        public static void Write(IEnumerable<Class> classes, RegistryKey root)
        {
            try
            {
                RegistryKey classRootKey = root.CreateSubKey("Classes", RegistryKeyPermissionCheck.ReadWriteSubTree);
                foreach (Class vclass in classes)
                {
                    RegistryKey classKey = classRootKey.CreateSubKey(vclass.ClassId.ToString(), RegistryKeyPermissionCheck.ReadWriteSubTree);
                    VoodooRegistry.Write(vclass, classKey);
                    classKey.Close();
                }
                classRootKey.Close();
            }
            catch (Exception exc)
            {
                throw new Exception("Error writing classes: " + exc.Message, exc);
            }
        }

        public static void Write(Class vclass, RegistryKey classKey)
        {
            try
            {
                classKey.SetValue("Name", vclass.Name);
                classKey.SetValue("Module", vclass.Module);
            }
            catch (Exception exc)
            {
                throw new Exception(String.Format("Error writing class '{0}'.", vclass.ClassId), exc);
            }
        }
        #endregion
        #region Hooks
        public static void Write(IEnumerable<Hook> hooks, RegistryKey root)
        {
            try
            {
                RegistryKey hookRootKey = root.CreateSubKey("Hooks", RegistryKeyPermissionCheck.ReadWriteSubTree);
                UInt32 counter = 0;
                foreach (Hook hook in hooks)
                {
                    RegistryKey hookKey = hookRootKey.CreateSubKey(counter++.ToString(), RegistryKeyPermissionCheck.ReadWriteSubTree);
                    VoodooRegistry.Write(hook, hookKey);
                    hookKey.Close();
                }
                hookRootKey.Close();
            }
            catch (Exception exc)
            {
                throw new Exception("Error writing hooks: " + exc.Message, exc);
            }
        }

        public static void Write(Hook hook, RegistryKey hookKey)
        {
            try
            {
                hookKey.SetValue("Active", hook.Active.ToString().ToLower());
                hookKey.SetValue("Name", hook.Name);
                hookKey.SetValue("Target", hook.Target);
                hookKey.SetValue("Config", hook.Config);
            }
            catch (Exception exc)
            {
                throw new Exception(String.Format("Error writing hook '{0}'.", hook.Name), exc);
            }
        }
        #endregion
        #region Defaults
        public static void Write(IEnumerable<Default> defaults, RegistryKey root)
        {
            try
            {
                RegistryKey defaultRootKey = root.CreateSubKey("Defaults", RegistryKeyPermissionCheck.ReadWriteSubTree);
                foreach (Default vdefault in defaults)
                {
                    RegistryKey defaultKey = defaultRootKey.CreateSubKey(vdefault.DefId.ToString(), RegistryKeyPermissionCheck.ReadWriteSubTree);
                    VoodooRegistry.Write(vdefault, defaultKey);
                    defaultKey.Close();
                }
                defaultRootKey.Close();
            }
            catch (Exception exc)
            {
                throw new Exception("Error writing defaults: " + exc.Message, exc);
            }
        }

        public static void Write(Default vdefault, RegistryKey defaultKey)
        {
            try
            {
                defaultKey.SetValue("Name", vdefault.Name);
                defaultKey.SetValue("Filter", vdefault.Filter);
                defaultKey.SetValue("Config", vdefault.Config);

                StringBuilder sb = new StringBuilder();
                foreach (Guid package in vdefault.Packages)
                {
                    sb.Append(package.ToString() + ArrayDelims[0]);
                }
                defaultKey.SetValue("Packages", sb.ToString());
            }
            catch (Exception exc)
            {
                throw new Exception(String.Format("Error writing default '{0}'.", vdefault.DefId), exc);
            }
        }
        #endregion
        #endregion

        #region Read Methods
        public static GlobalRegistry Read()
        {
            GlobalRegistry full = new GlobalRegistry();

            RegistryKey root = Registry.CurrentUser.OpenSubKey(@"Software\VoodooShader");

            full.Path = root.GetValue("Path") as String;
            full.Language = root.GetValue("Language") as String;
            full.BinPrefix = root.GetValue("BinPrefix") as String;

            full.Remotes = VoodooRegistry.ReadRemotes(root.OpenSubKey("Remotes")).ToArray();
            full.Packages = VoodooRegistry.ReadPackages(root.OpenSubKey("Packages")).ToArray();
            full.Modules = VoodooRegistry.ReadModules(root.OpenSubKey("Modules")).ToArray();
            full.Classes = VoodooRegistry.ReadClasses(root.OpenSubKey("Classes")).ToArray();
            full.Hooks = VoodooRegistry.ReadHooks(root.OpenSubKey("Hooks")).ToArray();
            full.Defaults = VoodooRegistry.ReadDefaults(root.OpenSubKey("Defaults")).ToArray();

            return full;
        }
        #region Remotes
        public static List<Remote> ReadRemotes(RegistryKey root)
        {
            try
            {
                List<Remote> remotes = new List<Remote>(root.SubKeyCount);
                foreach (String remoteKeyName in root.GetSubKeyNames())
                {
                    RegistryKey remoteKey = root.OpenSubKey(remoteKeyName);
                    Remote remote = VoodooRegistry.ReadRemote(remoteKey);
                    remotes.Add(remote);
                }
                return remotes;
            }
            catch (Exception exc)
            {
                throw new Exception("Error reading remotes: " + exc.Message, exc);
            }
        }

        public static Remote ReadRemote(RegistryKey remoteKey)
        {
            try
            {
                Remote remote = new Remote();

                remote.Name = remoteKey.GetValue("Name") as String;
                remote.Uri = remoteKey.GetValue("Uri") as String;

                return remote;
            }
            catch (Exception exc)
            {
                throw new Exception(String.Format("Error reading remote from '{0}'.", remoteKey.Name), exc);
            }
        }
        #endregion
        #region Packages
        public static List<Package> ReadPackages(RegistryKey root)
        {
            try
            {
                List<Package> packages = new List<Package>(root.SubKeyCount);
                foreach (String packageKeyName in root.GetSubKeyNames())
                {
                    RegistryKey packageKey = root.OpenSubKey(packageKeyName);
                    Package package = VoodooRegistry.ReadPackage(packageKey);
                    packages.Add(package);
                }
                return packages;
            }
            catch (Exception exc)
            {
                throw new Exception("Error reading packages: " + exc.Message, exc);
            }
        }

        public static Package ReadPackage(RegistryKey packageKey)
        {
            try
            {
                Package package = new Package();

                String id = packageKey.Name.Substring(packageKey.Name.LastIndexOf('\\') + 1);
                package.PackId = new Guid(id);
                package.Name = packageKey.GetValue("Name") as String;
                package.Version = packageKey.GetValue("Version") as String;
                package.ManifestUri = packageKey.GetValue("ManifestUri") as String;

                return package;
            }
            catch (Exception exc)
            {
                throw new Exception(String.Format("Error reading package from '{0}'.", packageKey.Name), exc);
            }
        }
        #endregion
        #region Modules
        public static List<Module> ReadModules(RegistryKey root)
        {
            try
            {
                List<Module> modules = new List<Module>(root.SubKeyCount);
                foreach (String moduleKeyName in root.GetSubKeyNames())
                {
                    RegistryKey moduleKey = root.OpenSubKey(moduleKeyName);
                    Module module = VoodooRegistry.ReadModule(moduleKey);
                    modules.Add(module);
                }
                return modules;
            }
            catch (Exception exc)
            {
                throw new Exception("Error reading modules: " + exc.Message, exc);
            }
        }

        public static Module ReadModule(RegistryKey moduleKey)
        {
            try
            {
                Module module = new Module();

                String id = moduleKey.Name.Substring(moduleKey.Name.LastIndexOf('\\') + 1);
                module.LibId = new Guid(id);
                module.Name = moduleKey.GetValue("Name") as String;
                module.Path = moduleKey.GetValue("Path") as String;
                module.Config = moduleKey.GetValue("Config") as String;
                module.Package = new Guid(moduleKey.GetValue("Package") as String);

                return module;
            }
            catch (Exception exc)
            {
                throw new Exception(String.Format("Error reading module from '{0}'.", moduleKey.Name), exc);
            }
        }
        #endregion
        #region Classes
        public static List<Class> ReadClasses(RegistryKey root)
        {
            try
            {
                List<Class> Classes = new List<Class>(root.SubKeyCount);
                foreach (String ClassKeyName in root.GetSubKeyNames())
                {
                    RegistryKey ClassKey = root.OpenSubKey(ClassKeyName);
                    Class vclass = VoodooRegistry.ReadClass(ClassKey);
                    Classes.Add(vclass);
                }
                return Classes;
            }
            catch (Exception exc)
            {
                throw new Exception("Error reading classes: " + exc.Message, exc);
            }
        }

        public static Class ReadClass(RegistryKey ClassKey)
        {
            try
            {
                Class vclass = new Class();

                String id = ClassKey.Name.Substring(ClassKey.Name.LastIndexOf('\\') + 1);
                vclass.ClassId = new Guid(id);
                vclass.Name = ClassKey.GetValue("Name") as String;
                vclass.Module = new Guid(ClassKey.GetValue("Module") as String);

                return vclass;
            }
            catch (Exception exc)
            {
                throw new Exception(String.Format("Error reading class from '{0}'.", ClassKey.Name), exc);
            }
        }
        #endregion
        #region Hooks
        public static List<Hook> ReadHooks(RegistryKey root)
        {
            try
            {
                List<Hook> Hooks = new List<Hook>(root.SubKeyCount);
                foreach (String HookKeyName in root.GetSubKeyNames())
                {
                    RegistryKey HookKey = root.OpenSubKey(HookKeyName);
                    Hook vHook = VoodooRegistry.ReadHook(HookKey);
                    Hooks.Add(vHook);
                }
                return Hooks;
            }
            catch (Exception exc)
            {
                throw new Exception("Error reading hooks: " + exc.Message, exc);
            }
        }

        public static Hook ReadHook(RegistryKey HookKey)
        {
            try
            {
                Hook vHook = new Hook();

                vHook.Active = Convert.ToBoolean(HookKey.GetValue("Active"));
                vHook.Name = HookKey.GetValue("Name") as String;
                vHook.Target = HookKey.GetValue("Target") as String;
                vHook.Config = HookKey.GetValue("Config") as String;

                return vHook;
            }
            catch (Exception exc)
            {
                throw new Exception(String.Format("Error reading hook from '{0}'.", HookKey.Name), exc);
            }
        }
        #endregion
        #region Defaults
        public static List<Default> ReadDefaults(RegistryKey root)
        {
            try
            {
                List<Default> Defaults = new List<Default>(root.SubKeyCount);
                foreach (String DefaultKeyName in root.GetSubKeyNames())
                {
                    RegistryKey DefaultKey = root.OpenSubKey(DefaultKeyName);
                    Default vDefault = VoodooRegistry.ReadDefault(DefaultKey);
                    Defaults.Add(vDefault);
                }
                return Defaults;
            }
            catch (Exception exc)
            {
                throw new Exception("Error reading defaults: " + exc.Message, exc);
            }
        }

        public static Default ReadDefault(RegistryKey DefaultKey)
        {
            try
            {
                Default vdefault = new Default();

                String id = DefaultKey.Name.Substring(DefaultKey.Name.LastIndexOf('\\') + 1);
                vdefault.DefId = new Guid(id);
                vdefault.Name = DefaultKey.GetValue("Name") as String;
                vdefault.Filter = DefaultKey.GetValue("Filter") as String;
                vdefault.Config = DefaultKey.GetValue("Config") as String;

                String[] packagesStr = (DefaultKey.GetValue("Packages") as String).Split(ArrayDelims, StringSplitOptions.RemoveEmptyEntries);
                vdefault.Packages = new Guid[packagesStr.Length];
                for (int i = 0; i < vdefault.Packages.Length; ++i)
                {
                    vdefault.Packages[i] = new Guid(packagesStr[i]);
                }

                return vdefault;
            }
            catch (Exception exc)
            {
                throw new Exception(String.Format("Error reading default from '{0}'.", DefaultKey.Name), exc);
            }
        }
        #endregion
        #endregion
    }
}
