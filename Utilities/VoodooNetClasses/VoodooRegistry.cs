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
    class VoodooRegistry
    {
        private static char[] ArrayDelims = { ';' };

        #region Write Methods
        static void Write(GlobalRegistry full)
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
        static void Write(Remote[] remotes, RegistryKey root)
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

        static void Write(Remote remote, RegistryKey remoteKey)
        {
            remoteKey.SetValue("Name", remote.Name);
            remoteKey.SetValue("Uri", remote.Uri);
        }
        #endregion

        #region Packages
        static void Write(Package[] packages, RegistryKey root)
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

        static void Write(Package package, RegistryKey packageKey)
        {
            packageKey.SetValue("Name", package.Name);
            packageKey.SetValue("Version", package.Version);
            packageKey.SetValue("ManifestUri", package.ManifestUri);
        }
        #endregion

        #region Modules
        static void Write(Module[] modules, RegistryKey root)
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

        static void Write(Module module, RegistryKey moduleKey)
        {
            moduleKey.SetValue("Name", module.Name);
            moduleKey.SetValue("Path", module.Path);
            moduleKey.SetValue("Config", module.Config);
            moduleKey.SetValue("Package", module.Package);
        }
        #endregion

        #region Classes
        static void Write(Class[] classes, RegistryKey root)
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

        static void Write(Class vclass, RegistryKey classKey)
        {
            classKey.SetValue("Name", vclass.Name);
            classKey.SetValue("Module", vclass.Module);
        }
        #endregion

        #region Hooks
        static void Write(Hook[] hooks, RegistryKey root)
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

        static void Write(Hook hook, RegistryKey hookKey)
        {
            hookKey.SetValue("Name", hook.Name);
            hookKey.SetValue("Target", hook.Target);
            hookKey.SetValue("Config", hook.Config);
        }
        #endregion

        #region Defaults
        static void Write(Default[] defaults, RegistryKey root)
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

        static void Write(Default vdefault, RegistryKey defaultKey)
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
        #endregion
        #endregion

        #region Read Methods
        static GlobalRegistry Read()
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
        static List<Remote> ReadRemotes(RegistryKey root)
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

        static Remote ReadRemote(RegistryKey remoteKey)
        {
            Remote remote = new Remote();

            remote.Name = remoteKey.GetValue("Name") as String;
            remote.Uri = remoteKey.GetValue("Uri") as String;

            return remote;
        }
        #endregion
        #region Packages
        static List<Package> ReadPackages(RegistryKey root)
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

        static Package ReadPackage(RegistryKey packageKey)
        {
            Package package = new Package();

            String id = packageKey.Name.Substring(packageKey.Name.LastIndexOf('\\') + 1);
            package.PackId = new Guid(id);
            package.Name = packageKey.GetValue("Name") as String;
            package.Version = packageKey.GetValue("Version") as String;
            package.ManifestUri = packageKey.GetValue("ManifestUri") as String;

            return package;
        }
        #endregion
        #region Modules
        static List<Module> ReadModules(RegistryKey root)
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

        static Module ReadModule(RegistryKey moduleKey)
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
        #endregion
        #region Classes
        static List<Class> ReadClasses(RegistryKey root)
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

        static Class ReadClass(RegistryKey ClassKey)
        {
            Class vclass = new Class();

            String id = ClassKey.Name.Substring(ClassKey.Name.LastIndexOf('\\') + 1);
            vclass.ClassId = new Guid(id);
            vclass.Name = ClassKey.GetValue("Name") as String;
            vclass.Module = new Guid(ClassKey.GetValue("Module") as String);

            return vclass;
        }
        #endregion
        #region Hooks
        static List<Hook> ReadHooks(RegistryKey root)
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

        static Hook ReadHook(RegistryKey HookKey)
        {
            Hook vHook = new Hook();

            vHook.Active = Convert.ToBoolean(HookKey.GetValue("Active"));
            vHook.Name = HookKey.GetValue("Name") as String;
            vHook.Target = HookKey.GetValue("Target") as String;
            vHook.Config = HookKey.GetValue("Config") as String;

            return vHook;
        }
        #endregion
        #region Defaults
        static List<Default> ReadDefaults(RegistryKey root)
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

        static Default ReadDefault(RegistryKey DefaultKey)
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
        #endregion
        #endregion
    }
}
