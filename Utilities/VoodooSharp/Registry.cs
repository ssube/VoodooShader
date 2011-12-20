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
using System.Xml.Serialization;
using Microsoft.Win32;

namespace VoodooSharp
{
    [System.SerializableAttribute()]
    [System.Xml.Serialization.XmlRootAttribute("VoodooRegistry", Namespace = "", IsNullable = false)]
    public partial class RegistryShard
    {
        [System.Xml.Serialization.XmlArrayItemAttribute(IsNullable = false)]
        public List<Hook> Hooks { get; set; }

        [System.Xml.Serialization.XmlAttributeAttribute()]
        public string Path { get; set; }

        [System.Xml.Serialization.XmlAttributeAttribute(DataType = "language")]
        public string Language { get; set; }

        [System.Xml.Serialization.XmlAttributeAttribute()]
        public string BinPrefix { get; set; }
    }

    public class GlobalRegistry
    {
        // Singleton/meta objects
        private static GlobalRegistry m_Instance;
        private static char[] m_Delims = { ';' };

        // Registry data
        public List<Hook> Hooks { get; set; }

        public string Path { get; set; }
        public string Language { get; set; }
        public string BinPrefix { get; set; }
        public string DefaultRoot { get; set; }

        public string BinPath
        {
            get
            {
                return System.IO.Path.Combine(Path, BinPrefix);
            }
        }

        public static string Errors { get; set; }

        public static bool Exists
        {
            get
            {
                RegistryKey root = Microsoft.Win32.Registry.CurrentUser.OpenSubKey(@"Software\VoodooShader");
                return (root != null);
            }
        }

        private GlobalRegistry()
        {
            DefaultRoot = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments) + @"\VoodooShader\";
            Path = DefaultRoot;
            Language = "en-US";
            BinPrefix = "bin\\";

            Hooks = new List<Hook>();
        }

        public static GlobalRegistry Instance
        {
            get
            {
                if (m_Instance == null)
                {
                    m_Instance = new GlobalRegistry();
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
        
        #region Public Methods
        public Hook GetHook(String name)
        {
            return Hooks.Find(m => m.Name == name);
        }
        public void SetHook(Hook hook)
        {
            Hooks.RemoveAll(h => h.Name == hook.Name);
            Hooks.Add(hook);
        }
        public int RemoveHook(String name)
        {
            return Hooks.RemoveAll(h => h.Name == name);
        }
        #endregion

        #region Write Methods
        public void Write()
        {
            Errors = null;

            if (Microsoft.Win32.Registry.CurrentUser.OpenSubKey(@"Software\VoodooShader") != null)
            {
                Microsoft.Win32.Registry.CurrentUser.DeleteSubKeyTree(@"Software\VoodooShader");
            }

            RegistryKey root = Microsoft.Win32.Registry.CurrentUser.CreateSubKey(@"Software\VoodooShader", RegistryKeyPermissionCheck.ReadWriteSubTree);

            root.SetValue("Path", Path);
            root.SetValue("Language", Language);
            root.SetValue("BinPrefix", BinPrefix);

            Write(Hooks, root);
        }
        
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
        #endregion

        #region Read Methods
        public void Update()
        {
            Errors = null;

            RegistryKey root = Microsoft.Win32.Registry.CurrentUser.OpenSubKey(@"Software\VoodooShader");

            if (root == null)
            {
                Errors = "Root does not exist.";
                return;
            }

            Path = root.GetValue("Path", DefaultRoot) as String;
            Language = root.GetValue("Language", "en-US") as String;
            BinPrefix = root.GetValue("BinPrefix", "bin\\") as String;

            Hooks = ReadHooks(root.OpenSubKey("Hooks")); if (Hooks == null) Hooks = new List<Hook>();
        }
        
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
        #endregion

        #region Xml Methods
        public RegistryShard Export()
        {
            RegistryShard shard = new RegistryShard();

            shard.BinPrefix = BinPrefix;
            shard.Language = Language;
            shard.Path = Path;

            shard.Hooks = Hooks;

            return shard;
        }
        public void Import(RegistryShard shard)
        {
            if (shard.BinPrefix != null) BinPrefix = shard.BinPrefix;
            if (shard.Language != null) Language = shard.Language;
            if (shard.Path != null) Path = shard.Path;

            if (shard.Hooks != null) Hooks.AddRange(shard.Hooks);
        }
        #endregion
    }
}
