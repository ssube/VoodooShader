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

            RegistryKey remoteRootKey = root.CreateSubKey("Remotes", RegistryKeyPermissionCheck.ReadWriteSubTree);
            foreach (Remote remote in full.Remotes)
            {
                RegistryKey remoteKey = remoteRootKey.CreateSubKey(VoodooHash.Hash(remote.Uri), RegistryKeyPermissionCheck.ReadWriteSubTree);
                VoodooRegistry.Write(remote, remoteKey);
            }
            
            RegistryKey packageRootKey = root.CreateSubKey("Packages", RegistryKeyPermissionCheck.ReadWriteSubTree);
            foreach (Package package in full.Packages)
            {
                RegistryKey packageKey = packageRootKey.CreateSubKey(package.PackId.ToString(), RegistryKeyPermissionCheck.ReadWriteSubTree);
                VoodooRegistry.Write(package, packageKey);
            }

            RegistryKey moduleRootKey = root.CreateSubKey("Modules", RegistryKeyPermissionCheck.ReadWriteSubTree);
            foreach (Module module in full.Modules)
            {
                RegistryKey moduleKey = moduleRootKey.CreateSubKey(module.LibId.ToString(), RegistryKeyPermissionCheck.ReadWriteSubTree);
                VoodooRegistry.Write(module, moduleKey);
            }

            RegistryKey classRootKey = root.CreateSubKey("Classes", RegistryKeyPermissionCheck.ReadWriteSubTree);
            foreach (Class vclass in full.Classes)
            {
                RegistryKey classKey = classRootKey.CreateSubKey(vclass.ClassId.ToString(), RegistryKeyPermissionCheck.ReadWriteSubTree);
                VoodooRegistry.Write(vclass, classKey);
            }

            RegistryKey hookRootKey = root.CreateSubKey("Hooks", RegistryKeyPermissionCheck.ReadWriteSubTree);
            UInt32 counter = 0;
            foreach (Hook hook in full.Hooks)
            {
                RegistryKey hookKey = hookRootKey.CreateSubKey(counter++.ToString(), RegistryKeyPermissionCheck.ReadWriteSubTree);
                VoodooRegistry.Write(hook, hookKey);
            }

            RegistryKey defaultRootKey = root.CreateSubKey("Defaults", RegistryKeyPermissionCheck.ReadWriteSubTree);
            foreach (Default vdefault in full.Defaults)
            {
                RegistryKey defaultKey = defaultRootKey.CreateSubKey(vdefault.DefId.ToString(), RegistryKeyPermissionCheck.ReadWriteSubTree);
                VoodooRegistry.Write(vdefault, defaultKey);
            }
        }

        static void Write(Remote remote, RegistryKey remoteKey)
        {
            remoteKey.SetValue("Name", remote.Name);
            remoteKey.SetValue("Uri", remote.Uri);
        }

        static void Write(Package package, RegistryKey packageKey)
        {
            packageKey.SetValue("Name", package.Name);
            packageKey.SetValue("Version", package.Version);
            packageKey.SetValue("ManifestUri", package.ManifestUri);
        }

        static void Write(Module module, RegistryKey moduleKey)
        {
            moduleKey.SetValue("Name", module.Name);
            moduleKey.SetValue("Path", module.Path);
            moduleKey.SetValue("Config", module.Config);
            moduleKey.SetValue("Package", module.Package);
        }

        static void Write(Class vclass, RegistryKey classKey)
        {
            classKey.SetValue("Name", vclass.Name);
            classKey.SetValue("Module", vclass.Module);
        }

        static void Write(Hook hook, RegistryKey hookKey)
        {
            hookKey.SetValue("Name", hook.Name);
            hookKey.SetValue("Target", hook.Target);
            hookKey.SetValue("Config", hook.Config);
        }

        static void Write(Default vdefault, RegistryKey defaultKey)
        {
            defaultKey.SetValue("Name", vdefault.Name);
            defaultKey.SetValue("Filter", vdefault.Filter);
            defaultKey.SetValue("Config", vdefault.Config);

            StringBuilder sb = new StringBuilder();
            foreach (Guid package in vdefault.Packages)
            {
                sb.Append(package.ToString() + ";");
            }
            defaultKey.SetValue("Packages", sb.ToString());
        }
    }
}
