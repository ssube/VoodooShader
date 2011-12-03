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
using System.Windows.Forms;
using System.Runtime.Serialization;
using System.Security.Permissions;
using Microsoft.Win32;

namespace VoodooNetClasses
{
    public class VoodooRegistry : ISerializable, IVoodooRegistryObject
    {
        private String m_Path, m_BinPrefix;

        private List<VoodooRemote> m_Remotes;
        private List<VoodooPackage> m_Packages;
        private List<VoodooModule> m_Modules;
        private List<VoodooClass> m_Classes;
        private List<VoodooHook> m_Hooks;
        private List<VoodooDefault> m_Defaults;

        public VoodooRegistry()
        {
            m_Path = String.Empty;
            m_BinPrefix = String.Empty;

            m_Remotes = new List<VoodooRemote>();
            m_Packages = new List<VoodooPackage>();
            m_Modules = new List<VoodooModule>();
            m_Classes = new List<VoodooClass>();
            m_Hooks = new List<VoodooHook>();
            m_Defaults = new List<VoodooDefault>();
        }

        protected VoodooRegistry(SerializationInfo info, StreamingContext context)
        {
            m_Path = info.GetString("Path");
            m_BinPrefix = info.GetString("BinPrefix");

            m_Remotes = (List<VoodooRemote>)info.GetValue("Remotes", typeof(List<VoodooRemote>));
            m_Packages = (List<VoodooPackage>)info.GetValue("Packages", typeof(List<VoodooPackage>));
            m_Modules = (List<VoodooModule>)info.GetValue("Modules", typeof(List<VoodooModule>));
            m_Classes = (List<VoodooClass>)info.GetValue("Classes", typeof(List<VoodooClass>));
            m_Hooks = (List<VoodooHook>)info.GetValue("Hooks", typeof(List<VoodooHook>));
            m_Defaults = (List<VoodooDefault>)info.GetValue("Defaults", typeof(List<VoodooDefault>));
        }

        [SecurityPermissionAttribute(SecurityAction.Demand, SerializationFormatter = true)]
        public virtual void GetObjectData(SerializationInfo info, StreamingContext context)
        {
            info.AddValue("Path", m_Path);
            info.AddValue("BinPrefix", m_BinPrefix);

            info.AddValue("Remotes", m_Remotes);
            info.AddValue("Packages", m_Packages);
            info.AddValue("Modules", m_Modules);
            info.AddValue("Classes", m_Classes);
            info.AddValue("Hooks", m_Hooks);
            info.AddValue("Defaults", m_Defaults);
        }

        public void FromRegistryKey(RegistryKey key)
        {
            try
            {
                m_Path = key.GetValue("Path") as String;
                if (m_Path == null)
                {
                    m_Path = String.Empty;
                }
            }
            catch (System.Exception)
            {
                m_Path = String.Empty;	
            }
            try
            {
                m_BinPrefix = key.GetValue("BinPrefix") as String;
                if (m_BinPrefix == null)
                {
                    m_BinPrefix = String.Empty;
                }
            }
            catch (System.Exception)
            {
                m_BinPrefix = String.Empty;	            	
            }

            FromRegistryKey_GetList(key, "Remotes", ref m_Remotes);
            FromRegistryKey_GetList(key, "Packages", ref m_Packages);
            FromRegistryKey_GetList(key, "Modules", ref m_Modules);
            FromRegistryKey_GetList(key, "Classes", ref m_Classes);
            FromRegistryKey_GetList(key, "Hooks", ref m_Hooks);
            FromRegistryKey_GetList(key, "Defaults", ref m_Defaults);
        }

        private void FromRegistryKey_GetList<T>(RegistryKey key, String subkey, ref List<T> outlist) where T : IVoodooRegistryObject, new()
        {
            try
            {
                RegistryKey listRoot = key.OpenSubKey(subkey);
                foreach (String listItem in listRoot.GetSubKeyNames())
                {
                    T item = new T();
                    item.FromRegistryKey(listRoot.OpenSubKey(listItem));
                    outlist.Add(item);
                }
            }
            catch (System.Exception)
            {
            	
            }
        }

        public void ToRegistryKey(RegistryKey parent)
        {
            String keyName = "VoodooShader";

            RegistryKey key = parent.OpenSubKey(keyName);
            if (key != null)
            {
                key.Close();
                parent.DeleteSubKeyTree(keyName);
            }

            key = parent.CreateSubKey(keyName, RegistryKeyPermissionCheck.ReadWriteSubTree);

            key.SetValue("Path", m_Path, RegistryValueKind.String);
            key.SetValue("BinPrefix", m_BinPrefix, RegistryValueKind.String);

            ToRegistryKey_MakeList(key, "Remotes", ref m_Remotes);
            ToRegistryKey_MakeList(key, "Packages", ref m_Packages);
            ToRegistryKey_MakeList(key, "Modules", ref m_Modules);
            ToRegistryKey_MakeList(key, "Classes", ref m_Classes);
            ToRegistryKey_MakeList(key, "Hooks", ref m_Hooks);
            ToRegistryKey_MakeList(key, "Defaults", ref m_Defaults);

            key.Close();
        }

        private void ToRegistryKey_MakeList<T>(RegistryKey key, String subkey, ref List<T> inlist) where T : IVoodooRegistryObject, new()
        {
            RegistryKey listKey = key.CreateSubKey(subkey, RegistryKeyPermissionCheck.ReadWriteSubTree);
            foreach (T listItem in inlist)
            {
                listItem.ToRegistryKey(listKey);
            }
        }

        public List<VoodooRemote> Remotes
        {
            get { return m_Remotes; }
        }

        public List<VoodooPackage> Packages
        {
            get { return m_Packages; }
        }

        public List<VoodooModule> Modules
        {
            get { return m_Modules; }
        }

        public List<VoodooClass> Classes
        {
            get { return m_Classes; }
        }

        public List<VoodooHook> Hooks
        {
            get { return m_Hooks; }
        }

        public List<VoodooDefault> Defaults
        {
            get { return m_Defaults; }
        }
    }
}
