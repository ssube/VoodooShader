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

namespace VoodooSharp
{
    public class VoodooRegistry
    {
        // Singleton/meta objects
        private static VoodooRegistry m_Instance;
        private static char[] m_Delims = { ';' };

        // Registry data
        public List<Remote> Remotes { get; set; }
        public List<Package> Packages { get; set; }
        public List<Module> Modules { get; set; }
        public List<Class> Classes { get; set; }
        public List<Hook> Hooks { get; set; }
        public List<Default> Defaults { get; set; }

        public string Path { get; set; }
        public string Language { get; set; }
        public string BinPrefix { get; set; }

        public static string Errors { get; set; }

        private VoodooRegistry()
        {
        }

        public static VoodooRegistry Instance
        {
            get
            {
                if (m_Instance == null)
                {
                    m_Instance = new VoodooRegistry();
                    m_Instance.Update();
                }
                return m_Instance;
            }
        }

        public static char ArraysDelim 
        { 
            get 
            {
                return m_Delims[0]; 
            } 
        }

        #region Write Methods
        public void Write()
        {
            Errors = null;

            if (Registry.CurrentUser.OpenSubKey(@"Software\VoodooShader") != null)
            {
                Registry.CurrentUser.DeleteSubKeyTree(@"Software\VoodooShader");
            }

            RegistryKey root = Registry.CurrentUser.CreateSubKey(@"Software\VoodooShader", RegistryKeyPermissionCheck.ReadWriteSubTree);

            root.SetValue("Path", Path);
            root.SetValue("Language", Language);
            root.SetValue("BinPrefix", BinPrefix);

            Write(Remotes, root);
            Write(Packages, root);
            Write(Modules, root);
            Write(Classes, root);
            Write(Hooks, root);
            Write(Defaults, root);
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
                    Write(remote, remoteKey);
                    remoteKey.Close();
                }
                remoteRootKey.Close();
            }
            catch (Exception exc)
            {
                Errors += String.Format("Error writing remotes:\n{0}\n", exc.Message);
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
                Errors += String.Format("Error writing remote '{0}': {1}\n", remote.Name, exc.Message);
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
                    Write(package, packageKey);
                    packageKey.Close();
                }
                packageRootKey.Close();
            }
            catch (Exception exc)
            {
                Errors += String.Format("Error writing packages:\n{0}\n", exc.Message);
            }
        }

        public static void Write(Package package, RegistryKey packageKey)
        {
            try
            {
                packageKey.SetValue("Name", package.Name);
                packageKey.SetValue("Version", package.Version);
                packageKey.SetValue("HomeUri", package.HomeUri);
            }
            catch (Exception exc)
            {
                Errors += String.Format("Error writing package '{0}': {1}\n", package.PackId, exc.Message);
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
                    Write(module, moduleKey);
                    moduleKey.Close();
                }
                moduleRootKey.Close();
            }
            catch (Exception exc)
            {
                Errors += String.Format("Error writing modules:\n{0}\n" + exc.Message);
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
                Errors += String.Format("Error writing module '{0}': {1}\n", module.LibId, exc.Message);
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
                    Write(vclass, classKey);
                    classKey.Close();
                }
                classRootKey.Close();
            }
            catch (Exception exc)
            {
                Errors += String.Format("Error writing classes:\n{0}\n" + exc.Message);
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
                Errors += String.Format("Error writing class '{0}': {1}\n", vclass.ClassId, exc.Message);
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
                    Write(hook, hookKey);
                    hookKey.Close();
                }
                hookRootKey.Close();
            }
            catch (Exception exc)
            {
                Errors += String.Format("Error writing hooks:\n{0}\n" + exc.Message);
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
                Errors += String.Format("Error writing hook '{0}': {1}\n", hook.Name, exc.Message);
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
                    Write(vdefault, defaultKey);
                    defaultKey.Close();
                }
                defaultRootKey.Close();
            }
            catch (Exception exc)
            {
                Errors += String.Format("Error writing defaults:\n{0}\n", exc.Message);
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
                    sb.Append(package.ToString() + m_Delims[0]);
                }
                defaultKey.SetValue("Packages", sb.ToString());
            }
            catch (Exception exc)
            {
                Errors += String.Format("Error writing default '{0}': {1}\n", vdefault.DefId, exc.Message);
            }
        }
        #endregion
        #endregion

        #region Read Methods
        public void Update()
        {
            Errors = null;

            RegistryKey root = Registry.CurrentUser.OpenSubKey(@"Software\VoodooShader");
            String defaultRoot = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + @"\VoodooShader\";

            Path = root.GetValue("Path", defaultRoot) as String;
            Language = root.GetValue("Language", "en-US") as String;
            BinPrefix = root.GetValue("BinPrefix", String.Empty) as String;

            Remotes = ReadRemotes(root.OpenSubKey("Remotes")); if (Remotes == null) Remotes = new List<Remote>();
            Packages = ReadPackages(root.OpenSubKey("Packages")); if (Packages == null) Packages = new List<Package>();
            Modules = ReadModules(root.OpenSubKey("Modules")); if (Modules == null) Modules = new List<Module>();
            Classes = ReadClasses(root.OpenSubKey("Classes")); if (Classes == null) Classes = new List<Class>();
            Hooks = ReadHooks(root.OpenSubKey("Hooks")); if (Hooks == null) Hooks = new List<Hook>();
            Defaults = ReadDefaults(root.OpenSubKey("Defaults")); if (Defaults == null) Defaults = new List<Default>();
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
                    Remote remote = ReadRemote(remoteKey);
                    remotes.Add(remote);
                }

                return remotes;
            }
            catch (Exception exc)
            {
                Errors += String.Format("Error reading remotes:\n{0}\n", exc.Message);
                return null;
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
                Errors += String.Format("Error reading remote from '{0}': {1}\n", remoteKey.Name, exc.Message);
                return null;
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
                    Package package = ReadPackage(packageKey);
                    packages.Add(package);
                }
                return packages;
            }
            catch (Exception exc)
            {
                Errors += String.Format("Error reading packages:\n{0}\n", exc.Message);
                return null;
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
                package.HomeUri = packageKey.GetValue("HomeUri") as String;

                return package;
            }
            catch (Exception exc)
            {
                Errors += String.Format("Error reading package from '{0}': {1}.\n", packageKey.Name, exc.Message);
                return null;
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
                    Module module = ReadModule(moduleKey);
                    modules.Add(module);
                }
                return modules;
            }
            catch (Exception exc)
            {
                Errors += String.Format("Error reading modules:\n{0}\n", exc.Message);
                return null;
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
                Errors += String.Format("Error reading module from '{0}': {1}\n", moduleKey.Name, exc.Message);
                return null;
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
                    Class vclass = ReadClass(ClassKey);
                    Classes.Add(vclass);
                }
                return Classes;
            }
            catch (Exception exc)
            {
                Errors += String.Format("Error reading classes:\n{0}\n", exc.Message);
                return null;
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
                Errors += String.Format("Error reading class from '{0}': {1}\n", ClassKey.Name, exc.Message);
                return null;
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
                    Hook vHook = ReadHook(HookKey);
                    Hooks.Add(vHook);
                }
                return Hooks;
            }
            catch (Exception exc)
            {
                Errors += String.Format("Error reading hooks:\n{0}\n", exc.Message);
                return null;
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
                Errors += String.Format("Error reading hook from '{0}': {1}\n", HookKey.Name, exc.Message);
                return null;
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
                    Default vDefault = ReadDefault(DefaultKey);
                    Defaults.Add(vDefault);
                }
                return Defaults;
            }
            catch (Exception exc)
            {
                Errors += String.Format("Error reading defaults:\n{0}\n", exc.Message);
                return null;
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

                String[] packagesStr = (DefaultKey.GetValue("Packages") as String).Split(m_Delims, StringSplitOptions.RemoveEmptyEntries);
                vdefault.Packages = new Guid[packagesStr.Length];
                for (int i = 0; i < vdefault.Packages.Length; ++i)
                {
                    vdefault.Packages[i] = new Guid(packagesStr[i]);
                }

                return vdefault;
            }
            catch (Exception exc)
            {
                Errors += String.Format("Error reading default from '{0}': {1}\n", DefaultKey.Name, exc.Message);
                return null;
            }
        }
        #endregion
        #endregion

        #region Xml Methods
        public RegistryChunk Export()
        {
            RegistryChunk chunk = new RegistryChunk();

            chunk.BinPrefix = BinPrefix;
            chunk.Language = Language;
            chunk.Path = Path;

            chunk.Classes = Classes.ToArray();
            chunk.Defaults = Defaults.ToArray();
            chunk.Hooks = Hooks.ToArray();
            chunk.Modules = Modules.ToArray();
            chunk.Packages = Packages.ToArray();
            chunk.Remotes = Remotes.ToArray();

            return chunk;
        }
        public void Import(RegistryChunk chunk)
        {
            if (chunk.BinPrefix != null) BinPrefix = chunk.BinPrefix;
            if (chunk.Language != null) Language = chunk.Language;
            if (chunk.Path != null) Path = chunk.Path;

            if (chunk.Classes != null) Classes.AddRange(chunk.Classes);
            if (chunk.Defaults != null) Defaults.AddRange(chunk.Defaults);
            if (chunk.Hooks != null) Hooks.AddRange(chunk.Hooks);
            if (chunk.Modules != null) Modules.AddRange(chunk.Modules);
            if (chunk.Packages != null) Packages.AddRange(chunk.Packages);
            if (chunk.Remotes != null) Remotes.AddRange(chunk.Remotes);
        }
        #endregion

        String PackageVersion(String PackageName)
        {
            Package fp = Packages.Find(p => p.Name == PackageName);
            if (fp != null)
            {
                return fp.Version;
            }
            else
            {
                return null;
            }
        }
    }
}
